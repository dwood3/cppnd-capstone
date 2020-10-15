
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <future>
#include "message_queue.h"
#include "process_frame_event.h"
#include "processed_frame_message.h"
#include "keyframe_processor.h"
#include <opencv2/opencv.hpp>
#include <thread>

std::string exec(const char *cmd)
{
	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
	if (!pipe)
	{
		throw std::runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
	{
		result += buffer.data();
	}
	return result;
}

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		std::cout << "Usage: ./keyframe_processor filename dim" << std::endl;
		return -1;
	}

	std::string filename = argv[1];

	cv::VideoCapture cap(filename);
	// if not success, exit program
	if (cap.isOpened() == false)
	{
		std::cout << "Cannot open the video file" << std::endl;
		std::cin.get(); //wait for any key press
		return -1;
	}

	int dim = atoi(argv[2]);

	//get frame resolution
	int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
	int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
	std::cout << "Video Resolution: " << frame_width << " x " << frame_height << std::endl;

	if ((dim <= 0) || (dim > frame_width) || (dim > frame_height) 
		|| (frame_width % dim != 0) || (frame_height % dim != 0))
	{
		std::cout << "Dim must be greater than 0, less than video resolution, and ";
		std::cout << "evenly divisible by resolution" << std::endl;
		return -1;
	}

	// Extract keyframes from video
	std::cout << "Extracting keyframes from " << filename << "..." << std::endl;
	std::string command = "python3 ../src/extract_keyframes.py " + filename;
	exec(command.c_str());

	// Read in keyframe indices
	std::ifstream inputFile("keyframes.txt");
	int size = 0;
	int keyframes_size = 0;
	inputFile >> keyframes_size;
	std::vector<int> keyframes;
	keyframes.resize(keyframes_size);
	int counter = 0;

	if (inputFile)
	{
		int value;

		// read the elements in the file into a vector
		while (inputFile >> value)
		{
			keyframes[counter++] = value;
		}
	}

	std::cout << "Found " << keyframes.size() << " keyframes." << std::endl;

	//get the frame rate of the video
	double fps = cap.get(CAP_PROP_FPS);

	std::shared_ptr<message_queue<process_frame_event>> _eventQueue = std::make_shared<message_queue<process_frame_event>>();
	std::shared_ptr<message_queue<processed_frame_message>> _msgQueue = std::make_shared<message_queue<processed_frame_message>>();

	int num_threads = 4;
	std::vector<keyframe_processor> kfps;
	std::vector<std::thread> kfp_threads;
	std::cout << "Launching keyframe processors... ";
	for (int i = 0; i < num_threads; i++)
	{
		keyframe_processor kfp = keyframe_processor(i, _eventQueue, _msgQueue);
		kfps.push_back(kfp);
		kfp_threads.push_back(std::thread(&keyframe_processor::process, kfp));
	}
	std::cout << "done." << std::endl;

	for (int i = 0; i < keyframes.size(); i++)
	{
		if (i % 10 == 0)
		{
			std::cout << "\rProcessing keyframe " << i << " / " << keyframes.size() << " ("
					  << (float(i) / keyframes.size()) * 100 << "%)" << std::flush;
		}
		cap.set(cv::CAP_PROP_POS_FRAMES, keyframes[i]);

		cv::Mat frame;
		bool bSuccess = cap.read(frame); // read a new frame from video

		//Breaking the for loop early at the end of the video
		if (bSuccess == false)
		{
			std::cout << "Found the (unexpected) end of the video" << std::endl;
			break;
		}

		int frame_idx = keyframes[i];
		double seconds = (1.0 / fps) * frame_idx;

		process_frame_event event = process_frame_event(i, frame_idx, frame, dim, fps, seconds);
		auto sentCondition = std::async(std::launch::async, &message_queue<process_frame_event>::send, _eventQueue, std::move(event));
	}
	std::cout << "\rProcessing keyframe " << keyframes.size() << " / " << keyframes.size() << " ("
			  << "100%) done." << std::endl;

	// Open file to prepare for writing
	std::ofstream outfile;
	outfile.open("output.csv", std::ios::out | std::ios::trunc);

	// Wait for processed messages to return
	int processed_frame_counter = 0;
	while (processed_frame_counter < keyframes.size())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		processed_frame_message message = _msgQueue->receive();

		// Write message to line in .csv file
		outfile << message.get_seconds();
		std::vector<int> medians = message.get_medians();
		for (int i = 0; i < medians.size(); i++)
			outfile << ", " << medians[i];
		outfile << std::endl;

		processed_frame_counter++;
		if (processed_frame_counter % 10 == 0)
		{
			std::cout << "\rProcessed keyframes: " << processed_frame_counter << " / " << keyframes.size()
					  << " (" << (float(processed_frame_counter) / keyframes.size()) * 100 << "%)" << std::flush;
		}
	}
	std::cout << "\rProcessed keyframes: " << keyframes.size() << " / " << keyframes.size()
			  << " (100%) done." << std::endl;

	outfile.close();

	// Stop frame processors by sending empty message
	//for(int i=0;i<num_threads;i++) {
	//    kfps[i].stop();
	//}

	return 0;
}
