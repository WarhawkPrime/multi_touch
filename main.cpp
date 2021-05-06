/* Multitouch Finger Detection Framework v5.0
* for VS2019 & OpenCV 4.11
*
* Bjoern Froemmer, January 2010 - October 2019
*/

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/video.hpp"

//#include "opencv.hpp"

#include <iostream>
#include <time.h>
#include <math.h>
#include <vector>







int main(void)
{
	//VideoCapture cap(0); // use the first camera found on the system
	cv::VideoCapture cap("../mt_camera_raw.AVI");

	if(!cap.isOpened())
	{
		std::cout << "ERROR: Could not open camera / video stream.\n";
		return -1;
	}

	// get the frame size of the videocapture object
	double videoWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	double videoHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

	cv::Mat frame, original, grey;

	int currentFrame = 0; // frame counter
	clock_t ms_start, ms_end, ms_time; // time

	char buffer[10]; // buffer for int to ascii conversion -> itoa(...)


	cv::Mat background;



	for(;;)
	{
		ms_start = clock(); // time start

		cap >> frame; // get a new frame from the videostream

		if(frame.data == NULL) // terminate the program if there are no more frames
		{
			std::cout << "TERMINATION: Camerastream stopped or last frame of video reached.\n";
			break;
		}

		original = frame.clone(); // copy frame to original

		cvtColor(original, grey, cv::COLOR_BGR2GRAY); // convert frame to greyscale image (copies the image in the process!)

		//--------------------------
		// your code goes here
		//--------------------------
		
		//take background
		if(currentFrame == 0)
		{
			background = original;
			cvtColor(background, grey, cv::COLOR_BGR2GRAY);
		}
		
		// background subtraction
		cv::Mat back_sub_res;
		cv::absdiff(background, original, back_sub_res);	// input, input, output.
	
		// simple highpass filter
		cv::Mat blur_res;
		cv::Size size(videoWidth, videoHeight);
		cv::Point point(-1, -1);
		cv::blur(back_sub_res, blur_res, size, point);	//input, output, size, point

		cv::Mat highpass_res;
		cv::absdiff(back_sub_res, blur_res, highpass_res);

		//cv::Mat blur_2_res;
		//cv::blur(highpass_res, blur_2_res, size, point);	//input, output, size, point

		//contrast
		cv::Mat contrast;
		highpass_res.convertTo(contrast, -1, 2, 0);

		
		//thresholding
		cv::Mat thresh_res;
		
		//cvtColor(thresh_res, grey, cv::COLOR_BGR2GRAY);
		thresh_res.convertTo(thresh_res, CV_8UC1);

		if(thresh_res.type() == CV_8UC1) {
			std::cout << "ist cv_8uc1" << std::endl;
		
			if(thresh_res.type() == CV_16UC1)
			{
				std::cout << "cv_16uc1" << std::endl;
			}
		//cv::threshold(back_sub_res, thresh_res, 40, 255, cv::THRESH_BINARY); // src, dest,thresh, max, type
		//cv::adaptiveThreshold(back_sub_res, thresh_res, 255,cv::ADAPTIVE_THRESH_GAUSSIAN_C ,cv::THRESH_BINARY, 13, 1); //src, dst, maxV, adaptMeth, type, threshsize, C
			cv::adaptiveThreshold(back_sub_res, back_sub_res, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 13, 2); //src, dst, maxV, adaptMeth, type, threshsize, C
		}

		// segmentation (binarization)
		//cv::Mat thres_res;
		//cv::threshold(highpass_res, thres_res, 90, 255, cv::THRESH_BINARY);	//input, output, thresh, maxval, type


		//--------------------------
		//
		//--------------------------



		

		if(cv::waitKey(1) == 27) // wait for user input
		{
			std::cout << "TERMINATION: User pressed ESC\n";
			break;
		}

		currentFrame++;

		// time end
		ms_end = clock();
		ms_time = ms_end - ms_start;

		putText(original, "frame #" + (std::string)_itoa(currentFrame, buffer, 10), cv::Point(0, 15), cv::FONT_HERSHEY_PLAIN, 1, CV_RGB(255, 255, 255), 1, 8); // write framecounter to the image (useful for debugging)
		putText(original, "time per frame: " + (std::string)_itoa(ms_time, buffer, 10) + "ms", cv::Point(0, 30), cv::FONT_HERSHEY_PLAIN, 1, CV_RGB(255, 255, 255), 1, 8); // write calculation time per frame to the image

		//imshow("window", original); // render the frame to a window
		cv::imshow("window", original); // render the frame to a window

		cv::imshow("background sub", back_sub_res);
		//cv::imshow("blur", blur_res);
		cv::imshow("highpass", highpass_res);
		//cv::imshow("2nd blur", blur_2_res);
		cv::imshow("thresh", thresh_res);
		cv::imshow("contrast", contrast);

	}

	std::cout << "SUCCESS: Program terminated like expected.\n";
	return 1;
}