//
// Created by 八木賢太郎 on 2018/01/07.
//

#ifndef MAINTEST_CALCLINIOREQUATION_H
#define MAINTEST_CALCLINIOREQUATION_H

int get_random(int min, int max);
void RANSACgetLiniorEquation(std::vector<cv::Point2f> point, double &grad, double &segment, bool gnuplot = true,
                                  const int LOOP_LIMIT = 200 , const int THRESHOLD = 500, const int INLIER_RANGE = 5);
void re_estimate(double &a, double &b, std::vector<cv::Point2f> inliers);
void calcLine(cv::Mat image, cv::Point2f *pt1, cv::Point2f *pt2);

#endif //MAINTEST_CALCLINIOREQUATION_H
