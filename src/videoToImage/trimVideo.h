//
// Created by 八木賢太郎 on 2018/01/03.
//

#ifndef MAINTEST_TRIMVIDEO_H
#define MAINTEST_TRIMVIDEO_H

#include <./opencv2/opencv.hpp>
#include <string.h>

namespace videoToImage {
    void trimVideo(std::string video_name, std::string input_path, std::string output_path, std::string image_list, std::string video_type);
}

#endif //MAINTEST_TRIMVIDEO_H
