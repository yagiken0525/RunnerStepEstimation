//
// Created by yagi on 18/07/18.
//

//
// Created by yagi on 18/07/18.
//

#include "panorama.h"
#include <opencv2/xfeatures2d.hpp>
#include "basicFunction/basicFunction.h"


using namespace std;
using namespace yagi;
using namespace cv;

//グローバル変数
struct mouseParam {
    int x;
    int y;
};
bool flag = false;
cv::Point2f tracking_runner_point;

//コールバック関数
void runnerClickCallBackFunc(int eventType, int x, int y, int flags, void *userdata) {
    switch (eventType) {
        case cv::EVENT_LBUTTONUP:
            std::cout << x << " , " << y << std::endl;
            tracking_runner_point.x = x;
            tracking_runner_point.y = y;
            flag = true;
    }
}

void Panorama::trackTargetRunner() {

    vector<cv::Scalar> colors;
    setColor(&colors);

    vector<cv::Mat> plotImages;

    //ランナーが全員映るフレームでクリック
    mouseParam mouseEvent;
    cv::namedWindow("click runners", CV_WINDOW_AUTOSIZE);
    cv::setMouseCallback("click runners", runnerClickCallBackFunc, &mouseEvent);

    cv::Mat image = image_info_list[0].image.clone();
    while (1) {
        cv::imshow("click runners", image);
        int key = cv::waitKey(1);

        if (flag) {
            //click point格納
            flag = false;
            cv::circle(image, tracking_runner_point, 2, colors[0], 2);
        }

        if (key == 'q')
            break;
    }

    int frameID = 0;
    cv::Point2f prePt = tracking_runner_point;
    HumanBody preHb;
    cv::Mat preH;

    for (ImageInfo im : image_info_list) {
        float minDist = 20;
        int minId = 0;
        int hbID = 0;
        bool target_found = false;
        cv::Point2f minPt;
        HumanBody minHb;
        cv::Mat dum = im.image.clone();
        for (HumanBody hb: im.runnerCandidate) {

            vector<cv::Point2f> bodyCoords = hb.getBodyCoord();
            if (target_found == false) {
                for (int i = 0; i < bodyCoords.size(); i++) {
                    if ((i == 0) || (i == 14) || (i == 15) || (i == 16) || (i == 17)) {
                        float dist = yagi::calc2PointDistance(bodyCoords[i], prePt);
                        if (minDist > dist) {
                            minDist = dist;
                            minId = hbID;
                            minPt = bodyCoords[i];
                            minHb = hb;
                        }
                    }
                }
            } else {
                float sum_dist = 0;
                int parts_id = 0;
                int parts_num = 0;

                for (int i = 0; i < bodyCoords.size(); i++) {
                    for (cv::Point2f pre_pt: preHb.getBodyCoord()) {
                        if ((parts_id != 4) && (parts_id != 7) && (parts_id != 10) && (parts_id != 13)) {
                            if ((bodyCoords[i].x != 0) && (pre_pt.x != 0)) {
                                parts_num++;
                                sum_dist += yagi::calc2PointDistance(bodyCoords[i], pre_pt);
                            }
                        }
                    }
                }
                sum_dist /= parts_num;

                if (sum_dist < minDist) {
                    minDist = sum_dist;
                    minId = hbID;
                    minPt = im.runnerCandidate[minId].getBodyCoord()[0];
                    minHb = hb;
                }
            }
            hbID++;


            if (minDist != 20.0) {
                image_info_list[frameID].runnerCandidate[minId].humanID = 1;
                preH = im.H;
                prePt = minPt;
                preHb = minHb;
                target_found = true;
//                for (cv::Point2f pt : im.runnerCandidate[minId].getBodyCoord()) {
//                    cv::circle(im.image, pt, 2, colors[3], 2);
//                }

            }
        }

        frameID++;
//        cv::imshow("targetRunner", im.image);
//        cv::waitKey(10);

        if (frameID == (image_info_list.size() - 1))
            break;
    }

}
