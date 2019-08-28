//
// Created by Derek Wood on 2019-08-28.
//

#include "process_frame_event.h"

process_frame_event::process_frame_event(int batch_idx_, int frame_idx_, Mat frame_, int dim_, float fps_, double seconds_)
{
    batch_idx = batch_idx_;
    frame_idx = frame_idx_;
    frame = frame_;
    dim = dim_;
    fps = fps_;
    seconds = seconds_;
}
