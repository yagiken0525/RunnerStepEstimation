//
// Created by 八木賢太郎 on 2018/01/03.
//

#ifndef MAINTEST_VIDEOTOIMAGE_H
#define MAINTEST_VIDEOTOIMAGE_H

#include <./opencv2/opencv.hpp>
#include <sys/stat.h>
#include <string.h>

namespace videoToImage {
    void videoToImage(std::string video_name,
                      std::string image_save_directory,
                      bool output_list_txt = true,
                      std::string imagelist_name = "/imagelist.txt",
                      std::string image_format = ".jpg");
}

std::string digitString(int num, int digit);

#endif //MAINTEST_VIDEOTOIMAGE_H
