//
// Created by 八木賢太郎 on 2018/03/10.
//

#ifndef MAINTEST_RANSAC_H
#define MAINTEST_RANSAC_H

#include <iostream>  // for cout
#include <stdio.h>   // for printf()
#include <vector>
#include <opencv2/opencv.hpp>

namespace yagi {
    class RANSAC {
        // 各種変数
        int DIM_NUM;
        int i, j, k;
        double p, d, px;

    public:
        RANSAC(const int DIM_NUM);                         // コンストラクタ
        void RANSACgetnonLiniorEquation(std::vector<cv::Point2f> points,
                                        const int LOOP_LIMIT,
                                        const int THRESHOLD,
                                        const int INLIER_RANGE,
                                        const int POINT_NUM);

        int get_random_num(int min, int max);

        std::vector<float> calcLeastSquaresMethod(std::vector<cv::Point2f> points);  // 最小二乗法
        std::vector<cv::Point2f> loadPointsFromText(std::string textFilePath);

        double ipow(double, int);       // べき乗計算
    };
};

#endif //MAINTEST_RANSAC_H
