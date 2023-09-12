

## hkws_cap_py(海康威视 pyhon 采集)项目简介
**hkws_cap_py 基于海康威视sdk 进行python 绑定 **<br/>

### hkws_cap_py解决的问题：
    1. 修复sdk的一些bug(具体看 https://mp.csdn.net/mp_blog/creation/editor/132838403)。
    2. 使得获取摄像头数字像rtsp一样简单，但同时兼顾获取的速度。
### 代码调用：
```
#opencv 太大不上传git
from hkws_cap_imp import HkVideoCapture
import cv2
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
```

### 技术博客:
https://blog.csdn.net/u012357842