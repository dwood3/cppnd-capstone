//
// Created by Derek Wood on 2019-08-26.
//

#ifndef PROCESS_KEYFRAMES_PROCESS_FRAME_EVENT_H
#define PROCESS_KEYFRAMES_PROCESS_FRAME_EVENT_H

#include <opencv2/opencv.hpp>
#include "processed_frame_message.h"

using namespace cv;

class process_frame_event
{
public:
    process_frame_event(int batch_idx, int frame_idx, Mat frame, int dim, float fps, double seconds);
    int get_batch_idx(){ return batch_idx; }
    int get_frame_idx(){ return frame_idx; }
    int get_dim(){ return dim; }
    float get_fps(){ return fps; }
    double get_seconds(){ return seconds; }
    Mat get_frame() { return frame; }

private:
    int batch_idx;
    int frame_idx;
    Mat frame;
    int dim;
    float fps;
    double seconds;
};

#endif //PROCESS_KEYFRAMES_PROCESS_FRAME_EVENT_H
