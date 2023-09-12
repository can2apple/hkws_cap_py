
class Pack:
    def __init__(self):
        pass 
    """
      return yuv
    """
    def GetYuv()->bytes:
        pass
    """
       compare to RC_TYPE
    """
    def GetRcType()->int:
        pass
    def GetWidth()->int:
        pass
    def GetHeight()->int:
        pass
    pass


class HkwsConnecter:
    def __init__(self):
        pass 
    ###
    # pool_max_size:defaue:8
    ###
    def SetConfig(self,ip:str, port:int,user_name:str, pw:str,pool_max_size:int):
        pass
    def StartInAsync():
        pass
    ##
    # return:Pack
    # #
    def Cap()->Pack:
        pass
    def ReClose():
        pass