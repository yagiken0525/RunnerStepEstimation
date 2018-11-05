//
// Created by 八木賢太郎 on 2018/01/03.
//

#include "videoToImage.h"
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

/**
 * @brief ビデオから画像へ分割
 * @param (string video_name) 入力動画へのパス(拡張子ありで)
 * @param (string image_save_directory) 画像保存先へのパス
 * @param (string image_format = ".jpg") 画像の保存形式(.jpg, .mpg ...etc)
 * @param (bool output_list_txt = true) 画像一覧をテキストで出力するかどうか
 * @author 八木賢太郎
 * @date 2018/1/3
 */

namespace videoToImage {
//ビデオから画像へ分割
    void videoToImage(string video_name, string image_save_directory,
                      bool output_list_txt, string imagelist_name, string image_format) {

        //入力ファイル名確認
        string video_path = video_name;
        cout << "input video name: " << video_path << endl;

        //動画読み込み
        cv::VideoCapture capture(video_name);
        int frame_counter = 0;

        //画像保存先の確保
        string image_folder = image_save_directory;
        cout << "output images directory: " << image_folder << endl;

        //ディレクトリ作成
        const char *cstr = image_folder.c_str();
        if (mkdir(cstr, 0777) == 0) {
            printf("directory correctly generated\n");
        } else {
            printf("directory already exists\n");
        }

        //動画からフレームへの分割
        //テキストファイルに出力する場合
        if (output_list_txt) {

            //テキストファイルオープン
            string imagelist_path = image_save_directory + imagelist_name;
            ofstream outputfile(imagelist_path);
            cout << "saving imagelist.txt: " << imagelist_path << endl;

            //フレーム分だけキャプチャ
            while (capture.grab()) {
                cv::Mat frame;
                capture >> frame;
                string number = digitString(frame_counter++, 4);

                //画像保存
                string image_file_name;
                image_file_name = image_folder + "/image" + number + image_format;
                cv::imwrite(image_file_name, frame);
                outputfile << image_file_name << endl;
            }
        } else {

            //テキストファイル出力しない場合
            while (capture.grab()) {
                //フレーム分だけキャプチャ
                cv::Mat frame;
                capture >> frame;

                //画像保存
                string number = digitString(frame_counter++, 4);
                cv::imwrite(image_folder + "/image" + number + image_format, frame);
            }
        }
    }
}

//intの数字を4に変換
string digitString(int num, int digit) {
    char c[32];
    sprintf(c, "%d", num);

    string s(c);
    while (s.length() < digit) {
        s = "0" + s;
    }
    return s.c_str();
}

////画像一覧の.txtファイル生成
//void makeImageListTxt(string output_path) {
////    if (checkFileExistence(m_image_txt_path) == false)
//    outputImageListText(output_path);
//}

////テキストファイルが存在するか判定
//bool checkFileExistence(const string &str) {
//    ifstream ifs(str);
//    return ifs.is_open();
//}

////テキストファイル出力
//void outputImageListText(string output_path) {
//    ofstream outputfile(output_path);
//    string line;
//    for (int i = 0; i < m_image_num; i++) {
//        string number = digitString(i, 4);
//        line = "../images/" + m_video_name + "/image" + number + ".jpg";
//        outputfile << line << endl;
//    }
//}


