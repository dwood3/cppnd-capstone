//
// Created by Derek Wood on 2019-08-28.
//

#include "processed_frame_message.h"

processed_frame_message::processed_frame_message(int _batch_idx, int _frame_idx, double _seconds, std::vector<int> _medians)
{
    batch_idx = _batch_idx;
    frame_idx = _frame_idx;
    seconds = _seconds;
    medians = _medians;
}
