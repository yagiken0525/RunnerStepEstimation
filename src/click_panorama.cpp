////
//// Created by 八木賢太郎 on 2018/01/12.
////
//#include "panorama.h"
//#include "basicFunction/basicFunction.h"
//#include "calcLiniorEquation.h"
//
//using namespace std;
//using namespace yagi;
//
//
//////コールバック関数(クリックでパノラマ生成)
//cv::Point2f clickPoint;
//vector<cv::Point2f> line;
//bool clicked = false;
//int clicking_flag = 0;
//int line_flag = 0;
//bool line_selected = false;
//
//void PanoramaCallBackFunc(int eventType, int x, int y, int flags, void *userdata) {
//
//    //クリックした点の座標を取得
//    clickPoint.x = x;
//    clickPoint.y = y;
//
//    //Set from image feature points
//    if (eventType == cv::EVENT_LBUTTONUP) {
//        line.push_back(clickPoint);
//        clicking_flag++;
//        clicked = true;
//    }
//}
//
//
////マウス入力用のパラメータ
//struct clickmouseParam {
//    int x;
//    int y;
//    int event;
//    int flags;
//};
//
//void Panorama::clickPanorama() {
//    //クリック用の変数
//    clickmouseParam mouseEvent;
//    cv::namedWindow("image", CV_WINDOW_AUTOSIZE);
//    cv::setMouseCallback("image", PanoramaCallBackFunc, &mouseEvent);
//
//    //特徴点選択
//    for (auto itr = image_info_list.begin(); itr != image_info_list.end(); ++itr) {
//
//        //クリックで特徴点選択
//        //ハードルのマーカーをクリック
//        cv::imshow("image", itr->image);
//        while (1) {
//            int key = cv::waitKey(1);
//            if (key == 'q') {
//                break;
//            }
//
//            //クリックされるたびに呼び出される
//            if (clicked) {
//                const cv::Scalar color(255, 0, 0);
//                cv::circle(itr->image, clickPoint, 2, color, 2);
//                clicked = false;
//            }
//
//            //２回クリックされるたびに呼び出される
//            if (clicking_flag == 2) {
//                const cv::Scalar color(255, 0, 0);
//
//                for (cv::Point2f i : line) {
//                    cv::circle(itr->image, i, 2, color, 2);
//                    cv::imshow("image", itr->image);
//                    clicking_flag = 0;
//                    line_flag++;
//                }
//            }
//
//            //線分2本(4回クリックされたら呼び出される)
//            if (line_flag == 2) {
//
//                cv::imshow("image", itr->image);
//                line_flag = 0;
//
//                //マスク画像生成
//                cv::Mat maskLine = cv::Mat::zeros(itr->image.size(), CV_8U);
////                drawLine(maskLine, line[0], line[1], 5, white);
//
//                //次のフレームのマスク領域決定
//                cout << "mask created" << endl;
//                (itr + 1)->clickMask = maskLine;
//                line_selected = true;
//
//            }
//
//        }
//
//        if (line_selected)
//            break;
//
//    }
//
//
//}
//
//////クリックパノラマ対応点抽出
//////テンプレートマッチングテスト
//void Panorama::templateTest() {
//
//    const cv::Scalar white(255, 255, 255);
//
//    //クリック用の変数
//    clickmouseParam mouseEvent;
//    cv::namedWindow("image", CV_WINDOW_AUTOSIZE);
//    cv::setMouseCallback("image", PanoramaCallBackFunc, &mouseEvent);
//
//    cv::Mat featureArea = cv::Mat::zeros(10, 10, CV_8U);
//
//    //特徴点選択
//    for (auto itr = image_info_list.begin(); itr != image_info_list.end(); ++itr) {
//
//        //クリックで特徴点選択
//        cv::imshow("image", itr->image);
//        while (1) {
//            int key = cv::waitKey(1);
//            if (key == 'q') {
//                break;
//            }
//
//            if (clicking_flag == 1) {
//                const cv::Scalar color(255, 0, 0);
//                cv::circle(itr->image, clickPoint, 2, color, 2);
//                cout << "clicked" << endl;
//                cv::imshow("image", itr->image);
//                clicking_flag = 0;
//
//                for (int row = 0; row < featureArea.rows; row++) {
//                    for (int col = 0; col < featureArea.cols; col++) {
//                        featureArea.at<unsigned char>(row, col) =
//                                itr->binary_image.at<unsigned char>(clickPoint.y - (featureArea.rows / 2) + row,
//                                                                    clickPoint.x - (featureArea.cols / 2) + col);
//                    }
//                }
//
//
//                if (line_flag == 2) {
//                    drawLine(itr->image, line[0], line[1], 3, color);
//                    cv::imshow("image", itr->image);
//                    line_flag = 0;
//
//                    //マスク画像生成
//                    cv::Mat maskLine = cv::Mat::zeros(itr->image.size(), CV_8U);
//                    drawLine(maskLine, line[0], line[1], 5, white);
//
//                    //次のフレームのマスク領域決定
//                    cout << "mask created" << endl;
//                    (itr + 1)->clickMask = maskLine;
//                    line_selected = true;
//                }
//
//            }
//
//        }
//
//        if (line_selected)
//            break;
//
//    }
//
//    //特徴点選択
//    for (int i = 1; i < image_info_list.size() - 1; i++) {
//
//
//        cv::Mat mask = maskAofB(image_info_list[i].trackArea, image_info_list[i].clickMask);
//
//        cv::imshow("gaga", image_info_list[i].trackArea);
//        cv::waitKey(0);
//        cv::imshow("gaga", image_info_list[i].clickMask);
//        cv::waitKey(0);
//
//        cv::Point2f pt1, pt2;
//        vector<cv::Point2f> target_point;
//        for (int i = 0; i < mask.rows; i++) {
//            for (int j = 0; j < mask.cols; j++) {
//                if (mask.at<unsigned char>(j, i) == 255) {
//                    cv::Point2f point(i, j);
//                    target_point.push_back(point);
//                }
//            }
//        }
//        double aaa = 0;
//        double bbb = 0;
//        double a;
//        double b;
//
//
//        pt1.x = 0;
//        pt1.y = b;
//        pt2.x = 500;
//        pt2.y = a * 500 + b;
//
//        cv::Mat insidelineMask = cv::Mat::zeros(mask.size(), CV_8U);
//
//        //cv::imshow("iaa", mask);
//        //cv::waitKey(0);
//        drawLine(insidelineMask, pt1, pt2, 10, white);
//        image_info_list[i + 1].clickMask = insidelineMask;
//
//        cv::imshow("iii", insidelineMask);
//        cv::waitKey(0);
//
//        cv::Mat result_templete;
//
//        cv::matchTemplate(image_info_list[i].gray_image, featureArea, result_templete, CV_TM_CCOEFF);
//        cv::Point maxpt;
//        int max = 0;
//        for (int i = 0; i < insidelineMask.rows; i++) {
//            for (int j = 0; j < insidelineMask.cols; j++) {
//                if (insidelineMask.at<unsigned char>(j, i) == 255) {
//                    int val = result_templete.at<unsigned char>(j, i);
//                    if (max < val) {
//                        max = val;
//                        maxpt.x = j;
//                        maxpt.y = i;
//                    }
//                }
//            }
//        }
//
//
//        cv::rectangle(image_info_list[i].gray_image, maxpt,
//                      cv::Point(maxpt.x + featureArea.cols, maxpt.y + featureArea.rows),
//                      white, 2, 8, 0);
//
//        cv::imshow("featureArea", featureArea);
//        cv::imshow("featureaaa", image_info_list[i].gray_image);
//
//        cv::imshow("template", result_templete);
//        cv::waitKey(0);
//    }
//}
//
////    //Debug///////////////////////////////
////
////    //showing from set points
////    for (int i = 0; i < from_feature_points.size(); i++) {
////        cout << "to_image ID = " << from_feature_points[i].m_imageID << endl;
////        for (int j = 0; j < from_feature_points[i].feature_point.size(); j++) {
////            cout << "to_image FP = " << from_feature_points[i].feature_point[j] << endl;
////        }
////    }
////
////    //showing to set points
////    for (int i = 0; i < to_feature_points.size(); i++) {
////        cout << "from_image ID = " << to_feature_points[i].m_imageID << endl;
////        for (int j = 0; j < to_feature_points[i].feature_point.size(); j++) {
////            cout << "from_image FP = " << to_feature_points[i].feature_point[j] << endl;
////        }
////    }
////
////    //here/////////////////////////////////////
////
////    //making panorama image
////    vector<cv::Mat> panoImages;
////    vector<cv::Mat> Homographys;
////    cv::Mat panorama = image_info_list[0].image;
////
////    for (int i = 0; i < image_info_list.size() - 1; i++) {
////
////        //load images
////        cv::Mat to_image = image_info_list[i].image;
////        cv::Mat from_image = image_info_list[i + 1].image;
////
////        //load feature points
////        vector<cv::Point2f> to_points = from_feature_points[i].feature_point;
////        vector<cv::Point2f> from_points = to_feature_points[i].feature_point;
////
////        //get homography
////        cv::Mat H = cv::findHomography(to_points, from_points, 0);
////        for (auto itr = Homographys.begin(); itr != Homographys.end(); itr++) {
////            H = *itr * H;
////        }
////        Homographys.push_back(H);
////
////        for (int i = 0; i < image_info_list.size(); i++) {
////            cv::Mat to_image = image_info_list[i].image;
////            cv::Mat from_image = image_info_list[i + 1].image;
////            cv::imshow("to", to_image);
////            cv::imshow("from", from_image);
////
////            vector<cv::Point2f> to_points = image_info_list[i].image;
////            vector<cv::Point2f> from_points = image_info_list[i].image;
////            for (auto itr = to_points.begin(); itr != to_points.end(); ++itr) {
////                cout << *itr << endl;
////            }
////            for (auto itr = from_points.begin(); itr != from_points.end(); ++itr) {
////                cout << *itr << endl;
////            }
////
////            cv::Mat H = cv::findHomography(from_points, to_points, 0);
////
////            //warping image
////            cv::Mat result;
////            cv::imshow("to", to_image);
////            cv::imshow("from", from_image);
////            cv::warpPerspective(to_image, result, H,
////                                cv::Size(static_cast<int>(to_image.cols * 1.5),
////                                         static_cast<int>(to_image.rows * 1.1)));
////
////            //making panorama
////            cv::waitKey(0);
////            for (int y = 0; y < result.rows; y++) {
////                for (int x = 0; x < result.cols; x++) {
////                    result.at<cv::Vec3b>(y, x) = from_image.at<cv::Vec3b>(y, x);
////                    cv::warpPerspective(from_image, result, H,
////                                        cv::Size(static_cast<int>(from_image.cols * 1.5),
////                                                 static_cast<int>(from_image.rows * 1.1)));
////                    imshow("result before", result);
////                    cv::waitKey(0);
////                    for (int y = 0; y < from_image.rows; y++) {
////                        for (int x = 0; x < from_image.cols; x++) {
////                            result.at<cv::Vec3b>(y, x) = to_image.at<cv::Vec3b>(y, x);
////                        }
////                    }
////
////                    //show result
////                    imshow("result img", result);
////                    panoImages.push_back(result);
////                    cv::waitKey(0);
////                }
////
////                cv::imshow("pano1", panoImages[0]);
////                cv::imshow("pano2", panoImages[1]);
////
////                //warping image
////                cv::Mat result;
////                cv::warpPerspective(panoImages[1], result, Homographys[0],
////                                    cv::Size(static_cast<int>(panoImages[1].cols * 1.5),
////                                             static_cast<int>(panoImages[1].rows * 1.1)));
////                imshow("result img warp", result);
////                cv::waitKey(0);
////
////                cv::Vec3b color_black(0, 0, 0);
////                for (int y = 0; y < panoImages[0].rows; y++) {
////                    for (int x = 0; x < panoImages[0].cols; x++) {
////                        if (panoImages[0].at<cv::Vec3b>(y, x) != color_black) {
////                            result.at<cv::Vec3b>(y, x) = panoImages[0].at<cv::Vec3b>(y, x);
////                        }
////                    }
////                }
////                imshow("result img mix", result);
////                cv::waitKey(0);
////            }
////        }
////    }
////    //for loop for set points
//////    for (int i = 0; i < from_feature_points.size(); i++) {
//////
//////        //load images
//////        cv::Mat to_image = images[from_feature_points[i].m_imageID];
//////        cv::Mat from_image = images[to_feature_points[i].m_imageID];
//////
//////        //load feature points
//////        vector<cv::Point2f> to_points = from_feature_points[i].feature_point;
//////        vector<cv::Point2f> from_points = to_feature_points[i].feature_point;
//////
//////        //get homography
//////        cv::Mat H = cv::findHomography(to_points, from_points, 0);
//////        for (auto itr = Homographys.begin(); itr != Homographys.end(); itr++) {
//////            H = *itr * H;
//////        }
//////        Homographys.push_back(H);
//////        for (auto itr = panorama_images.begin(); itr != panorama_images.end(); ++itr) {
//////            cv::Mat to_image = images[itr->get_imageID()];
//////            cv::Mat from_image = images[(itr + 1)->get_imageID()];
//////            cv::imshow("to", to_image);
//////            cv::imshow("from", from_image);
//////            vector<cv::Point2f> to_points = itr->get_feature_points();
//////            vector<cv::Point2f> from_points = (itr + 1)->get_feature_points();
//////            for (auto itr = to_points.begin(); itr != to_points.end(); ++itr) {
//////                cout << *itr << endl;
//////            }
//////            for (auto itr = from_points.begin(); itr != from_points.end(); ++itr) {
//////                cout << *itr << endl;
//////            }
//////
//////            cv::Mat H = cv::findHomography(from_points, to_points, 0);
//////
//////            //warping image
//////            cv::Mat result;
//////            cv::imshow("to", to_image);
//////            cv::imshow("from", from_image);
//////            cv::warpPerspective(to_image, result, H,
//////                                cv::Size(static_cast<int>(to_image.cols * 1.5),
//////                                         static_cast<int>(to_image.rows * 1.1)));
//////
//////            //making panorama
//////            cv::waitKey(0);
//////            for (int y = 0; y < result.rows; y++) {
//////                for (int x = 0; x < result.cols; x++) {
//////                    result.at<cv::Vec3b>(y, x) = from_image.at<cv::Vec3b>(y, x);
//////                    cv::warpPerspective(from_image, result, H,
//////                                        cv::Size(static_cast<int>(from_image.cols * 1.5),
//////                                                 static_cast<int>(from_image.rows * 1.1)));
//////                    imshow("result before", result);
//////                    cv::waitKey(0);
//////                    for (int y = 0; y < from_image.rows; y++) {
//////                        for (int x = 0; x < from_image.cols; x++) {
//////                            result.at<cv::Vec3b>(y, x) = to_image.at<cv::Vec3b>(y, x);
//////                        }
//////                    }
//////
//////                    //show result
//////                    imshow("result img", result);
//////                    panoImages.push_back(result);
//////                    cv::waitKey(0);
//////                }
//////
//////                cv::imshow("pano1", panoImages[0]);
//////                cv::imshow("pano2", panoImages[1]);
//////
//////                //warping image
//////                cv::Mat result;
//////                cv::warpPerspective(panoImages[1], result, Homographys[0],
//////                                    cv::Size(static_cast<int>(panoImages[1].cols * 1.5),
//////                                             static_cast<int>(panoImages[1].rows * 1.1)));
//////                imshow("result img warp", result);
//////                cv::waitKey(0);
//////
//////                cv::Vec3b color_black(0, 0, 0);
//////                for (int y = 0; y < panoImages[0].rows; y++) {
//////                    for (int x = 0; x < panoImages[0].cols; x++) {
//////                        if (panoImages[0].at<cv::Vec3b>(y, x) != color_black) {
//////                            result.at<cv::Vec3b>(y, x) = panoImages[0].at<cv::Vec3b>(y, x);
//////                        }
//////                    }
//////                }
//////                imshow("result img mix", result);
//////                cv::waitKey(0);
//////            }
//////        }
//////    }
//////    //warping image
//////    cv::Mat result;
//////    cv::warpPerspective(panoImages[1], result, Homographys[0],
//////                        cv::Size(static_cast<int>(panoImages[1].cols * 1.5), static_cast<int>(panoImages[1].rows * 1.1)));
//////    imshow("result img warp", result);
//////    cv::waitKey(0);
//////
//////    cv::Vec3b color_black(0,0,0);
//////    for (int y = 0; y < panoImages[0].rows; y++) {
//////        for (int x = 0; x < panoImages[0].cols; x++) {
//////            if (panoImages[0].at<cv::Vec3b>(y, x) != color_black) {
//////                result.at<cv::Vec3b>(y, x) = panoImages[0].at<cv::Vec3b>(y, x);
//////            }
//////        }
//////    }
//////    imshow("result img mix", result);
//////    cv::waitKey(0);
////
////    //calculating each Homography
////    for (int i = 0; i < Homographys.size(); i++) {
////        cv::Mat H = Homographys[i];
////        for (int j = 0; j < i; j++) {
////            H *= Homographys[j];
////        }
////        Homographys[i] = H;
////    }
////
////    for (int i = 1; i < panoImages.size(); i++) {
////        panoImages[i] = panoImages[1] * Homographys[i - 1];
////        cv::imshow("warp", panoImages[i]);
////    }
////
////
////}
////
//////    //warping
//////    cv::Mat result;
//////
//////    cv::Mat H = Homographys[1];
//////    cv::warpPerspective(panoImages[1], result, H,
//////                        cv::Size(static_cast<int>(panoImages[1].cols * 1.5),
//////                                 static_cast<int>(panoImages[1].rows * 1.1)));
//////    cv::imshow("resultdesu", result);
//////    cv::imwrite("res.jpg", result);
//////    cv::imshow("base", panoImages[0]);
//////    cv::imwrite("base.jpg", panoImages[0]);
//////    for (int y = 0; y < result.rows; y++) {
//////        for (int x = 0; x < result.cols; x++) {
//////            result.at<cv::Vec3b>(y, x) = panoImages[0].at<cv::Vec3b>(y, x);
//////        }
//////    }
//////    cv::imshow("ressssssult", result);
//////    cv::imwrite("revsvss.jpg", result);
//////    cv::waitKey(0);
//////    for (int i = 0; i < Homos.size(); i++) {
//////        cv::warpPerspective(panoImages[0], panoImages[0], Homos[i],
//////                            cv::Size(static_cast<int>(panoImages[0].cols * 1.5),
//////                                     static_cast<int>(panoImages[0].rows * 1.1)));
//////        cv::waitKey(0);
//////        for (int y = 0; y < panoImages.size(); y++) {
//////            for (int x = 0; x < panoImages[0].cols; x++) {
//////                panoImages[0].at<cv::Vec3b>(y, x) = panoImages[i + 1].at<cv::Vec3b>(y, x);
//////            }
//////        }
//////        imshow("result img", panoImages[0]);
//////        cv::waitKey(0);
//////    }
//////    image_count = 0;
//////    cv::namedWindow("image_pano", CV_WINDOW_AUTOSIZE);
//////    cv::setMouseCallback("image_pano", PanoramaCallBackFunc, &mouseEvent);
//////    for (int i = 0; i < panoImages.size(); i++) {
//////        //クリックで特徴点選択
//////        cv::imshow("image", panoImages[i]);
//////        while (1) {
//////            int key = cv::waitKey(1);
//////            if (key == 'q')
//////                break;
//////        }
//////        image_count++;
//////    }
//////
//////    for (int i = 0; i < from_feature_points.size(); i++) {
//////
//////        cv::Mat to_image = images[from_feature_points[i].m_imageID];
//////        cv::Mat from_image = images[n_featurePoints[i].m_imageID];
//////        vector <cv::Point2f> to_points = from_feature_points[i].feature_point;
//////        vector <cv::Point2f> from_points = n_featurePoints[i].feature_point;
//////        cv::Mat H = cv::findHomography(to_points, from_points, 0);
//////
//////        cv::Mat result;
//////        cv::warpPerspective(to_image, result, H,
//////                            cv::Size(static_cast<int>(to_image.cols * 1.5), static_cast<int>(to_image.rows * 1.1)));
//////        cv::waitKey(0);
//////        for (int y = 0; y < to_image.rows; y++) {
//////            for (int x = 0; x < to_image.cols; x++) {
//////                result.at<cv::Vec3b>(y, x) = from_image.at<cv::Vec3b>(y, x);
//////            }
//////        }
//////        imshow("result img", result);
//////        panoImages.push_back(result);
//////        cv::waitKey(0);
//////    }
////
////
//////    for (auto itr = panorama_images.begin(); itr != panorama_images.end(); ++itr) {
//////        cout << itr->get_imageID() << endl;
//////        cv::Mat to_image = images[itr->get_imageID()];
//////        cout << (itr + 1)->get_imageID() << endl;
//////        cv::Mat from_image = images[(itr + 1)->get_imageID()];
//////        vector<cv::Point2f> to_points = itr->get_feature_points();
//////        cout << "kkkkkk" << endl;
//////        vector<cv::Point2f> from_points = (itr + 1)->get_feature_points();
//////        cv::Mat H = cv::findHomography(to_points, from_points, 0);
//////        cout << "kkkkkk" << endl;
//////
//////        cv::Mat result;
//////        cv::warpPerspective(to_image, result, H,
//////                            cv::Size(static_cast<int>(to_image.cols * 1.5), static_cast<int>(to_image.rows * 1.1)));
//////        cout << "kkkkkk" << endl;
//////        cv::waitKey(0);
//////        for (int y = 0; y < to_image.rows; y++) {
//////            for (int x = 0; x < to_image.cols; x++) {
//////                result.at<cv::Vec3b>(y, x) = from_image.at<cv::Vec3b>(y, x);
//////            }
//////        }
//////        imshow("result img", result);
//////        cv::waitKey(0);
//////
//////    }
////
////
////////コールバック関数(マスクエリア)
//////void Panorama::myCallBackFunc(int eventType, int x, int y, int flags, void* userdata)
//////{
//////    /* mouseParam *ptr = static_cast<mouseParam*> (userdata);
//////
//////    ptr->x = x;
//////    ptr->y = y;
//////    ptr->event = eventType;
//////    ptr->flags = flags;*/
//////
//////    switch (eventType) {
//////    case cv::EVENT_LBUTTONUP:
//////        std::cout << x << " , " << y << std::endl;
//////        cv::Point2i coord;
//////        coord.x = x;
//////        coord.y = y;
//////        rect_coords.push_back(coord);
//////
//////        if (mask_area_flag == 0) {
//////            cout << "upper left corner: " << coord << endl;
//////            mask_area_flag = 1;
//////        }
//////        else {
//////            cout << "lowwer right corner: " << coord << endl;
//////            cv::Rect rect(rect_coords[0], rect_coords[1]);
//////            cv::Point2i center((rect.tl().x + rect.br().x) / 2, (rect.tl().y + rect.br().y) / 2);
//////
//////            Panorama::MaskArea mask_area;
//////            mask_area._mask_area = rect;
//////            mask_area._center = center;
//////            mask_areas.push_back(mask_area);
//////
//////            cout << "New rect generated: " << rect << endl;
//////            rect_coords.clear();
//////            mask_area_flag = 0;
//////        }
//////
//////    }
//////}
////
//////static void callBackFunc(int eventType, int x, int y, int flags, void* userdata)
//////{
//////    Panorama* panorama = static_cast<Panorama*>(userdata);
//////    panorama->myCallBackFunc(eventType, x, y, flags, nullptr );
//////}
////
////
////
//////コールバック関数(hsvcolor)
////void ClickTrackingCallBackFunc(int eventType, int x, int y, int flags, void *userdata) {
////    mouseParam *ptr = static_cast<mouseParam *> (userdata);
////
////    ptr->x = x;
////    ptr->y = y;
////    ptr->event = eventType;
////    ptr->flags = flags;
////
////    switch (ptr->event) {
////        case cv::EVENT_LBUTTONUP:
////            std::cout << ptr->x << " , " << ptr->y << std::endl;
////
////    }
////}
//////
////void Panorama::featurePointTracking() {
////    //クリック用の変数
////    mouseParam mouseEvent;
////    cv::namedWindow("image", CV_WINDOW_AUTOSIZE);
////    cv::setMouseCallback("image", myCallBackFunc, &mouseEvent);
////    cout << "Click with Ctrl key => to_image feature points" << endl;
////    cout << "Click  => from_image feature points" << endl;
////
////    //クリックで特徴点選択
////    cv::Mat image = image_info_list[0].image;
////    cv::imshow("image", image);
////    while (1) {
////        int key = cv::waitKey(1);
////        if (key == 'q') {
////            break;
////        }
////    }
////}
//
//
////
////
////
////
////
////
////
////