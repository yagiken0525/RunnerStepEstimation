//
// Created by yagi on 18/10/03.
//

#ifndef RUNNERSTEPS_NDEGREEAPPROXIMATION_H
#define RUNNERSTEPS_NDEGREEAPPROXIMATION_H

#include "opencv2/opencv.hpp"
#include "iostream"

class nDegreeApproximation {
public:
    nDegreeApproximation(int n, std::vector<cv::Point2d> data){
        _n = n;
        _N = n;
        _S = data.size();
        for(cv::Point2d pt: data){
            _data.push_back(pt);
        }
    }

    void gauss(cv::Mat a);
    void sai();

    int _n;
    int _N;
    int _S;
    std::vector<cv::Point2d> _data;
};


#endif //RUNNERSTEPS_NDEGREEAPPROXIMATION_H
