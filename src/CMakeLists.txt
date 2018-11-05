cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)

#include_directories(matplotlibcpp.h)
add_library(Panorama STATIC panorama.cpp panorama.h loadImage.cpp trackingTrack.cpp click_panorama.cpp maskTeropArea.cpp loadOpenPoseData.cpp findHomographyMatrix.cpp click4corners.cpp trackingRunner.cpp virtualRace.cpp virtualRace.h nDegreeApproximation.cpp nDegreeApproximation.h)
add_library(calcLiniorEquation calcLiniorEquation.cpp calcLiniorEquation.h openpose/exportHumanPoseTxt.cpp)
#add_library(HOG STATIC HOGfeature.cpp HOGfeature.h)
#add_library(KLT STATIC KLTtracker.cpp KLTtracker.h)
#add_library(HumanPose STATIC humanPose.cpp humanPose.h plotRunnerPose/matplotlibcpp.h)
#target_link_libraries(HumanPose ${PYTHON_LIBRARIES})