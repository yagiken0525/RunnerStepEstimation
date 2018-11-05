//
// Created by yagi on 18/07/18.
//

#include "panorama.h"
#include "basicFunction/basicFunction.h"
#include <opencv2/xfeatures2d.hpp>

using namespace std;
using namespace yagi;
using namespace cv;

//OpenPoseのデータ読みこみ
void Panorama::detectHumanArea() {
    cout << "[Detect human area]" << endl;
    ifstream ifs(_human_area_list);
    string line;
    HumanBody runner;
    vector<HumanBody> runners;
    vector<cv::Point2f> pre_centers;

    bool first_runner = true;
    int frame_counter = 0;

    while (getline(ifs, line)) {

        vector<string> coords = split(line, ' ');

        if (coords.size() == 5) {

            if (!first_runner) {

                HumanBody dummy_runner = runner;
                runners.push_back(dummy_runner);
                runner.clearBodyCoord();

                if (coords[1] == "0") {
                    vector<HumanBody> dummy_runners = runners;
                    allRunners.push_back(dummy_runners);

                    runners.clear();
                    frame_counter++;
                }
            }
        } else {
            runner.setBodyCoord(coords);
            runner.mask_rect = runner.getMaskRect();
            first_runner = false;
        }

        if (frame_counter == image_info_list.size()) {
            break;
        }
    }

    for (int i = 0; i < allRunners.size(); i++) {
        image_info_list[i].Runners = allRunners[i];
    }

    cout << "[Detect human area finished]" << endl;
}


void Panorama::HumanBody::setBodyCoord(vector<string> coord) {
    cv::Point2f coord_f(stof(coord[0]), stof(coord[1]));
    _body_parts_coord.push_back(coord_f);
}


vector<cv::Point2f> Panorama::HumanBody::getBodyCoord() {
    return this->_body_parts_coord;
}


void Panorama::HumanBody::clearBodyCoord() {
    _body_parts_coord.clear();
}


void Panorama::maskHumanArea(bool show_mask) {
    cout << "[Mask human area]" << endl;

    for (auto itr_frame = image_info_list.begin(); itr_frame != image_info_list.end(); ++itr_frame) {
        std::vector<HumanBody> runnersInFrame = itr_frame->Runners;
        std::vector<MaskArea> mask_in_frame;

        for (auto itr_runner = runnersInFrame.begin(); itr_runner != runnersInFrame.end(); ++itr_runner) {
            MaskArea area;
            area._mask_area = itr_runner->getMaskRect();
            mask_in_frame.push_back(area);
        }

        for (auto itr = mask_areas.begin(); itr != mask_areas.end(); ++itr) {
            mask_in_frame.push_back(*itr);
        }

        itr_frame->maskAreas = mask_in_frame;
    }



    //デバッグ マスク領域を表示

    for (auto itr_frame = image_info_list.begin(); itr_frame != image_info_list.end(); ++itr_frame) {

        cv::Scalar color(0, 0, 0);
        cv::Mat image = itr_frame->image;
        cv::Mat maskimage = cv::Mat::ones(itr_frame->image.size(), CV_8U) * 255;
        cv::Mat mask = image.clone();

        for (auto itr = itr_frame->maskAreas.begin(); itr != itr_frame->maskAreas.end(); ++itr) {
            cv::rectangle(mask, itr->_mask_area, color, CV_FILLED);
            cv::rectangle(maskimage, itr->_mask_area, color, CV_FILLED);
        }

        itr_frame->maskimage = maskimage;
        itr_frame->maskedrunner = mask;

        if (show_mask) {
            cv::imshow("mask", mask);
            cv::waitKey(0);
        }
    }

    cout << "[Mask human area finished]" << endl;
}


cv::Rect Panorama::HumanBody::getMaskRect() {
    int min_x = 10000;
    int max_x = 0;
    int min_y = 10000;
    int max_y = 0;
    for (auto itr_coord = _body_parts_coord.begin(); itr_coord != _body_parts_coord.end(); ++itr_coord) {
        int x = itr_coord->x;
        int y = itr_coord->y;
        if (min_x > x)
            if (x > 0)
                min_x = x;
        if (min_y > y)
            if (y > 0)
                min_y = y;
        if (max_x < x)
            max_x = x;
        if (max_y < y)
            max_y = y;
    }

    // マスク領域の指定
    int margin_x_left = 50;
    int margin_x_right = 50;
    int margin_y_up = 50;
    int margin_y_down = 80;
    return cv::Rect(min_x - margin_x_left, min_y - margin_y_up, max_x - min_x + margin_x_right,
                    max_y - min_y + margin_y_down);
}
