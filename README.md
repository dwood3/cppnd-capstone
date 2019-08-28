# cppnd-capstone
C++ Project - Capstone

### Requirements

- opencv

### Overview

This project takes a video as input and performs the following steps:
1) Extracts the keyframes (I-Frames) of the video
2) For each I-Frame, the frame is converted to grayscale.
3) For each grayscale image, the frame is divided into cells of dim x dim dimension (dim is passed in as an argument)
4) For each cell, the median intensity (pixel) value is computed.
5) The results are stored in output.csv where the first column is the keyframe location (number of seconds), following my the pixel intensity values (indexed top to down, left to right).

This code was written to process the keyframes in parallel using shared pointers, classes, and multithreading.
   
This code infrastructure can be re-used on backends to perform video processing algorithms really quickly.

### Getting Started

#### Using Vagrant to manage dependencies
A Vagrantfile is included in case you do not want to worry about installing dependencies.

To build environment with dependencies, type:

``` vagrant up ```

This may take about 30 minutes to install dependencies as OpenCV is installed from source.

Once complete, type:

```vagrant ssh```

#### Building the program

Assuming OpenCV is installed in /usr/local (it is if using the Vagrantfile)

```
mkdir build
cd build
cmake ../
make
```

#### Running the program

You can grab the example .mp4 file I used here:
[videoplayback.mp4](https://cpp-capstone-wood.s3-us-west-1.amazonaws.com/videoplayback.mp4)

videoplayback.mp4 contains the music video of Mark Ronson - Uptown Funk ft. Bruno Mars.

Example:
```
(in the build directory)
./process_keyframes /vagrant/videoplayback.mp4 32
```

For fun, I've uploaded the extracted keyframes to the images/ directory.  Every time the video has a "cut" there should be a keyframe image you can look at.

#### Outputs
The program outputs a file output.csv in the directory that you run the executable.

For each row, the keyframe timestamp in seconds is given, followed by median pixel intensity values for each cell.  The cells are indexed from top down (rows), then across (cols).

#### Assumptions
It is assumed that the video dimensions are such that it can divide evenly into cells of input dimension ```dim```.  If this is not the case, the cells in the final row and final column are not included.

#### Other notes / Credits
I referenced this piece of code to get going with the keyframes quickly [Link](https://stackoverflow.com/questions/42798634/extracting-keyframes-python-opencv)
