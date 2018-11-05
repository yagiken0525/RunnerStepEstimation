//
// Created by yagi on 18/07/18.
//

#include "panorama.h"
#include "basicFunction/basicFunction.h"
#include <opencv2/xfeatures2d.hpp>

using namespace std;
using namespace yagi;
using namespace cv;


// グローバル変数
cv::Rect box;
std::vector<cv::Rect> rect_mask_vec;
bool drawing_box = false;
bool area_selected = false;


void draw_box(cv::Mat* img, cv::Rect rect){
    cv::rectangle(*img, cv::Point2d(box.x, box.y), cv::Point2d(box.x + box.width, box.y + box.height),
                  cv::Scalar(0xff, 0x00, 0x00), 2);
}


void teropMaskCallBackFunc(int eventType, int x, int y, int flags, void *userdata) {

    cv::Mat *image = static_cast<cv::Mat *>(userdata);

    switch (eventType) {
        case cv::EVENT_MOUSEMOVE:
            if (drawing_box) {
                box.width = x - box.x;
                box.height = y - box.y;
            }
            break;

        case cv::EVENT_LBUTTONDOWN:
            drawing_box = true;
            box = cv::Rect(x, y, 0, 0);
            break;

        case cv::EVENT_LBUTTONUP:
            drawing_box = false;
            if (box.width < 0) {
                box.x += box.width;
                box.width *= -1;
            }
            if (box.height < 0) {
                box.y += box.height;
                box.height *= -1;
            }
            draw_box(image, box);
            area_selected = true;
            cout << box << endl;
            break;
    }
}


void Panorama::setMaskArea(cv::Rect mask_area, cv::Point2i center) {
    MaskArea mask;
    mask._mask_area = mask_area;
    mask_areas.push_back(mask);
}


void Panorama::selectMaskArea() {

    //クリックするための準備
    string window_name = "Select mask area (N: next frame, E: finish masking)";
    cv::namedWindow(window_name, CV_WINDOW_AUTOSIZE);
    cv::Scalar color(0, 255, 0);
    bool finish_masking = false;

    string file_name = "../images/" + this->_video_name + "/maskArea.txt";

    //テキストファイルからマスク領域を入力
    if (this->useLastMaskedArea){
        std::ifstream ifs(file_name);
        std::string str;

        cout << "[Select mask region]" << endl;
        vector<string> mask_rect;
        while (getline(ifs, str))
        {
//            std::cout << "[" << str << "]" << std::endl;
            mask_rect = yagi::split(str, ' ');
            cv::Rect temp(stoi(mask_rect[0]), stoi(mask_rect[1]), stoi(mask_rect[2]), stoi(mask_rect[3]));
            rect_mask_vec.push_back(temp);
        }

    }else {
        //テキストファイル準備
        ofstream outputfile(file_name);

        for (auto itr = image_info_list.begin(); itr != image_info_list.end(); ++itr) {

            //表示用の画像
            cv::Mat dummy = itr->image.clone();
            cv::Mat temp = itr->image.clone();
            cv::imshow(window_name, dummy);
            cv::setMouseCallback(window_name, teropMaskCallBackFunc, (void *)&dummy);

            while (1) {
                dummy.copyTo(temp);

                int key = cv::waitKey(1);
                if (key == 'n') {
                    break;
                }

                if (key == 'e') {
                    finish_masking = true;
                    break;
                }

                if (drawing_box) {
                    draw_box(&temp, box);
                }

                if (area_selected) {
                    cv::Rect temp = box;
                    rect_mask_vec.push_back(temp);
                    outputfile << box.x << " " << box.y << " " << box.width << " " << box.height << endl;
                    area_selected = false;
                }

                cv::imshow(window_name, temp);

            }

            if (finish_masking) {
                break;
            }
        }
        outputfile.close();
    }

    for (auto itr_rect = rect_mask_vec.begin(); itr_rect != rect_mask_vec.end(); ++itr_rect) {
        cv::Point2i center((itr_rect->tl().x + itr_rect->br().x) / 2,
                           (itr_rect->tl().y + itr_rect->br().y) / 2);
        setMaskArea(*itr_rect, center);
    }

    cout << "[Select mask area finished]" << endl;
    cv::destroyAllWindows();
}

