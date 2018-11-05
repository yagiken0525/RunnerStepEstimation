//
// Created by yagi on 18/01/10.
//

#include "basicFunction.h"
#include "opencv2/highgui/highgui.hpp"


using namespace std;
using namespace cv;

string yagi::digitString(int num, int digit) {
    char c[32];
    sprintf(c, "%d", num);

    string s(c);
    while (s.length() < digit) {
        s = "0" + s;
    }
    return s.c_str();
}

void yagi::setColor(vector<cv::Scalar> *colors) {
    cv::Scalar color1(255, 255, 255);
    cv::Scalar color2(255, 0, 0);
    cv::Scalar color3(0, 255, 0);
    cv::Scalar color4(0, 0, 255);
    cv::Scalar color5(200, 0, 0);
    cv::Scalar color6(0, 200, 0);
    cv::Scalar color7(255, 0, 200);
    cv::Scalar color8(255, 255, 0);
    cv::Scalar color9(0, 255, 255);
    cv::Scalar color10(100, 255, 0);
    cv::Scalar color11(0, 100, 255);

    colors->push_back(color1);
    colors->push_back(color2);
    colors->push_back(color3);
    colors->push_back(color4);
    colors->push_back(color5);
    colors->push_back(color6);
    colors->push_back(color7);
    colors->push_back(color8);
    colors->push_back(color9);
    colors->push_back(color10);
    colors->push_back(color11);

}

void yagi::videoFlipSave(std::string videoname, std::string folderpath) {
    Mat img;
    VideoCapture cap(folderpath + videoname); //Windowsの場合　パス中の¥は重ねて¥¥とする
    int max_frame=cap.get(CV_CAP_PROP_FRAME_COUNT); //フレーム数
    cap>>img;
    VideoWriter writer(folderpath + "e.mp4", CV_FOURCC('M', 'P', 'E', 'G'), 30, img.size(), true);
    for(int i=0; i<max_frame - 1;i++){
        cap>>img ; //1フレーム分取り出してimgに保持させる

        cv::flip(img, img, -1);
        imshow("Video",img);
        waitKey(1); // 表示のために1ms待つ
        writer << img;
    }
}


cv::Mat yagi::maskAofB(cv::Mat A, cv::Mat B) {
    cv::Mat dst;
    A.copyTo(dst, B);
    return dst;
}


//2点からその直線の方程式
void yagi::getGradSegment(cv::Point2f pt1, cv::Point2f pt2, float *grad, float *segment) {
//    cout << "get grad segment " << pt1 << pt2 << endl;
    *grad = (pt1.y - pt2.y) / (pt1.x - pt2.x);
    *segment = pt1.y - (pt1.x * (*grad));
}

//2点を結ぶ直線描画
void yagi::drawLine(cv::Mat edgeMask, cv::Point2f pt1, cv::Point2f pt2, int thickness, cv::Scalar color) {
    float grad = (pt1.y - pt2.y) / (pt1.x - pt2.x);
    pt1.y = pt1.y - (grad * pt1.x);
    pt1.x = 0;

    pt2.x = edgeMask.cols;
    pt2.y = grad * pt2.x + pt1.y;

    cv::line(edgeMask, pt1, pt2, color, thickness);
}


void yagi::MatToVec(const cv::Mat &src, vector<vector<unsigned char>> &dst) {
    for (int i = 0; i < src.rows; i++) {
        dst.push_back(src.row(i));
    }
}


void yagi::VecToMat(const vector<vector<unsigned char>> src, cv::Mat &dst) {
    // Assume it already has edge information
//    cout << "aa" << endl;
    int height = src.size(), width = src[0].size();
    cv::Mat matrix(height, width, CV_8U);
    matrix = cv::Scalar::all(0);
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            matrix.at<unsigned char>(j, i) = src[j][i];
        }
    }
    dst = matrix;
}


//string用のsplit関数
vector<string> yagi::split(const string &s, char delim) {
    vector<string> elems;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        if (!item.empty()) {
            elems.push_back(item);
        }
    }
    return elems;
}


float yagi::distPoint2Line(cv::Point2f pt, float a, float b){
    float grad = -(1/a);
    float segment = pt.y - (grad * pt.x);
    cv::Point2f pt_line;
    pt_line.x = (segment - b)/(a - grad);
    pt_line.y = a * pt_line.x + b;

    return calc2PointDistance(pt, pt_line);

}

template<typename Point>
float yagi::calc2PointDistance(Point p1, Point p2) {
    int x_dist = pow((p1.x - p2.x), 2);
    int y_dist = pow((p1.y - p2.y), 2);
    float distance = sqrt((x_dist + y_dist));
    return distance;
}

void  yagi::mycalcfloatWarpedPoint(vector<cv::Point2f> next, vector<cv::Point2f> *warped, cv::Mat H) {

    for (int i = 0; i < next.size(); i++) {
        cv::Point2f next_pt = next[i];
        cv::Point2f warp_pt;

        warp_pt.x =
                H.at<float>(0, 0) * next_pt.x +
                H.at<float>(0, 1) * next_pt.y +
                H.at<float>(0, 2) * 1;

        warp_pt.y =
                H.at<float>(1, 0) * next_pt.x +
                H.at<float>(1, 1) * next_pt.y +
                H.at<float>(1, 2) * 1;

        float z = H.at<float>(2, 0) * next_pt.x +
                  H.at<float>(2, 1) * next_pt.y +
                  H.at<float>(2, 2) * 1;


        cout << warp_pt << endl;
        warp_pt.x = warp_pt.x / z;
        warp_pt.y = warp_pt.y / z;
        cout << warp_pt << endl;

        warped->push_back(warp_pt);

    }
}


void yagi::mycalcWarpedPoint(vector<cv::Point2f> next, vector<cv::Point2f> *warped, cv::Mat H) {

    for (int i = 0; i < next.size(); i++) {
        cv::Point2f next_pt = next[i];
        cv::Point2f warp_pt;

        warp_pt.x =
                H.at<double>(0, 0) * next_pt.x +
                H.at<double>(0, 1) * next_pt.y +
                H.at<double>(0, 2) * 1;

        warp_pt.y =
                H.at<double>(1, 0) * next_pt.x +
                H.at<double>(1, 1) * next_pt.y +
                H.at<double>(1, 2) * 1;

        double z = H.at<double>(2, 0) * next_pt.x +
                   H.at<double>(2, 1) * next_pt.y +
                   H.at<double>(2, 2) * 1;

        warp_pt.x = warp_pt.x / z;
        warp_pt.y = warp_pt.y / z;


        warped->push_back(warp_pt);

    }
}


void yagi::mycalcDoubleWarpedPoint(vector<cv::Point2d> next, vector<cv::Point2d> *warped, cv::Mat H) {

    for (int i = 0; i < next.size(); i++) {
        cv::Point2d next_pt = next[i];
        cv::Point2d warp_pt;

        warp_pt.x =
                H.at<double>(0, 0) * next_pt.x +
                H.at<double>(0, 1) * next_pt.y +
                H.at<double>(0, 2) * 1;

        warp_pt.y =
                H.at<double>(1, 0) * next_pt.x +
                H.at<double>(1, 1) * next_pt.y +
                H.at<double>(1, 2) * 1;

        double z = H.at<double>(2, 0) * next_pt.x +
                   H.at<double>(2, 1) * next_pt.y +
                   H.at<double>(2, 2) * 1;

        warp_pt.x = warp_pt.x / z;
        warp_pt.y = warp_pt.y / z;


        warped->push_back(warp_pt);

    }
}




