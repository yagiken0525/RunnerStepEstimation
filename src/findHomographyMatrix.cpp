//
// Created by yagi on 18/07/18.
//

#include "./panorama.h"
#include "basicFunction/basicFunction.h"

using namespace yagi;
using namespace std;

void Panorama::templateMatchingFindHomography() {

    cout << "[calculate translation by template matching]" << endl;

    cv::Mat thisImg = image_info_list[0].image.clone();

    //テンプレートマッチ用のrect size指定
    cv::Point2f start_pt(40, 20);
    int width = 550;
    int height = 280;
    cv::Rect tempRect(start_pt.x, start_pt.y, width, height);
    cv::Point2f pre_pt = start_pt;

    //ホモグラフィー用の点格納
    vector<cv::Point2f> this_points;
    vector<cv::Point2f> next_points;

    bool first_frame = true;
    int frame_num = 0;

    for (int i = 0; i < image_info_list.size(); i++) {
        if(i == 0){
            cv::Mat initH = cv::Mat::zeros(3, 3, CV_64F);
            initH.at<double>(0, 0) = 1;
            initH.at<double>(1, 1) = 1;
            initH.at<double>(2, 2) = 1;
            image_info_list[0].H = initH.clone();
        }else if(i > 0) {
            //このフレーム画像
            cv::Mat prevHsvImg = image_info_list[i - 1].hsv_image;
            cv::Mat thisHsvImg = image_info_list[i].hsv_image;
            vector<cv::Mat> prev_planes;
            vector<cv::Mat> this_planes;
            cv::split(prevHsvImg, prev_planes);
            cv::split(thisHsvImg, this_planes);

            cv::Mat prevImg = prev_planes[0];
            cv::Mat thisImg = this_planes[0];

            cv::Mat tempImg(thisImg, tempRect);
            cv::Mat tempMask(image_info_list[i].maskimage, tempRect);

            cv::Mat maskedTemp = tempImg.clone();
            for (int i = 0; i < tempMask.rows; i++) {
                for (int j = 0; j < tempMask.cols; j++) {
                    if (tempMask.at<unsigned char>(i, j) == 0) {
                        maskedTemp.at<unsigned char>(i, j) = 0;
                    }
                }
            }
//        cv::imshow("dummy mask area", maskedTemp);

            //テンプレートマッチング
            cv::Mat tempResult;
            cv::matchTemplate(prevImg, tempImg, tempResult, CV_TM_CCORR_NORMED, tempMask);

            // 最大のスコアの場所を探す
            cv::Rect roi_rect(0, 0, tempImg.cols, tempImg.rows);
            cv::Point max_pt;
            double maxVal;
            cv::minMaxLoc(tempResult, NULL, &maxVal, NULL, &max_pt);

            //Point型からPoint2f型に変換するため
            cv::Point2f max_pt2f;
            max_pt2f.x = max_pt.x;
            max_pt2f.y = max_pt.y;

            //前のフレーム(pre_pt)とトランスレーションが大きく異なる場合には第二最大値を選択
            if (!first_frame) {
                cv::Mat mask = cv::Mat::ones(tempResult.size(), CV_8U) * 255;
                while (calc2PointDistance(max_pt2f, pre_pt) > 3) {
                    mask.at<unsigned char>(max_pt) = 0;
                    cv::minMaxLoc(tempResult, NULL, &maxVal, NULL, &max_pt, mask);
                    max_pt2f.x = max_pt.x;
                    max_pt2f.y = max_pt.y;
                }
//                cout << frame_num << "PrePt " << pre_pt << ": maxPt " << max_pt << " "
//                     << calc2PointDistance(max_pt2f, pre_pt) << endl;
            } else {
                first_frame = false;
            }

            //pre_pt更新
            pre_pt = max_pt;
            roi_rect.x = max_pt.x;
            roi_rect.y = max_pt.y;

            //カラー画像でテンプレート最大値に対応領域を当て貼める
            cv::Mat prevDummy = image_info_list[i - 1].image.clone();
            cv::Mat thisDummy = image_info_list[i].image.clone();
            cv::Mat tempDummy(thisDummy, tempRect);
            cv::rectangle(prevDummy, roi_rect, cv::Scalar(255, 255, 255));

            for (int x = max_pt.x; x < max_pt.x + width; x++) {
                for (int y = max_pt.y; y < max_pt.y + height; y++) {
                    prevDummy.at<cv::Vec3b>(y, x) = tempDummy.at<cv::Vec3b>(y - max_pt.y, x - max_pt.x);
                }
            }
//        cv::imshow("dummy", prevDummy);
//        cv::waitKey();

            //平行移動から直線上の4点をhomographyの対応点として選択
            //prev frame用
            for (int pt = 1; pt < image_info_list[i - 1].grads.size() - 1; pt++) {
                float a1 = image_info_list[i - 1].grads[pt];
                float b1 = image_info_list[i - 1].segments[pt];

                cv::Point2f prev1(max_pt.x - start_pt.x, a1 * (max_pt.x - start_pt.x) + b1);
                cv::Point2f prev2(prevImg.cols, (a1 * prevImg.cols) + b1);

                image_info_list[i].prev_keypoints.push_back(prev1);
                image_info_list[i].prev_keypoints.push_back(prev2);
            }

            //this frame用
            for (int pt = 1; pt < image_info_list[i].grads.size() - 1; pt++) {
                float a1 = image_info_list[i].grads[pt];
                float b1 = image_info_list[i].segments[pt];

                cv::Point2f this1(0, b1);
                cv::Point2f this2(thisImg.cols - (max_pt.x - start_pt.x),
                                  a1 * (thisImg.cols - (max_pt.x - start_pt.x)) + b1);

                image_info_list[i].this_keypoints.push_back(this1);
                image_info_list[i].this_keypoints.push_back(this2);

//                デバッグ
                drawLine(image_info_list[i].trackLineImage, image_info_list[i].track_lines[pt].first, image_info_list[i].track_lines[pt].second, 1, cv::Scalar(255,0,0));
//                cv::imshow("aa",image_info_list[i].trackLineImage);
//                cv::waitKey();
            }

            //Debug：対応点を描画
            cv::Mat prev_dum = image_info_list[i - 1].trackLineImage.clone();
            cv::Mat this_dum = image_info_list[i].trackLineImage.clone();
            cv::Scalar GREEN(0, 255, 0);

            for (int pt = 0; pt < image_info_list[i].prev_keypoints.size() - 1; pt++) {
                cv::Point2f prev_pt = image_info_list[i].prev_keypoints[pt];
                cv::Point2f this_pt = image_info_list[i].this_keypoints[pt];
                cv::circle(prev_dum, prev_pt, 3, GREEN, 2);
                cv::circle(this_dum, this_pt, 3, GREEN, 2);
            }
//            cv::imshow("a", prev_dum);
//            cv::imshow("b", this_dum);
//            cv::waitKey();

            image_info_list[i].H = cv::findHomography(image_info_list[i].this_keypoints,
                                                          image_info_list[i].prev_keypoints,
                                                          CV_RANSAC, 1.0);
        }
        frame_num++;
    }
    cv::destroyAllWindows();
    cout << "[calculate translation by template matching finished]" << endl;

}

//void Panorama::templateInverseMatchingFindHomography() {
//
//    cout << "calculate translation from last frame by template matching" << endl;
//
//    cv::Mat thisImg = image_info_list[0].image.clone();
//
//    //テンプレートマッチ用のrect size指定
//    cv::Point2f start_pt(40, 20);
//    int width = 550;
//    int height = 280;
//    cv::Rect tempRect(start_pt.x, start_pt.y, width, height);
//    cv::Point2f pre_pt = start_pt;
//
//    //ホモグラフィー用の点格納
//    vector<cv::Point2f> this_points;
//    vector<cv::Point2f> next_points;
//    bool first_frame = true;
//
//    //最後のフレームのホモグラフィー
//    cv::Mat mul_H = cv::Mat::zeros(3, 3, CV_64F);
//    mul_H.at<double>(0, 0) = 1;
//    mul_H.at<double>(1, 1) = 1;
//    mul_H.at<double>(2, 2) = 1;
//    image_info_list[image_info_list.size() - 1].inverseH = mul_H.clone();
//
//    for (int i = image_info_list.size() - 2; i >= 0; i--) {
//
//        //このフレーム画像
//        cv::Mat thisHsvImg = image_info_list[i].hsv_image;
//        cv::Mat nextHsvImg = image_info_list[i + 1].hsv_image;
//        vector<cv::Mat> this_planes;
//        vector<cv::Mat> next_planes;
//        cv::split(thisHsvImg, this_planes);
//        cv::split(nextHsvImg, next_planes);
//
//        cv::Mat thisImg = this_planes[0];
//        cv::Mat nextImg = next_planes[0];
//
//        cv::Mat tempImg(nextImg, tempRect);
//        cv::Mat tempMask(image_info_list[i].maskimage, tempRect);
//
//        //テンプレートマッチング
//        cv::Mat tempResult;
//        cv::matchTemplate(thisImg, tempImg, tempResult, CV_TM_CCORR_NORMED, tempMask);
//
//        // 最大のスコアの場所を探す
//        cv::Rect roi_rect(0, 0, tempImg.cols, tempImg.rows);
//        cv::Point max_pt;
//        double maxVal;
//        cv::minMaxLoc(tempResult, NULL, &maxVal, NULL, &max_pt);
//
//        //Point型からPoint2f型に変換するため
//        cv::Point2f max_pt2f;
//        max_pt2f.x = max_pt.x;
//        max_pt2f.y = max_pt.y;
//
//        //前のフレーム(pre_pt)とトランスレーションが大きく異なる場合には第二最大値を選択
//        if (!first_frame) {
//            cv::Mat mask = cv::Mat::ones(tempResult.size(), CV_8U) * 255;
//            while (calc2PointDistance(max_pt2f, pre_pt) > 3) {
//                mask.at<unsigned char>(max_pt) = 0;
//                cv::minMaxLoc(tempResult, NULL, &maxVal, NULL, &max_pt, mask);
//                max_pt2f.x = max_pt.x;
//                max_pt2f.y = max_pt.y;
//            }
//        } else {
//            first_frame = false;
//        }
//
//        //pre_pt更新
//        pre_pt = max_pt;
//        roi_rect.x = max_pt.x;
//        roi_rect.y = max_pt.y;
//
//        //カラー画像でテンプレート最大値に対応領域を当て貼める
//        cv::Mat thisDummy = image_info_list[i].image.clone();
//        cv::Mat nextDummy = image_info_list[i + 1].image.clone();
//        cv::Mat tempDummy(nextDummy, tempRect);
//        cv::rectangle(thisDummy, roi_rect, cv::Scalar(255, 255, 255));
//
//        for (int x = max_pt.x; x < max_pt.x + width; x++) {
//            for (int y = max_pt.y; y < max_pt.y + height; y++) {
//                thisDummy.at<cv::Vec3b>(y, x) = tempDummy.at<cv::Vec3b>(y - max_pt.y, x - max_pt.x);
//            }
//        }
//
//        //平行移動から直線上の4点をhomographyの対応点として選択
//        //this frame用
//        for (int pt = 1; pt < image_info_list[i].grads.size() - 1; pt++) {
//            float a1 = image_info_list[i].grads[pt];
//            float b1 = image_info_list[i].segments[pt];
//
//            cv::Point2f this1(max_pt.x - start_pt.x, a1 * (max_pt.x - start_pt.x) + b1);
//            cv::Point2f this2(thisImg.cols, (a1 * thisImg.cols) + b1);
//
//            image_info_list[i].this_keypoints.push_back(this1);
//            image_info_list[i].this_keypoints.push_back(this2);
//        }
//
//        //next frame用
//        for (int pt = 1; pt < image_info_list[i + 1].grads.size() - 1; pt++) {
//            float a1 = image_info_list[i + 1].grads[pt];
//            float b1 = image_info_list[i + 1].segments[pt];
//
//            cv::Point2f next1(0, b1);
//            cv::Point2f next2(nextImg.cols - (max_pt.x - start_pt.x),
//                              a1 * (nextImg.cols - (max_pt.x - start_pt.x)) + b1);
//
//            image_info_list[i].next_keypoints.push_back(next1);
//            image_info_list[i].next_keypoints.push_back(next2);
//        }
//
//        //Debug：対応点を描画
//        cv::Mat this_dum = image_info_list[i].image.clone();
//        cv::Mat next_dum = image_info_list[i + 1].image.clone();
//        cv::Scalar GREEN(0, 255, 0);
//
//        for (int pt = 0; pt < image_info_list[i].this_keypoints.size() - 1; pt++) {
//            cv::Point2f this_pt = image_info_list[i].this_keypoints[pt];
//            cv::Point2f next_pt = image_info_list[i].next_keypoints[pt];
//            cv::circle(this_dum, this_pt, 3, GREEN, 2);
//            cv::circle(next_dum, next_pt, 3, GREEN, 2);
//        }
//
////        cv::imshow("t_dum", this_dum);
////        cv::imshow("n_dum", next_dum);
////        cv::waitKey();
//
//        image_info_list[i].inverseH = cv::findHomography(image_info_list[i].this_keypoints, image_info_list[i].next_keypoints,
//                                                  CV_RANSAC, 3.0);
//
//    }
//    cv::destroyAllWindows();
//
//    cout << "calculate translation finished" << endl;
//}