//
// Created by 八木賢太郎 on 2018/01/07.
//

//RANSAC
#pragma comment(lib, "opencv_core2410d")

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include "calcLiniorEquation.h"

using namespace std;
using namespace cv;

// 特異値分解による方程式解法
void calcLiniorEquationSVM(vector<cv::Point2f> point, double &grad, double &segment, cv::Mat test) {

    //必要な変数を宣言
    cv::Mat left_side, right_side, solution;

    //全点のうちからランダムに選ぶ
    int x1 = get_random(0, point.size());
    int x2 = get_random(0, point.size());

    //点をMat型にする必要ある
    solution = cv::Mat(2, 1, CV_64FC1);

    cv::Scalar color(255,255,255);
    cv::circle(test, point[x1], 8, color);
    cv::circle(test, point[x2], 8, color);

    //この行列は計算用
    left_side = (Mat_<double>(2, 2) << point[x1].x, 1, point[x2].x, 1);
    right_side = (Mat_<double>(2, 1) << point[x1].y, point[x2].y);

    cv::solve(left_side, right_side, solution);

    //直線の方程式gradとsegment
    grad = solution.at<double>(0, 0);
    segment = solution.at<double>(1, 0);
}


//RANSACで直線の方程式求める
void RANSACgetLiniorEquation(vector<cv::Point2f> point, double &grad, double &segment, bool gnuplot,
                             const int LOOP_LIMIT, const int THRESHOLD, const int INLIER_RANGE) {

    //最も良い値を記録
    int max_inlier = 0;
    float best_grad;
    float best_segment;

    for (int i = 0; i < LOOP_LIMIT; i++) {

        //特異値分解で方程式求めてる
        cv::Mat test = cv::Mat::zeros(360, 640, CV_8U);
        calcLiniorEquationSVM(point, grad, segment, test);

        cv::Point2f pt1;
        cv::Point2f pt2;
        pt1.x = 0;
        pt1.y = segment;
        pt2.x = 640;
        pt2.y = 640 * grad + segment;
        vector<cv::Point2f> pts;
        pts.push_back(pt1);
        pts.push_back(pt2);


        //inlier配列生成
        vector<cv::Point2f> inliers;

        //求めた方程式がどれだけ他の点と一致しているのか
        //全ての点に対するループ
        for (int j = 0; j < point.size(); j++) {

            //inlier点をカウント
            if (point[j].y <= (grad * point[j].x + segment + INLIER_RANGE) &&
                point[j].y >= (grad * point[j].x + segment - INLIER_RANGE)) {

                //inlierに格納
                inliers.push_back(point[j]);
            }
        }

        //もしも最大inlier数更新したらbest~~を更新
        if (max_inlier < inliers.size()){
            best_grad = grad;
            best_segment = segment;
            max_inlier = inliers.size();
        }
    }

    grad = best_grad;
    segment = best_segment;

}

int get_random(int min, int max) {
    return min + (int) (rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
}

//最小二乗法でより正確な方程式求める
void re_estimate(double &a, double &b, vector<cv::Point2f> inliers) {
    a = b = 0;
    //A = sigma (x * y), B = sigma (x), C = sigma (y), D = sigma (x ^ 2)
    double A, B, C, D;
    A = B = C = D = 0;
    for (int i = 0; i < inliers.size(); ++i) {
        A += inliers[i].x * inliers[i].y;
        B += inliers[i].x;
        C += inliers[i].y;
        D += pow(inliers[i].x, 2.0);
    }
    a = (inliers.size() * A - B * C) / (inliers.size() * D - pow(B, 2.0));
    b = (D * C - A * B) / (inliers.size() * D - pow(B, 2.0));
}


void calcLine(cv::Mat image, cv::Point2f *pt1, cv::Point2f *pt2) {

    vector<cv::Point2f> target_point;
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            if (image.at<unsigned char>(i, j) == 255) {
                cv::Point2f point(j, i);
                target_point.push_back(point);
            }
        }
    }

    double a = 0;
    double b = 0;
    double &grad = a;
    double &segment = b;
    RANSACgetLiniorEquation(target_point, grad, segment, false, 30, 300, 1);

    //y = ax からその直線上の二点を計算
    pt1->x = 0;
    pt1->y = segment;
    pt2->x = 500;
    pt2->y = 500 * grad + segment;

}
