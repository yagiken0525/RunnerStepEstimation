//
// Created by 八木賢太郎 on 2018/03/10.
//

#include "RANSAC.h"
#include "../basicFunction/basicFunction.h"
#include <fstream>

//#define N 7      // データ数
//#define DIM 3     // 予測曲線の次数

using namespace std;
using namespace yagi;

/*
 * コンストラクタ
 */
RANSAC::RANSAC(const int dim) {

    DIM_NUM = dim;

}

//ランダムに値取得
int RANSAC::get_random_num(int min, int max) {
    return min + (int) (rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
}

/*
 * 最小二乗法
 */
vector<float> RANSAC::calcLeastSquaresMethod(vector<cv::Point2f> points) {

    double a[DIM_NUM + 1][DIM_NUM + 2], s[2 * DIM_NUM + 1], t[DIM_NUM + 1];

    // s, t 初期化
    for (i = 0; i <= 2 * DIM_NUM; i++)
        s[i] = 0;
    for (i = 0; i <= DIM_NUM; i++)
        t[i] = 0;

    int N = points.size();
    double *x;
    double *y;
    x = new double[N];
    y = new double[N];

    int index = 0;
    for (cv::Point2f point : points) {
        x[index] = point.x;
        y[index] = point.y;
        index++;
    }

    // s[], t[] 計算
    for (i = 0; i < N; i++) {
        for (j = 0; j <= 2 * DIM_NUM; j++)
            s[j] += ipow(x[i], j);
        for (j = 0; j <= DIM_NUM; j++)
            t[j] += ipow(x[i], j) * y[i];
    }

    // a[][] に s[], t[] 代入
    for (i = 0; i <= DIM_NUM; i++) {
        for (j = 0; j <= DIM_NUM; j++)
            a[i][j] = s[i + j];
        a[i][DIM_NUM + 1] = t[i];
    }

    // 掃き出し
    for (k = 0; k <= DIM_NUM; k++) {
        p = a[k][k];
        for (j = k; j <= DIM_NUM + 1; j++)
            a[k][j] /= p;
        for (i = 0; i <= DIM_NUM; i++) {
            if (i != k) {
                d = a[i][k];
                for (j = k; j <= DIM_NUM + 1; j++)
                    a[i][j] -= d * a[k][j];
            }
        }
    }

    // y 値計算＆結果出力
    for (k = 0; k <= DIM_NUM; k++)
        printf("a%d = %10.6f\n", k, a[k][DIM_NUM + 1]);
    printf("    x    y\n");
    for (px = 0; px <= N; px += 1) {
        p = 0;
        for (k = 0; k <= DIM_NUM; k++)
            p += a[k][DIM_NUM + 1] * ipow(px, k);
        printf("%5.0f %5.1f\n", px, p);
    }

    vector<float> dims;
    for (int i = 0; i < this->DIM_NUM + 1; i++){
        dims.push_back(float(a[i][this->DIM_NUM + 1]));
    }

    return dims;

}


/*
 * べき乗計算
 */
double RANSAC::ipow(double p, int n) {
    int k;
    double s = 1;

    for (k = 1; k <= n; k++)
        s *= p;

    return s;
}


//テキストからcv::Point2fを読み込み
vector<cv::Point2f> RANSAC::loadPointsFromText(string textFilePath) {

    ifstream ifs(textFilePath);
    string str;

    if (ifs.fail()) {
        cerr << "File do not exist.\n";
        exit(0);
    }

    vector<cv::Point2f> points;
    while (getline(ifs, str)) {
        cout << str << endl;
        vector<string> words = yagi::split(str, ' ');

        cv::Point2f point(stof(words[0]), stof(words[1]));
        points.push_back(point);
    }

    return points;
}

//RANSACで曲線の方程式求める
void RANSAC::RANSACgetnonLiniorEquation(vector<cv::Point2f> points,
                                        const int LOOP_LIMIT,
                                        const int THRESHOLD,
                                        const int INLIER_RANGE,
                                        const int POINT_NUM) {
    //変数
    int inlierMaxNum = 0;
    vector<float> bestA(this->DIM_NUM + 1);

    for (int loopCount = 0; loopCount < LOOP_LIMIT; loopCount++) {

        //変数
        int inlierNum = 0;

        //1ループ分のvector
        vector<cv::Point2f> randomPoints;

        //ランダムに点を取得
        vector<bool> numSelected(points.size(), false);
        for (int i = 0; i < POINT_NUM; i++) {
            int n = get_random_num(0, points.size());
            if (numSelected[n] == false) {
                numSelected[n] = true;
                randomPoints.push_back(points[n]);
            }else{
                i--;
                continue;
            }
        }

        //曲線の方程式求める
        vector<float> a(this->DIM_NUM + 1);
        a = this->calcLeastSquaresMethod(randomPoints);

        //インライア数計算
        for (k = 0; k <= DIM_NUM; k++)
            printf("a%d = %10.6f\n", k, a[k]);
        printf("    x    y\n");
        for (px = 0; px < points.size(); px += 1) {
            p = 0;

            int x = points[px].x;
            cout << x << endl;
            for (k = 0; k <= DIM_NUM; k++) {
                p += a[k] * ipow(x, k);
            }
            printf("%5.0f %5.1f ", x, p);
            cv::Point2f ransacPoint(x, p);

            //実際の点との距離を求める
//            cout << "original point: " << points[px] << endl;
//            cout << "ransac point: " << ransacPoint << endl;

            float dist = yagi::calc2PointDistance(points[px], ransacPoint);
            cout << "dist " << dist << endl;
            if (dist < INLIER_RANGE){
                inlierNum++;
            }
        }

        //最適値更新
        if(inlierMaxNum < inlierNum){
            inlierMaxNum = inlierNum;
            bestA = a;
        }

        for (float a : bestA){
            cout << a << endl;
        }

        cout << "max inlier " << inlierMaxNum << endl;
    }


}
