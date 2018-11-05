#おまじない　最低限必要なCmakeのバージョン指定
cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)

#project name ${PROJECT_NAME}で参照可能
project( runnerSteps )

#出力情報を増やして cmake デバッグしやすくします。
set(CMAKE_VERBOSE_MAKEFILE 1)

#コンパイルで使用するc++のバージョン指定
set(CMAKE_CXX_STANDARD 11)

#.cmakeファイル = module へのパス
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

# release か debugかを指定
set(CMAKE_BUILD_TYPE Debug)

# ちゃんとビルドして生成されたものは
# /build/ディレクトリにしまいましょうという指示
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR
            "CMake generation is not possible within the source directory!"
            "\n Remove the CMakeCache.txt file and try again from another folder, e.g.:"
            "\n "
            "\n rm CMakeCache.txt"
            "\n mkdir build"
            "\n cd build"
            "\n cmake .."
            )
endif()

# モジュールのinclude
find_package(GFlags)
find_package(Glog)
find_package(OpenPose REQUIRED)
include(OpenCV)
include(matplotlib)
include_directories(${OpenPose_INCLUDE_DIRS} ${GFLAGS_INCLUDE_DIR} ${GLOG_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS})

# subdirectoryの追加
add_subdirectory(src)
add_subdirectory(src/videoToImage)
add_subdirectory(src/plotRunnerPose)
add_subdirectory(src/basicFunction)
add_subdirectory(src/complimentFootprints)
add_subdirectory(src/openpose)
# add_subdirectory(src/HasegawaSan)

# 実行ファイルの指定
add_executable(${PROJECT_NAME} main.cpp)

# ライブラリのリンク
target_link_libraries(${PROJECT_NAME} Panorama)
target_link_libraries(${PROJECT_NAME} my_openpose)
target_link_libraries(${PROJECT_NAME} basicFunc)
target_link_libraries(${PROJECT_NAME} mymatplot)
target_link_libraries(${PROJECT_NAME} calcLiniorEquation)
target_link_libraries(${PROJECT_NAME} ransac)
target_link_libraries(${PROJECT_NAME} videoToImage)
target_link_libraries(${PROJECT_NAME} ${OpenPose_LIBS} ${GFLAGS_LIBRARY} ${GLOG_LIBRARY})
#target_link_libraries(${PROJECT_NAME} Interface)
#target_link_libraries(${PROJECT_NAME} Kalman)
#target_link_libraries(${PROJECT_NAME} Strobo)
#target_link_libraries(${PROJECT_NAME} Mean)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
