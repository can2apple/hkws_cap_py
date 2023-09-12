set(hkws_Found 1)
set(hkws_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include")
set(hkws_INCLUDE_DIR ${hkws_INCLUDE_DIRS})
set(hkws_LIBRARIES  ${CMAKE_CURRENT_LIST_DIR}/lib/HCCore.lib 
                    ${CMAKE_CURRENT_LIST_DIR}/lib/HCNetSDK.lib
                    ${CMAKE_CURRENT_LIST_DIR}/lib/PlayCtrl.lib
)  
set(hkws_LIBRARY ${hkws_LIBRARIES})                       
set(hkws_DLL_FILES ${CMAKE_CURRENT_LIST_DIR}/bin/HCCore.dll 
  ${CMAKE_CURRENT_LIST_DIR}/bin/AudioRender.dll 
  ${CMAKE_CURRENT_LIST_DIR}/bin/HCNetSDK.dll 
  ${CMAKE_CURRENT_LIST_DIR}/bin/hlog.dll 
  ${CMAKE_CURRENT_LIST_DIR}/bin/hpr.dll
  ${CMAKE_CURRENT_LIST_DIR}/bin/libcrypto-1_1-x64.dll
  ${CMAKE_CURRENT_LIST_DIR}/bin/libssl-1_1-x64.dll
  ${CMAKE_CURRENT_LIST_DIR}/bin/PlayCtrl.dll
  ${CMAKE_CURRENT_LIST_DIR}/bin/SuperRender.dll
  ${CMAKE_CURRENT_LIST_DIR}/bin/zlib1.dll
)
set(hkws_HCNetSDKCom_DLL_FILES ${CMAKE_CURRENT_LIST_DIR}/bin/HCNetSDKCom/AnalyzeData.dll 
  ${CMAKE_CURRENT_LIST_DIR}/bin/HCNetSDKCom/AudioIntercom.dll
  ${CMAKE_CURRENT_LIST_DIR}/bin/HCNetSDKCom/AudioRender.dll
  ${CMAKE_CURRENT_LIST_DIR}/bin/HCNetSDKCom/HCAlarm.dll
  ${CMAKE_CURRENT_LIST_DIR}/bin/HCNetSDKCom/HCCoreDevCfg.dll
  ${CMAKE_CURRENT_LIST_DIR}/bin/HCNetSDKCom/HCDisplay.dll
  ${CMAKE_CURRENT_LIST_DIR}/bin/HCNetSDKCom/HCGeneralCfgMgr.dll
  ${CMAKE_CURRENT_LIST_DIR}/bin/HCNetSDKCom/HCIndustry.dll
  ${CMAKE_CURRENT_LIST_DIR}/bin/HCNetSDKCom/HCPlayBack.dll
  ${CMAKE_CURRENT_LIST_DIR}/bin/HCNetSDKCom/HCPreview.dll
  ${CMAKE_CURRENT_LIST_DIR}/bin/HCNetSDKCom/HCVoiceTalk.dll
  ${CMAKE_CURRENT_LIST_DIR}/bin/HCNetSDKCom/libiconv2.dll
  ${CMAKE_CURRENT_LIST_DIR}/bin/HCNetSDKCom/OpenAL32.dll
  ${CMAKE_CURRENT_LIST_DIR}/bin/HCNetSDKCom/StreamTransClient.dll
  ${CMAKE_CURRENT_LIST_DIR}/bin/HCNetSDKCom/SystemTransform.dll
)

