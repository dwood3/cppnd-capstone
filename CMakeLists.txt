cmake_minimum_required(VERSION 3.11.3)

# set(CMAKE_CXX_STANDARD 17)
project(process_keyframes)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -pthread")

find_package(OpenCV 4.1 REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
add_definitions(${OpenCV_DEFINITIONS})

# Find all executables
file(GLOB project_SRCS src/*.cpp) #src/*.h

#add_library(message_queue SHARED src/message_queue.cpp)
#add_library(process_frame_event SHARED src/process_frame_event.cpp)
#add_library(process_frame_message SHARED src/processed_frame_message.cpp)
#add_library(keyframe_processor SHARED src/keyframe_processor.cpp)

# Add project executable
add_executable(process_keyframes ${project_SRCS})
#target_link_libraries(process_keyframes ${OpenCV_LIBRARIES} message_queue process_frame_event process_frame_message keyframe_processor)
target_link_libraries(process_keyframes ${OpenCV_LIBRARIES})
