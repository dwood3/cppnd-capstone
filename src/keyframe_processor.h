//
// Created by Derek Wood on 2019-08-28.
//

#ifndef PROCESS_KEYFRAMES_KEYFRAME_PROCESSOR_H
#define PROCESS_KEYFRAMES_KEYFRAME_PROCESSOR_H

#include "message_queue.h"
#include <mutex>
#include "process_frame_event.h"
#include "processed_frame_message.h"
#include <vector>
#include <future>

class keyframe_processor
{
public:
    keyframe_processor(int _id,
            std::shared_ptr<message_queue<process_frame_event>> _eventQueue,
            std::shared_ptr<message_queue<processed_frame_message>> _msgQueue);
    void process();
    void process_event(process_frame_event event);
    bool get_running_state() { return run; }
    void stop() { run = false; }
private:
    int id;
    bool run;
    std::shared_ptr<message_queue<process_frame_event>> eventQueue;
    std::shared_ptr<message_queue<processed_frame_message>> msgQueue;
};

#endif //PROCESS_KEYFRAMES_KEYFRAME_PROCESSOR_H
