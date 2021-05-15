#pragma once

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/video.hpp"
#include "opencv2/highgui.hpp"

#include <cmath>

#include <iostream>
#include <algorithm>



struct Node
{
	public:
		Node(){}
		Node(cv::RotatedRect rec, int id) : rec(rec), id(id)
			{
				this->rec = rec;
				this->id = id;
			}
	public:
		cv::RotatedRect rec;
		int id;
};


class Helper
{

public:

	int calc_id(cv::RotatedRect& rect);
	//void k_nearest_neighbour(cv::RotatedRect rect);

	double calc_euclidean_distance(cv::RotatedRect p, cv::RotatedRect q);

	void add_RRect_to_last(cv::RotatedRect rect, int id);
	void add_RRect_to_current(cv::RotatedRect rect, int id);

	bool last_frame_empty();
	bool current_frame_empty();

	void overwright();

private:
	
	std::vector<std::shared_ptr<Node> > last_frame_tracked;
	std::vector<std::shared_ptr<Node> > current_frame_tracked;

};

//--------------------
		//	Algorithm
		//	Distance calculation between blobs in 2 consecutive frames
		//	Allocation of unique (!) IDs
		//	Tracking of the blobs utilizing the "nearest neighbour" algorithm -*Tracking:spatial and temporal
		//	
		//	1. save the contours and hierarchy of the last and current frame.
		//	2. before going through the hierarchy to draw the elements:
		//		contours:	detected contours
		//		hierarchy:	containing information about the image topology. It has many
		//					elements as the number of condours
		//	
		//		
		//	RotatedRect: single Ellipse (one Finger) public attribute center.
		//	center.x and center.y return the x and y coordinates. 
		//	
		//	
		//	
		//--------------------
/*
1. select k numbers of the neighbours
2. calc euclidean distance of all data points
3. take the nearest k neighbours
4. declare new point to the calculated group.





*/
void k_nearest_neighbour();

//no grid (kina but...) -> euclidean
double calc_euclidean_distance(cv::RotatedRect p, cv::RotatedRect q);
