//
// Created by yagi on 18/10/17.
//

#ifndef RUNNERSTEPS_MYOPENPOSE_H
#define RUNNERSTEPS_MYOPENPOSE_H

//#include <openpose/flags.hpp>
// OpenPose dependencies
#include <openpose/headers.hpp>

namespace yagi{
    void outputTextFromVideo(const std::string folder_path, const std::string video_path, const std::string output_path);
};
#endif //RUNNERSTEPS_MYOPENPOSE_H
