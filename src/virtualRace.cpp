//
// Created by yagi on 18/10/02.
//

#include "virtualRace.h"
#include "basicFunction/basicFunction.h"
using namespace std;
using namespace cv;

void virtualRace::readSavedData(std::string videoName){
    ifstream savedData("../images/" + videoName + "/savedData.txt");

    string str;
    vector<string> line_strings;
    while (getline(savedData, str))
    {
        line_strings = yagi::split(str, ' ');
        if(line_strings[0] == "Corner_points_panorama:"){
            for(int i = 1; i < 9; i+=2){
                cv::Point2f pt(stof(line_strings[i]), stof(line_strings[i+1]));
                this->cornerPoints.push_back(pt);
            }
        }
        if(line_strings[0] == "SmallSizePanorama:"){
            this->smallPanoramaWidth = stof(line_strings[1]);
            this->PanoramaWidth = stof(line_strings[3]);
            this->smallPanoramaHeight = stof(line_strings[2]);
            this->PanoramaHeight = stof(line_strings[4]);
        }
        if(line_strings[0] == "Frame:"){
            cv::Mat H = cv::Mat::zeros(3, 3, CV_64F);
            for(int i = 0; i < 3; i++){
                for(int j = 0; j < 3; j++){
                    H.at<double>(i, j) = stod(line_strings[i * 3 + j + 2]);
                }
            }

            this->HomographyList.push_back(H);
            cout << H << endl;
            cout << this->HomographyList[this->HomographyList.size() - 1] << endl;
        }

    }
}

void virtualRace::loadImages(string videoName) {
    //画像リストopen
    ifstream ifs("../images/" + videoName + "/imagelist.txt");

    //imshowうまくいかない時用
    string line;
    int string_size = 0;

    while (getline(ifs, line)) {

        //imshowがうまくいかないときここ原因(下4行をコメントアウト)
//        if (string_size == 0 || (string_size + 1) == line.size()) {
//            line.erase(line.size() - 1);
//        }
//        string_size = line.size();

        //カラー
        cv::Mat image = cv::imread(line);
        this->srcImages.push_back(image);
    }
}