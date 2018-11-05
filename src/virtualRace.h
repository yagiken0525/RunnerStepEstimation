//
// Created by yagi on 18/10/02.
//

#ifndef RUNNERSTEPS_VIRTUALRACE_H
#define RUNNERSTEPS_VIRTUALRACE_H

#include <iostream>
#include <opencv2/opencv.hpp>

class virtualRace {
public:
    std::vector<cv::Point2f> cornerPoints;
    std::vector<cv::Mat> HomographyList;
    std::vector<cv::Mat> srcImages;
    float smallPanoramaWidth;
    float PanoramaWidth;
    float smallPanoramaHeight;
    float PanoramaHeight;

    void readSavedData(std::string videoName);
    void loadImages(std::string videoName);
};


#endif //RUNNERSTEPS_VIRTUALRACE_H
