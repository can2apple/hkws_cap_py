
#ifndef _h_hkws_sdk_to_opencv_imp_
#define _h_hkws_sdk_to_opencv_imp_
#include <iostream>
#include <functional>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <HCNetSDK.h>
//#include <plaympeg4.h>
#include <PlayM4.h>

class HkwsConnecter
{
public:

	enum  RC_TYPE
	{
		RC_NULL = 0,//什么都没有
		RC_DATA,//数据回调
		RC_LOGIN_ERR,//登录出错
		RC_RECONN,//连接被断开，正在重连
		RC_GET_DATA_CH,//打开传输通道出错
		RC_DATA_TSANSFER_ERR,//数据传输出错 比如阻塞
		RC_CLOSE
	};
	class Pack
	{
	public:
		Pack() {}
		~Pack() {}
		int rc_type = 0;//RC_TYPE
		std::string yuv;
		int width = 0;
		int height = 0;
		int size = 0;

	};
public:
	void SetConfig(const std::string& ip, const int port, const std::string& user_name, const std::string& pw,
		int pool_max_size)
	{
		this->_ip = ip;
		this->_port = port;
		this->_user_name = user_name;
		this->_pw = pw;
		this->_pool_max_size = pool_max_size;
	}

	Pack GetFrameInThread() {

	}
	void ReClose() {
		this->_if_close.store(true);
	}
	//void SetRc(RC_HANDLE rc) {
	//	_rc_data = rc;
	//}
	HkwsConnecter() {
		this->_if_close.store(false);
		this->_ip = "192.168.1.64";
		this->_port = 8000;
		this->_user_name = "admin";
		this->_pw = "123456";
		this->_pool_max_size = 10;
		this->_delay = 1;

	};
	~HkwsConnecter() {
	}
	void StartInAsync() {
		std::thread th = std::thread(&HkwsConnecter::_handle_thread, this);
		th.detach();
	}
	Pack Cap() {
		Pack re;
		re.rc_type = RC_NULL;
		Pack* t = NULL;
		t = get_stage();
		if (t != NULL) {
			re = *t;
			delete t;
		}
		else {
			t = get_data();
			if (t != NULL) {
				re = *t;
				delete t;
			}
		}
		return re;
	}


	void push_stage(int rc_t) {
		Pack* pk = new Pack();
		pk->rc_type = rc_t;
		rc_lock.lock();
		pool_stage.push_back(pk);
		rc_lock.unlock();
	}
	Pack* get_stage() {
		Pack* t = NULL;
		if (rc_lock.try_lock()) {
			if (pool_stage.size() > 0) {
				t = pool_stage[0];
				pool_stage.erase(pool_stage.begin());
			}
			rc_lock.unlock();
		}
		return t;
	}
	void push_data(int w, int h, int data_size, char* data) {
		Pack* pk = new Pack();
		pk->rc_type = RC_DATA;
		pk->width = w;
		pk->height = h;
		pk->size = data_size;
		pk->yuv.assign(data, data_size);
		rc_lock.lock();
		pool_pkg.push_back(pk);
		
		if (pool_pkg.size() > this->_pool_max_size) {
			Pack* t = pool_pkg[0];
			pool_pkg.erase(pool_pkg.begin());
			delete t;
		}
		rc_lock.unlock();
	}
	Pack* get_data() {
		Pack* t = NULL;
		//if (rc_lock.try_lock())
		{
			rc_lock.lock();
			if (pool_pkg.size() > 0) {
				t = pool_pkg.back();
				pool_pkg.pop_back();
			}
			rc_lock.unlock();
		}
		return t;
	}
private:
	void _handle_thread();
	
	void _clear_data() {
		for (int i = 0; i < pool_pkg.size(); i++) {
			delete pool_pkg[i];
		}
		pool_pkg.clear();

		for (int i = 0; i < pool_stage.size(); i++) {
			delete pool_stage[i];
		}
		pool_stage.clear();
	}
	std::atomic<bool> _if_close;
	std::string _ip = "192.168.1.64";
	int _port = 8000;
	std::string _user_name = "admin";
	std::string _pw = "123456";
	int _pool_max_size = 10;
	int _delay = 1;
	//RC_HANDLE _rc_data;
	std::vector<Pack*> pool_pkg = std::vector<Pack*>();
	std::vector<Pack*> pool_stage = std::vector<Pack*>();
	std::mutex rc_lock;
};

#endif