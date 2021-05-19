/* Multitouch Finger Detection Framework v5.0
* for VS2019 & OpenCV 4.11
*
* Bjoern Froemmer, January 2010 - October 2019
*/

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/video.hpp"
#include "opencv2/highgui.hpp"



#include <map>
#include <unordered_map>
#include "Additions.h"


#include "TuioTime.h"
#include "TuioCursor.h"
#include "TuioServer.h"

//#include "opencv.hpp"

#include <iostream>
#include <time.h>
#include <math.h>
#include <vector>


int main(void)
{

	//creation of a helper object
	Helper helper;

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

	helper.set_hight(videoHeight);
	helper.set_width(videoWidth);
	
	
	cv::Mat frame, original, grey;
	
	int currentFrame = 0; // frame counter
	clock_t ms_start, ms_end, ms_time; // time

	char buffer[10]; // buffer for int to ascii conversion -> itoa(...)


	cv::Mat background;

	
	

	//========== TUIO Server ==========
	TUIO::TuioServer *server = new TUIO::TuioServer();
	
	
	
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

		cv::Mat endresult;

		// background subtraction
		cv::Mat back_sub_res;
		cv::absdiff(background, original, back_sub_res);	// input, input, output.

		// simple highpass filter
		cv::Mat blur_res;
		cv::blur(back_sub_res, blur_res, cv::Size(videoWidth, videoHeight), cv::Point(-1, -1));
		cv::Mat highpass;
		cv::absdiff(back_sub_res, blur_res, highpass);
		
		// threshold
		cv::Mat thresh_res = highpass;

		cv::cvtColor(thresh_res, thresh_res, cv::COLOR_BGR2GRAY);
		cv::threshold(thresh_res, thresh_res, 42, 255, cv::THRESH_TRUNC);
		cv::adaptiveThreshold(thresh_res, thresh_res, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 13, -4); //src, dst, maxV, adaptMeth, type, threshsize, C 
		cv::threshold(thresh_res, thresh_res, 35, 255, cv::THRESH_BINARY);

		endresult = thresh_res;

		
		cv::Mat input = thresh_res;
		//--------------------------
		//	find contours
		//--------------------------
		

		std::vector<std::vector<cv::Point>> contours;
		std::vector<cv::Vec4i> hierarchy;

		cv::findContours(input, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE, cv::Point(-1, -1));



		//iterate through all the top-level contours -> "hierarchy" may not be empty
		if(!hierarchy.empty() )
		{


			/*
			create RotatedRect
			calc id from last_frame
				if empty -> get id dependent from size (or for loop?)
			insert rec with new id to current_frame
			draw ellipses from current_frame
			overwrite last_frame with current_frame
	
			*/


			for(int idx = 0; idx >= 0; idx = hierarchy[idx][0])
			{
				//check contour size (number of points) and area ("blob" size)
				if( cv::contourArea( cv::Mat(contours.at(idx))) > 4 && contours.at(idx).size() > 1)
				{

					cv::RotatedRect rec = cv::fitEllipse(cv::Mat(contours.at(idx)));
					int calc_id = helper.calc_id(rec);
					//std::cout << "test id: " << calc_id << std::endl;
					
					
					cv::ellipse(original, rec, cv::Scalar(0,0,255), 1, 8);
					cv::drawContours(original, contours, idx, cv::Scalar(255,0,0),1,8, hierarchy);

					
					std::string IDs = std::to_string(idx);
					cv::putText(endresult,"... " + std::to_string(calc_id) + " x: " + std::to_string(rec.center.x) + " y: " + std::to_string(rec.center.y), rec.center, cv::FONT_HERSHEY_PLAIN, 1, CV_RGB(255, 255, 255), 1, 8);
					//cv::putText(endresult, "test id: " + std::to_string(calc_id) + "__" + (std::string)_itoa(idx, buffer, 10), rec.center, cv::FONT_HERSHEY_PLAIN, 1, CV_RGB(255, 255, 255), 1, 8);
					//cv::putText(thres_res, idx + (std::string)_itoa(idx, buffer, 10), rec.center, cv::FONT_HERSHEY_PLAIN, 1, CV_RGB(255, 255, 255), 1, 8);
					rec = helper.normalize_rect(rec);
					std::cout << "id: " << calc_id << " x: " << rec.center.x << " y: " << rec.center.y << std::endl;
					
				}
			}
		}
		helper.overwright();


		//--------------------------
		//	end
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

		
		//imshow("window", original); // render the frame to a window
		cv::imshow("window", original); // render the frame to a window
		
		cv::imshow("background sub", back_sub_res);	
		cv::imshow("endresult", endresult);

		//putText(original, "frame #" + (std::string)_itoa(currentFrame, buffer, 10), cv::Point(0, 15), cv::FONT_HERSHEY_PLAIN, 1, CV_RGB(255, 255, 255), 1, 8); // write framecounter to the image (useful for debugging)
		//putText(original, "time per frame: " + (std::string)_itoa(ms_time, buffer, 10) + "ms", cv::Point(0, 30), cv::FONT_HERSHEY_PLAIN, 1, CV_RGB(255, 255, 255), 1, 8); // write calculation time per frame to the image

		
		//========== Danger Zone ==========
		server->initFrame(TUIO::TuioTime::getSessionTime());


		
	}

	std::cout << "SUCCESS: Program terminated like expected.\n";
	return 1;
}