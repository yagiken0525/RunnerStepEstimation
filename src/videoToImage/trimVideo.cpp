//
// Created by 八木賢太郎 on 2018/01/03.
//

#include "trimVideo.h"
#include <sys/stat.h>
#include <string>
#include "../basicFunction/basicFunction.h"
using namespace std;

/**
 * @brief ビデオのトリミング
 * @param (string video_name) 入力動画へのパス(拡張子ありで)
 * @param (string output_path) 動画保存先へのパス
 * @author 八木賢太郎
 * @date 2018/1/3
 */

namespace videoToImage {

//ビデオのトリミング
    void trimVideo(string video_name, string input_path, string output_path, string imagelist_name, string video_type) {

        //入力動画名
        string video_path = input_path + video_name + video_type;
        cout << "Trimming video name: " << video_path << endl;

        //動画キャプチャ
        cv::VideoCapture cap(video_path); //Macの場合
        vector<cv::Mat> image_list;
        cv::Mat img;

        //画像保存先の確保
        string image_folder(output_path + video_name);
        cout << "output images directory: " << output_path << endl;

        //ディレクトリ作成
        const char *cstr = image_folder.c_str();
        if (mkdir(cstr, 0777) == 0) {
            printf("directory correctly generated\n");
        } else {
            printf("directory already exists\n");
        }

        //出力する動画ファイルの設定をします。拡張子はWMV1で、毎秒15フレーム、画像縦横サイズは1024*1024。
        string output_file_name = output_path + video_name + video_type;
        cout << "Saving video name: " << output_file_name << endl;
        cv::VideoWriter writer(output_file_name,
                               CV_FOURCC('M', 'P', '4', 'V'),
                               cap.get(cv::CAP_PROP_FPS),
                               cv::Size((int) cap.get(cv::CAP_PROP_FRAME_WIDTH),
                                        (int) cap.get(cv::CAP_PROP_FRAME_HEIGHT)));

        if (!writer.isOpened()) { cout << "couldn't open writer" << endl; }


        int i = 0;
        int frame_counter = 0;

        //テキストファイルオープン
        string imagelist_path = output_path + video_name + imagelist_name;
        cout << "imageList " << imagelist_path << endl;
        ofstream outputfile(imagelist_path);

        while (1) {
            cap >> img; //1フレーム分取り出してimgに保持させる
            resize(img, img, cv::Size(), 640.0/img.cols ,320.0/img.rows);
            if (i == 0) {

                cv::imshow("'S' save, 'Q' quit, 'number' *100 frame skip", img);
                int key = cv::waitKey(0);

                if (key == 's') {
                    cout << "This frame was saved!" << endl;
                    string number = yagi::digitString(frame_counter++, 4);

                    //画像保存
                    string image_file_name;
                    image_file_name = image_folder + "/image" + number + ".jpg";
                    cv::imwrite(image_file_name, img);
                    outputfile << image_file_name << endl;
//                    writer.operator<<(img);
                }

                if (key == '0') {
                    cout << "30frame skipped" << endl;
                    i = 30;
                }

                if (key == '1') {
                    cout << "100frame skipped" << endl;
                    i = 100;
                }

                if (key == '2') {
                    cout << "200frame skipped" << endl;
                    i = 200;
                }

                if (key == '3') {
                    cout << "300frame skipped" << endl;
                    i = 300;
                }

                if (key == '4') {
                    cout << "400frame skipped" << endl;
                    i = 400;
                }

                if (key == '5') {
                    cout << "500frame skipped" << endl;
                    i = 500;
                }

                if (key == '6') {
                    cout << "600frame skipped" << endl;
                    i = 600;
                }

                if (key == '7') {
                    cout << "700frame skipped" << endl;
                    i = 700;
                }

                if (key == '8') {
                    cout << "800frame skipped" << endl;
                    i = 800;
                }

                if (key == '9') {
                    cout << "900frame skipped" << endl;
                    i = 900;
                }

                if (key == 'q') {
                    break;
                }

                if (key == 'z') {
                    break;
                }
            }
            if (i > 0)
                i--;
        }
        cout << "Video trimming finished " << endl;

    }

}