from hkws_cap_py import Pack,HkwsConnecter 
import cv2
import numpy as np
class RC_TYPE:
    def __init__(self):
        pass 
    RC_NULL = 0#什么都没有
    RC_DATA=1#数据回调
    RC_LOGIN_ERR=2#登录出错
    RC_RECONN=3#连接被断开，正在重连
    RC_GET_DATA_CH=4#//打开传输通道出错
    RC_DATA_TSANSFER_ERR=5#//数据传输出错 比如阻塞
    RC_CLOSE=6

class HkVideoCapture():
    def __init__(self,ip:str,port:int,user:str,pw:str,pool_size:int=8) -> None:
        self.__hkws=HkwsConnecter()
        self.__hkws.SetConfig(ip,port,user,pw,pool_size)
        self.__hkws.StartInAsync()
        pass
    def read(self):
        pack=self.__hkws.Cap()
        # print(pack.GetRcType())
        ret=0
        frame={}
        if pack.GetRcType()==RC_TYPE.RC_DATA:
            ret=1
            # print("{},{}".format(pack.GetHeight(),pack.GetWidth()))
            yuv=np.frombuffer(pack.GetYuv(), dtype=np.uint8)
            frame = yuv.reshape(pack.GetHeight()+ pack.GetHeight()// 2, pack.GetWidth())
            frame=cv2.cvtColor(frame,cv2.COLOR_YUV2BGR_YV12)
        if pack.GetRcType()>1:
            ret=-1
        return ret,frame   
 
    def release(self):
        self.__hkws.ReClose()
        pass
    pass


#how to use:
# import signal
# import sys
# exit_cap={}

# def exit_handler():
#     if exit_cap:
#        exit_cap.release()
#     pass
 
# def sig_handler(signum, frame):
#     sys.exit(0)

# if __name__ == '__main__':
  
#     signal.signal(signal.SIGTERM, sig_handler)  # kill pid
#     signal.signal(signal.SIGINT, sig_handler)  # ctrl -c

#     cv2.namedWindow('preview') 
#     cap=HkVideoCapture("192.168.0.100",8000,"admin","can123456",10)
#     exit_cap=cap
#     while(1):
#         ret,frame=cap.read()
#         if ret==1:
#             pass
#             cv2.imshow("preview",frame)
#         else:
#             pass
#             # print("err{}".format(ret))
#         c=cv2.waitKey(1)
#         if c==27:
#             break
#     cap.release()
#     pass