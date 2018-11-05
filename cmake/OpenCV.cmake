#${project name}config.cmake を探す場所を指定
#set(OpenCV_DIR /home/yagi/UserLibrary/opencv-2.4.13/Build)

#findcmake,configcmakeを探す
find_package(OpenCV REQUIRED)

if (OpenCV_FOUND)
    #include一覧に格納
    include_directories(${OpenCV_INCLUDE_DIRS})
    #linkライブラリ一覧に格納
    link_directories (${OpenCV_LIBS})
    #debug用出力
    message(STATUS "OpenCV_DIR: ${OpenCV_DIR}")
    message(STATUS "OpenCV version: ${OpenCV_VERSION}")
    message(STATUS "OpenCV include: ${OpenCV_INCLUDE_DIRS}")
    message(STATUS "OoenCV libraries : ${OpenCV_LIBS}")
else()
    message(WARNING "Could not find OpenCV.")
endif()