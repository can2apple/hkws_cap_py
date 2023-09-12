from hkws_cap_imp import HkVideoCapture
import cv2

if __name__ == '__main__':

    cv2.namedWindow('preview') 
    cap=HkVideoCapture("192.168.0.100",8000,"admin","can123456",10)
    while(1):
        ret,frame=cap.read()
        if ret==1:
            cv2.imshow("preview",frame)
        else:
            pass
        c=cv2.waitKey(1)
        if c==27:
            break
    cap.release()