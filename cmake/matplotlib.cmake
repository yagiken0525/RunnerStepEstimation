#${project name}config.cmake を探す場所を指定
#set(OpenCV_DIR /Users/yagikentarou/UserLibrary/opencv/opencv/build)

find_package(PythonLibs REQUIRED)

if (PythonLibs_FOUND)
    #include一覧に格納
    include_directories(${PYTHON_INCLUDE_DIRS})
    #linkライブラリ一覧に格納
    #link_directories (${PYTHON_LIBRARIES})

    #debug用出力
    message(STATUS "Python_include_DIR: ${PYTHON_INCLUDE_DIR}")
    message(STATUS "Pythonlib libraries : ${PYTHON_LIBRARIES}")
else()
    message(WARNING "Could not find Python Lib.")
endif()