
// * @file main.cpp
// * @brief 100m走の動画から各選手の歩幅、歩数を算出
// * @author 八木賢太郎
// * @date 2018/1/3
// */

#include <opencv2/opencv.hpp>
#include "src/panorama.h"
#include "src/openpose/myOpenPose.h"

//#include <openpose/flags.hpp>
//#include "src/nDegreeApproximation.h"
//#include "src/basicFunction/basicFunction.h"
using namespace std;
using namespace yagi;
using namespace cv;

int main(){

//    videoFlipSave("IMG_1762.MOV", "/home/yagi/sfmDR/inputVideos/");

    //! 入力動画のファイル名
    string video_name = "00008";
    string folder_path = "/home/yagi/sfmDR/inputVideos/" + video_name + "/";

    Panorama pano;
    pano.useLastMaskedArea = true;
    pano.useLastTrackLine = true;
    pano.useLastCornerPoints = true;
    pano.selectHumanlane = false;
    pano.runnerId = 6;

    //ファイルパスセット
    pano.setVariables(video_name);


    //ビデオから画像への変換
//    pano.videotoImage();


    bool DETECT_OPEN_POSE = true;
    if(DETECT_OPEN_POSE){
        outputTextFromVideo(folder_path, folder_path + video_name + ".mp4", folder_path );
    }



    //画像の読み込み
    //(true => 読み込んだ画像を表示)
    pano.loadImage(false);


    //クリックで映像中のテロップをマスク
    pano.selectMaskArea();


    //open poseの結果を読み込み
    pano.detectHumanArea();


    //マスク領域を削除
    //(true => マスク領域画像を表示)
    pano.maskHumanArea(false);


    //トラックをクリックで選択
    pano.selectTrack();


    //トラックをトラッキング
    //(true => 直線画像を表示)
    pano.trackTracking(false);


    //トラックの4角を選択
    pano.startFinishLineSelect();


    //トラック位置から選手候補人物を選択
    //(true => ランナー候補画像を表示)
    pano.selectRunnerCandidates(false);


    // テンプレートマッチングでホモグラフィー求める
    // 1フレーム目からのホモグラフィー
    pano.templateMatchingFindHomography();

    //パノラマ生成
    pano.generatePanorama();
//    pano.generateNthFramePanorama();

    //トラッｷングランナー
    pano.trackTargetRunner();


    //俯瞰画像へのホモグラフィー求める
    pano.getOverviewHomography();


    //俯瞰画像から5m毎の位置を推定し元画像に投影
//    pano.projectTrackLine();

//    pano.templateInverseMatchingFindHomography();
//    pano.generateInversePanorama();
//    pano.getInverseOverviewHomography();
//    pano.projectInverseTrackLine();

    //足跡投影
    pano.projectOverview();


    //ストロボ画像の生成
//    pano.makeStroboImage();
//    pano.makeStroboRangeImage(0, 250);


    //データのセーブ
    pano.saveData();

    cv::destroyAllWindows();
    //仮想対決
    pano.makeVirtualRaceImages("Bolt958");
    return 0;
}
