#include"hkws_py.h"

static  LONG nPort = -1;
static volatile int gbHandling = 3;
static void CALLBACK exception_rc(DWORD dwType, LONG lUserID, LONG lHandle, void* pUser);
static void CALLBACK real_data_rc(LONG lRealHandle, DWORD dwDataType, BYTE* pBuffer, DWORD dwBufSize, void* pUser);
static void CALLBACK dec_rc(long nPort, char* pBuf, long nSize, FRAME_INFO* pFrameInfo, void* user, void* nReserved2);


void HkwsConnecter::_handle_thread() {
	NET_DVR_Init();
	NET_DVR_SetConnectTime(3000, 3);
	NET_DVR_SetReconnect(30000, true);
	LONG lUserID;
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	LONG lRealPlayHandle = -1;
	lUserID = NET_DVR_Login_V30((char*)this->_ip.c_str(), this->_port, (char*)this->_user_name.c_str(), (char*)this->_pw.c_str(), &struDeviceInfo);
	if (lUserID < 0)
	{
		push_stage((int)RC_LOGIN_ERR);
		NET_DVR_Cleanup();
		this->_if_close.store(true);
	}
	else {
		NET_DVR_SetExceptionCallBack_V30(0, NULL, exception_rc, this);
		NET_DVR_CLIENTINFO ClientInfo;
		ClientInfo.lChannel = 1;        //Channel number 设备通道号
		ClientInfo.hPlayWnd = NULL;     //窗口为空，设备SDK不解码只取流
		ClientInfo.lLinkMode = 0;       //Main Stream
		ClientInfo.sMultiCastIP = NULL;
		LONG lRealPlayHandle;
		lRealPlayHandle = NET_DVR_RealPlay_V30(lUserID, &ClientInfo, real_data_rc, this, TRUE);
		if (lRealPlayHandle < 0)
		{
			push_stage((int)RC_GET_DATA_CH);
			this->_if_close.store(true);
		}
	}
	while (1) {
		bool expected = true;
		if (this->_if_close.compare_exchange_strong(expected, true)) {
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	if (lRealPlayHandle >= 0) {
		if (!NET_DVR_StopRealPlay(lRealPlayHandle))
		{
			printf("NET_DVR_StopRealPlay error! Error number: %d\n", NET_DVR_GetLastError());
		}
		NET_DVR_Logout(lUserID);
		NET_DVR_Cleanup();
	}
	_clear_data();
	push_stage((int)RC_CLOSE);
	this->_if_close.store(true);
}


static void CALLBACK exception_rc(DWORD dwType, LONG lUserID, LONG lHandle, void* pUser)
{
	if (dwType == EXCEPTION_RECONNECT) {
		HkwsConnecter* p = (HkwsConnecter*)pUser;
		p->push_stage((int)HkwsConnecter::RC_RECONN);
	}
}
static void CALLBACK real_data_rc(LONG lRealHandle, DWORD dwDataType, BYTE* pBuffer, DWORD dwBufSize, void* pUser)
{
	switch (dwDataType)
	{
	case NET_DVR_SYSHEAD:
		if (!PlayM4_GetPort(&nPort))
		{
			break;
		}
		if (dwBufSize > 0)
		{
			if (!PlayM4_SetStreamOpenMode(nPort, STREAME_REALTIME))
			{
				break;
			}
			if (!PlayM4_OpenStream(nPort, pBuffer, dwBufSize, 10 * 1024 * 1024))
			{
				break;
			}
			if (!PlayM4_Play(nPort, NULL))
			{
				break;
			}
			//自己把 sdk 头文件改了：真的大公司也做这种事，相当无语 void *指针
			//PLAYM4_API BOOL __stdcall PlayM4_SetDecCallBackMend(LONG nPort,void (CALLBACK* DecCBFun)(long nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, long nUser,long nReserved2), long nUser);(原来)
			//PLAYM4_API BOOL __stdcall PlayM4_SetDecCallBackMend(LONG nPort, void (CALLBACK * DecCBFun)(long nPort, char* pBuf, long nSize, FRAME_INFO * pFrameInfo, void* nUser, void* nReserved2), void* nUser);(修改后）
			if (!PlayM4_SetDecCallBackMend(nPort, dec_rc, pUser)) {
				break;
			}
			/*if (!PlayM4_SetDecCallBack(nPort, dec_rc))
			{
				break;
			}*/

		}
		break;
	case NET_DVR_STREAMDATA:
		if (dwBufSize > 0 && nPort != -1)
		{
			if (!PlayM4_InputData(nPort, pBuffer, dwBufSize))
			{
				HkwsConnecter* p = (HkwsConnecter*)pUser;
				p->push_stage((int)HkwsConnecter::RC_DATA_TSANSFER_ERR);
				break;
			}
		}
		break;
	default:
		if (dwBufSize > 0 && nPort != -1)
		{
			if (!PlayM4_InputData(nPort, pBuffer, dwBufSize))
			{
				break;
			}
		}
		break;
	}
}
static void CALLBACK dec_rc(long nPort, char* pBuf, long nSize, FRAME_INFO* pFrameInfo, void* pUser, void* nReserved2)
{
	if (gbHandling)
	{
		gbHandling--;
		return;
	}
	long lFrameType = pFrameInfo->nType;
	if (lFrameType == T_YV12)
	{
		HkwsConnecter* p = (HkwsConnecter*)pUser;
		p->push_data(pFrameInfo->nWidth, pFrameInfo->nHeight, nSize, pBuf);
	}
	gbHandling = 3;
}


PYBIND11_MODULE(hkws_cap_py, m) {
	py::class_<HkwsConnecter::Pack>(m, "Pack")
		.def(py::init<>())
		.def("GetYuv", &HkwsConnecter::Pack::GetYuv)
		.def("GetRcType", &HkwsConnecter::Pack::GetRcType)
		.def("GetWidth", &HkwsConnecter::Pack::GetWidth)
		.def("GetHeight", &HkwsConnecter::Pack::GetHeight)
		;
	py::class_<HkwsConnecter>(m, "HkwsConnecter")
		.def(py::init<>())
		.def("SetConfig", &HkwsConnecter::SetConfig)
		.def("StartInAsync", &HkwsConnecter::StartInAsync)
		.def("Cap", &HkwsConnecter::Cap)
		.def("ReClose", &HkwsConnecter::ReClose)
		;
}
//void main() {}