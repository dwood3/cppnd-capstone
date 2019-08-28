//
// Created by Derek Wood on 2019-08-28.
//

#ifndef PROCESS_KEYFRAMES_PROCESSED_FRAME_MESSAGE_H
#define PROCESS_KEYFRAMES_PROCESSED_FRAME_MESSAGE_H

#include <vector>

class processed_frame_message
{
public:
    processed_frame_message(int _batch_idx, int _frame_idx, double _seconds, std::vector<int> _medians);
    int get_batch_idx() { return batch_idx; }
    int get_frame_idx() { return frame_idx; }
    double get_seconds() { return seconds; }
    std::vector<int> get_medians() { return medians; }

private:
    std::vector<int> medians;
    int batch_idx;
    int frame_idx;
    double seconds;

};

#endif //PROCESS_KEYFRAMES_PROCESSED_FRAME_MESSAGE_H
