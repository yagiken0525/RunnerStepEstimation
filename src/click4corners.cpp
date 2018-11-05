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
bool clicked_4corners = false;
cv::Point2f clicked_point;


//コールバック関数
void runnerCallBackFunc(int eventType, int x, int y, int flags, void *userdata) {
    switch (eventType) {
        case cv::EVENT_LBUTTONUP:
            std::cout << x << " , " << y << std::endl;
            clicked_point.x = x;
            clicked_point.y = y;
            clicked_4corners = true;

    }
}


void Panorama::startFinishLineSelect() {

    cout << "[Click start line]" << endl;

    vector<cv::Scalar> colors;
    setColor(&colors);
    string file_name = "../images/" + this->_video_name + "/startFinishLine.txt";

    //最初のフレームでスタートラインクリック
    mouseParam mouseEvent;
    string windowName = "click start line (Q: finish clicking)";
    cv::namedWindow(windowName, CV_WINDOW_AUTOSIZE);
    cv::setMouseCallback(windowName, runnerCallBackFunc, &mouseEvent);
    cv::Mat image = image_info_list[0].image.clone();

    if (!useLastCornerPoints) {
        while (1) {
            cv::imshow(windowName, image);
            int key = cv::waitKey(1);

            if (clicked_4corners) {
                //click point格納
                clicked_4corners = false;
                cv::circle(image, clicked_point, 2, colors[0], 2);
                cv::Point2f pt(clicked_point.x, clicked_point.y);
                this->startLineCornerPoints.push_back(pt);
            }

            if (key == 'q')
                break;
        }

        //最後のフレームでゴールラインクリック
        cout << "Click finish line" << endl;

        windowName = "click finish line(Q: finish clicking, B: back to previous frame)";
        cv::namedWindow(windowName, CV_WINDOW_AUTOSIZE);
        cv::setMouseCallback(windowName, runnerCallBackFunc, &mouseEvent);
        bool lineSelected = false;

        for (int i = image_info_list.size() - 2; i > 0; i--) {
            cv::Mat lastImage = image_info_list[i].image.clone();

            while (1) {
                cv::imshow(windowName, lastImage);
                int key = cv::waitKey(1);

                if (clicked_4corners) {
                    //click point格納
                    clicked_4corners = false;
                    cv::circle(lastImage, clicked_point, 2, colors[0], 2);
                    cv::Point2f pt(clicked_point.x, clicked_point.y);
                    this->finishLineCornerPoints.push_back(pt);
                }

                if (key == 'b') {
                    break;
                }

                if (key == 'q') {
                    this->finalLineImageNum = i;
                    lineSelected = true;
                    break;
                }
            }
            if (lineSelected)
                break;
        }

        ofstream outputfile(file_name);
        for (cv::Point2f point: startLineCornerPoints){
            outputfile << point.x << " " << point.y;
            outputfile << endl;
        }
        for (cv::Point2f point: finishLineCornerPoints){
            outputfile << point.x << " " << point.y;
            outputfile << endl;
        }
        outputfile << this->finalLineImageNum;
        outputfile.close();

    }else{
        std::ifstream ifs(file_name);
        std::string str;
        vector<cv::Point2f> clicked_4cornersPoints;
        if (ifs.fail())
        {
            std::cerr << "クリックポイントが見つかりません" << std::endl;
        }

        int i = 0;
        while (getline(ifs, str))
        {
            if (i < 4) {
                vector<string> loaded = yagi::split(str, ' ');
                cv::Point2f loadPt(stof(loaded[0]), stof(loaded[1]));
                clicked_4cornersPoints.push_back(loadPt);
            }else{
                vector<string> loaded = yagi::split(str, ' ');
                this->finalLineImageNum = stoi(loaded[0]);
            }
            i++;
        }
        this->startLineCornerPoints.push_back(clicked_4cornersPoints[0]);
        this->startLineCornerPoints.push_back(clicked_4cornersPoints[1]);
        this->finishLineCornerPoints.push_back(clicked_4cornersPoints[2]);
        this->finishLineCornerPoints.push_back(clicked_4cornersPoints[3]);

    }

    cv::destroyAllWindows();

    cout << "[Click start line finished]" << endl;
}
