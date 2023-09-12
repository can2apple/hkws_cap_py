cmake_minimum_required (VERSION 3.7)
project(hkws_cap_py)
option(IF_DEBUG         "Set to ON to build use debug"  OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fobjc-arc -std=c++latest -O2 -stdlib=libc++ -isysroot  ${CMAKE_OSX_SYSROOT}")
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined ${CMAKE_SHARED_LINKER_FLAG}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} 
                        ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


set(INEED_LIB_DIR  "./ineed")
if(WIN32)
    set (KITS_PAL x64)
    set(IF_USR_SOURCE_GROUP TRUE)
    set(CMAKE_C_FLAGS "-Wall ${CMAKE_C_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined ${CMAKE_SHARED_LINKER_FLAG}")
    ADD_DEFINITIONS(-DHAVE_CONFIG_H -D_CRT_SECURE_NO_WARNINGS -DWIN32_LEAN_AND_MEAN )
        include(CheckCXXCompilerFlag)
        CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
        if (_cpp_latest_flag_supported)
            add_compile_options("/std:c++latest")
        endif()
    set(_GOBAL_LIB  WS2_32.lib imm32.lib  Secur32.lib Shlwapi.lib bcrypt.lib vfw32.lib comctl32.lib OpenGL32.Lib
	      Winmm.lib Version.lib Strmiids.lib Iphlpapi.lib Psapi.lib userenv.lib netapi32.lib Crypt32.lib )
    #source:
    function(fun_file_filter _argv_)
        list(LENGTH ARGV  len)
        set(ind 0)
        while(ind LESS ${len})
            list(GET ARGV ${ind} node)  
            get_filename_component(PARENT_DIR "${node}" DIRECTORY)
            string(REPLACE "/" "\\" GROUP "${PARENT_DIR}")
            source_group("${GROUP}" FILES "${node}")
            math(EXPR ind "${ind}+1")
        endwhile()
    endfunction()
endif()

set(CMAKE_TYPE 1)
if (IF_DEBUG)
set(CMAKE_TYPE 0)
endif()
list(GET CMAKE_CONFIGURATION_TYPES ${CMAKE_TYPE} CMAKE_BUILD_TYPE)
set(CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE})
set(DEBUG_OR_RELEASE ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

#设置Python目标：
set(PYTHON_EXECUTABLE "C:/Python38/python.exe")
set(pybind11_DIR ${INEED_LIB_DIR}/pybind11/share/cmake/pybind11)
find_package(pybind11 REQUIRED CONFIG NO_DEFAULT_PATH)
set(hkws_DIR ${INEED_LIB_DIR}/hkws)
find_package(hkws REQUIRED CONFIG NO_DEFAULT_PATH)
set(OpenCV_DIR ${INEED_LIB_DIR}/opencv)
find_package(OpenCV REQUIRED CONFIG NO_DEFAULT_PATH)

set(DLL_FILES)
if(WIN32)
    set(OPENCV_DLL_FILES ${OpenCV_DIR}/x64/vc15/bin/opencv_ffmpeg346_64.dll ${OpenCV_DIR}/x64/vc15/bin/opencv_world346.dll)
    if (CMAKE_BUILD_TYPE STREQUAL Debug) 
        set(OPENCV_DLL_FILES ${OpenCV_DIR}/x64/vc15/bin/opencv_ffmpeg346_64.dll ${OpenCV_DIR}/x64/vc15/bin/opencv_world346d.dll)
    endif()
endif()
file(MAKE_DIRECTORY ${DEBUG_OR_RELEASE}) 

list(APPEND DLL_FILES ${Libuv_DLL_FILES} )
list(APPEND DLL_FILES ${protobuf_DLL_FILES} )

####海康威视
list(APPEND DLL_FILES ${OPENCV_DLL_FILES} )
list(APPEND DLL_FILES ${hkws_DLL_FILES} )
set(DEBUG_OR_RELEASE_HCNETSDKCOM ${DEBUG_OR_RELEASE}/HCNetSDKCom)
file(MAKE_DIRECTORY ${DEBUG_OR_RELEASE_HCNETSDKCOM}) 
file(COPY ${hkws_HCNetSDKCom_DLL_FILES} DESTINATION ${DEBUG_OR_RELEASE_HCNETSDKCOM}) 
file(COPY ${DLL_FILES} DESTINATION ${DEBUG_OR_RELEASE})  





add_executable( hkws_deom
    hkws_imp.h
    hkws_imp.cpp
   
)
target_include_directories(hkws_deom PRIVATE ${hkws_INCLUDE_DIR}  )

target_link_libraries(hkws_deom PRIVATE
${_GOBAL_LIB} 
${hkws_LIBRARIES} 
opencv_world
)

#set(Python_root "C:/Python38")
#set(Python_numpy_root ${Python_root}/Lib/site-packages/numpy/core )
#set(Python_INCLUDE_DIRS ${Python_root}/include ${Python_numpy_root}/include)
#set(Python_INCLUDE_DIR ${Python_INCLUDE_DIRS})
#set(Python_LIBRARIES  ${Python_root}/libs/python3.lib ${Python_root}/libs/python38.lib  
#  ${Python_numpy_root}/lib/npymath.lib
#)
#set(Python_LIBRARY ${Python_LIBRARIES})


pybind11_add_module(hkws_cap_py
       hkws_py.h
       hkws_py.cpp
)

target_include_directories(hkws_cap_py PRIVATE ${hkws_INCLUDE_DIR} )
target_link_libraries(hkws_cap_py PRIVATE ${_GOBAL_LIB}  ${hkws_LIBRARIES} )

SET(PY_FILES  hkws_cap_imp.py  hkws_cap_py.py  hkws_cap_demo.py)
list(APPEND RESULUT_FILES ${PY_FILES} )
file(COPY ${RESULUT_FILES} DESTINATION ${DEBUG_OR_RELEASE})  


