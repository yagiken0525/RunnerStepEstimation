//
// Created by 八木賢太郎 on 2018/01/05.
//

#include<iostream>
#include <fstream>
#include "mymatplot.h"
#include "../basicFunction/basicFunction.h"
#include "matplotlib.h"
#include <assert.h>

using namespace std;

template<typename Point>
float MyMatPlot::calcDistance(Point p1, Point p2) {
    int x_dist = pow((p1.x - p2.x), 2);
    int y_dist = pow((p1.y - p2.y), 2);
    float distance = sqrt((x_dist + y_dist));
    return distance;
}

vector<cv::Point2f> MyMatPlot::get_vector(int i) {
    switch (i) {
        case 0:
            return Nose;
        case 1:
            return Neck;
        case 2:
            return RShoulder;
        case 3:
            return RElbow;
        case 4:
            return RWrist;
        case 5:
            return LShoulder;
        case 6:
            return LElbow;
        case 7:
            return LWrist;
        case 8:
            return RHip;
        case 9:
            return RKnee;
        case 10:
            return RAnkle;
        case 11:
            return LHip;
        case 12:
            return LKnee;
        case 13:
            return LAnkle;
        case 14:
            return REye;
        case 15:
            return LEye;
        case 16:
            return REar;
        case 17:
            return LEar;
        case 18:
            return Bkg;
    }
}

string MyMatPlot::get_name(int i) {
    switch (i) {
        case 0:
            return "Nose";
        case 1:
            return "Neck";
        case 2:
            return "RShoulder";
        case 3:
            return "RElbow";
        case 4:
            return "RWrist";
        case 5:
            return "LShoulder";
        case 6:
            return "LElbow";
        case 7:
            return "LWrist";
        case 8:
            return "RHip";
        case 9:
            return "RKnee";
        case 10:
            return "RAnkle";
        case 11:
            return "LHip";
        case 12:
            return "LKnee";
        case 13:
            return "LAnkle";
        case 14:
            return "REye";
        case 15:
            return "LEye";
        case 16:
            return "REar";
        case 17:
            return "LEar";
        case 18:
            return "Bkg";
    }
}

void MyMatPlot::pushCoord(int i, cv::Point2f pt) {
    switch (i) {
        case 0:
            Nose.push_back(pt);
            break;
        case 1:
            Neck.push_back(pt);
            break;
        case 2:
            RShoulder.push_back(pt);
            break;
        case 3:
            RElbow.push_back(pt);
            break;
        case 4:
            RWrist.push_back(pt);
            break;
        case 5:
            LShoulder.push_back(pt);
            break;
        case 6:
            LElbow.push_back(pt);
            break;
        case 7:
            LWrist.push_back(pt);
            break;
        case 8:
            RHip.push_back(pt);
            break;
        case 9:
            RKnee.push_back(pt);
            break;
        case 10:
            RAnkle.push_back(pt);
            break;
        case 11:
            LHip.push_back(pt);
            break;
        case 12:
            LKnee.push_back(pt);
            break;
        case 13:
            LAnkle.push_back(pt);
            break;
        case 14:
            REye.push_back(pt);
            break;
        case 15:
            LEye.push_back(pt);
            break;
        case 16:
            REar.push_back(pt);
            break;
        case 17:
            LEar.push_back(pt);
            break;
        case 18:
            Bkg.push_back(pt);
            break;
    }
}

void MyMatPlot::plotDistanceTwoPoints(vector<cv::Point2f> pt1_list, vector<cv::Point2f> pt2_list
        , string filename1, string filename2, int humanID) {

    assert(pt1_list.size() == pt2_list.size());
    int point_num = pt1_list.size();

    string filename = "../plotData/" + to_string(humanID) + "/" + filename1 + "_" + filename2;

    //txtで出力
    filename = filename + ".txt";
    ofstream outputfile(filename);
    float pre_dest = 0;
    float grad = 0;
    bool firstframe = true;

    vector<float> x(point_num), y(point_num);
    for (int i = 0; i < point_num; i++) {
//        cout << pt1_list[i] << endl;
        float distance = calcDistance(pt1_list[i], pt2_list[i]);
//        cout << distance << endl;
        if (!firstframe) {
            if (abs(pre_dest - distance) > 50) {
                outputfile << " " << pre_dest << endl;
            } else {
                outputfile << " " << distance << endl;
                pre_dest = distance;
            }
        }else{
            outputfile << " " << distance << endl;
            firstframe = false;
        }
    }

    outputfile.close();

}


void MyMatPlot::plotDistanceToLine( vector<cv::Point2f> pt1_list,
                                    string filename1,
                                    vector<float> a,
                                    vector<float> b,
                                    int humanID) {

    int point_num = pt1_list.size();

    //txtで出力
    string filename = "../plotData/" + to_string(humanID)+ "/distLine" + filename1;
    filename = filename + ".txt";
    ofstream outputfile(filename);

    float pre_dest = 0;
    float grad = 0;
    bool firstframe = true;
    vector<float> x(point_num), y(point_num);
    for (int i = 0; i < point_num; i++) {
        float distance = yagi::distPoint2Line(pt1_list[i], a[i], b[i]);
//        cout << distance << endl;
        if (!firstframe) {
            if (abs(pre_dest - distance) > 50) {
                outputfile << " " << pre_dest << endl;
            } else {
                outputfile << " " << distance << endl;
                pre_dest = distance;
            }
        }else{
            outputfile << " " << distance << endl;
            firstframe = false;
        }
    }

    outputfile.close();
//    cout << "graph save" << endl;
}

void MyMatPlot::estimateFootPlotTiming(vector<cv::Point2f> pt1_list, vector<cv::Point2f> pt2_list
        , string filename1, string filename2, vector<int>* frame_nums, int humanID) {

    cout << " pt1 size " << pt1_list.size() << endl;
    cout << " pt2 size " << pt2_list.size() << endl;
    assert(pt1_list.size() == pt2_list.size());

    int point_num = pt1_list.size();
    vector<float> dist;
    float pre_dest = 0;
    float grad = 0;
    bool firstframe = true;
    vector<float> x(point_num), y(point_num);
    for (int i = 0; i < point_num; i++) {
        float distance = calcDistance(pt1_list[i], pt2_list[i]);
        dist.push_back(distance);
    }

    for (int i = 2;  i < dist.size(); i++){
        cout << "dist" << i << dist[i] << endl;
        //5フレーム間で最小か、最大値をとる時
        if ((((dist[i] > dist[i - 1]) && (dist[i] > dist[i -2])) && ((dist[i] > dist[i + 1]) && (dist[i] > dist[i + 2])))
            || (((dist[i] < dist[i - 1]) && (dist[i] < dist[i -2])) && ((dist[i] < dist[i + 1]) && (dist[i] < dist[i + 2])))){
            frame_nums->push_back(i);

        }
    }

    cout << "Foot print frames selected" << endl;
}




