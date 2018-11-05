//
// Created by 八木賢太郎 on 2018/01/05.
//

#ifndef MAINTEST_MYMATPLOT_H
#define MAINTEST_MYMATPLOT_H

#include <opencv2/opencv.hpp>
#include "matplotlib.h"

#define STR(var) #var   //引数にした変数を変数名を示す文字列リテラルとして返すマクロ関数

#include <iostream>

using namespace std;

enum bodyID {
    Nose = 0,
    Neck = 1,
    RShoulder = 2,
    RElbow = 3,
    RWrist = 4,
    LShoulder = 5,
    LElbow = 6,
    LWrist = 7,
    RHip = 8,
    RKnee = 9,
    RAnkle = 10,
    LHip = 11,
    LKnee = 12,
    LAnkle = 13,
    REye = 14,
    LEye = 15,
    REar = 16,
    LEar = 17,
    Bkg = 18
};

class MyMatPlot {

private:
    vector<cv::Point2f> Nose;
    vector<cv::Point2f> Neck;
    vector<cv::Point2f> RShoulder;
    vector<cv::Point2f> RElbow;
    vector<cv::Point2f> RWrist;
    vector<cv::Point2f> LShoulder;
    vector<cv::Point2f> LElbow;
    vector<cv::Point2f> LWrist;
    vector<cv::Point2f> RHip;
    vector<cv::Point2f> RKnee;
    vector<cv::Point2f> RAnkle;
    vector<cv::Point2f> LHip;
    vector<cv::Point2f> LKnee;
    vector<cv::Point2f> LAnkle;
    vector<cv::Point2f> REye;
    vector<cv::Point2f> LEye;
    vector<cv::Point2f> REar;
    vector<cv::Point2f> LEar;
    vector<cv::Point2f> Bkg;
    vector<float> grad;
    vector<float> segment;

public:
    static const int PARTS_NUM = 18;

    vector<cv::Point2f> get_vector(int i);

    string get_name(int i);

    void pushCoord(int i, cv::Point2f pt);

    template<typename Point>
    static float calcDistance(Point p1, Point p2);

    void plotDistanceTwoPoints(std::vector<cv::Point2f> pt1_list, std::vector<cv::Point2f> pt2_list,
                               std::string filename1 = "A", std::string filename = "B", int humanID = 0);

    void estimateFootPlotTiming(vector<cv::Point2f> pt1_list, vector<cv::Point2f> pt2_list
            , string filename1, string filename2, vector<int>* frame_nums, int humanID = 0);

    void plotDistanceToLine(vector<cv::Point2f> pt1_list, string filename1, vector<float> a, vector<float> b, int humanID = 0);

    void pushGrad(float a){this->grad.push_back(a);};

    void pushSegment(float b){this->segment.push_back(b);};

    float pullGrad(int i){return this->grad[i];};

    float pullSegment(int i){return this->segment[i];};

    vector<float> getGrads(){return this->grad;};

    vector<float> getSegment(){return this->segment;};

};


#endif //MAINTEST_MYMATPLOT_H
