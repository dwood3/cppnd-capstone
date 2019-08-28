#include "keyframe_processor.h"

keyframe_processor::keyframe_processor(int _id,
        std::shared_ptr<message_queue<process_frame_event>> _eventQueue,
        std::shared_ptr<message_queue<processed_frame_message>> _msgQueue)
{
    id = _id;
    eventQueue = _eventQueue;
    msgQueue = _msgQueue;
    run = true;
}

void keyframe_processor::process_event(process_frame_event event)
{
    // Convert to grey'
    Mat grey_frame;
    cvtColor(event.get_frame(), grey_frame, cv::COLOR_RGB2GRAY);

    // Drop last row and col cell if any
    int dim = event.get_dim();
    Mat frame = event.get_frame();
    int num_col_cells = frame.cols / dim;
    int num_row_cells = frame.rows / dim;

    std::vector<int> medians(num_col_cells*num_row_cells);
    int median_counter = 0;

    for(int j=0; j<num_col_cells; j++)
    {
        Range colRange = Range(j*dim,(j+1)*dim);
        for(int k=0; k<num_row_cells; k++)
        {
            Range rowRange = Range(k*dim,(k+1)*dim);
            Mat subFrame = grey_frame(rowRange,colRange);

            std::vector<int> pixels(dim*dim);
            uint64 pixel_counter = 0;

            for(int l=0; l<dim; l++)
            {
                for(int m=0; m<dim; m++)
                {
                    Scalar intensity = subFrame.at<uchar>(l,m);
                    pixels[pixel_counter++] = intensity.val[0];
                }
            }

            sort(pixels.begin(), pixels.end());

            int median_index = (pixels.size() / 2) + 1;
            int median = pixels[median_index];
            medians[median_counter++] = median;
        }
    }

    processed_frame_message msg(event.get_batch_idx(), event.get_frame_idx(), event.get_seconds(), medians);
    auto sentCondition = std::async(std::launch::async, &message_queue<processed_frame_message>::send, msgQueue, std::move(msg));
}

void keyframe_processor::process()
{
    while (run)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        process_frame_event event = eventQueue->receive();
        process_event(event);
    }
    std::cout << "Stopping keyframe_processor ID: " << id << std::endl;
}
