#include "Additions.h"



/*
k = 1
thresh = define a threshold of distance to not take a id from another finger/hand

get list with all RotatedRect from (currentFrame - 1) > 0
compare euclidean distance from current RotatedRect from currentFrame with all RotatedRect from the list
take Id of nearest rrect
if not found, create new id (size of rrect list + 1)

*/


/*
calculate distance from current rect to every other rect from last_

*/


int Helper::calc_id(cv::RotatedRect& rect)
{
	//define k and the max distance 
	const int k = 1;	//1 for a finger, 5 for a hand
	int max_dis = 35;


	double distance = DBL_MAX;
	double calc_dist;
	int last_id = -1;


	//there were frames with fingers before
	if(!last_frame_tracked.empty())
	{
		//calc distances to all existing rects and select k rects with the smallest distance
		for(auto& last_rect : last_frame_tracked)
		{
			//find smallest k elements
			calc_dist = calc_euclidean_distance(rect, last_rect.get()->rec);

			if( (calc_dist < distance) && (calc_dist <= max_dis) )
			{
				distance = calc_dist;
				last_id = last_rect.get()->id;
			}
		}
		//out of the for loop because a new finger appears:

		//no existing id found -> create new id. no dublicate with last and current 
		if(last_id == -1)
		{
			//search last and current for dublicate id
			std::vector<int> ids;
			for(auto& last_rect : last_frame_tracked)
			{
				ids.push_back(last_rect.get()->id);
			}
			for(auto& current_rect : current_frame_tracked)
			{
				ids.push_back(current_rect.get()->id);
			}
			
			std::sort(ids.begin(), ids.end());
			
			last_id = 0;
			for(auto& id : ids)
			{
				if(last_id == id)
					last_id++;
			}
			
			add_RRect_to_current(rect, last_id);
			return last_id;
		}
		// id gefunden
		else
		{
			add_RRect_to_current(rect, last_id);
			return last_id;
		}
	}
	//no existing in last_frame -> create new id depending on size of current_frame_tracked - working as intended
	else
	{
		if(current_frame_tracked.empty())
		{
			last_id = 0;
			add_RRect_to_current(rect, last_id);
			return last_id;
		}
		else
		{
			last_id = current_frame_tracked.size();
			add_RRect_to_current(rect, last_id);
			return last_id;
		}
	}


	

	return 0;
}



/*
void Helper::k_nearest_neighbour(cv::RotatedRect rect)
{
	//define k and the max distance 
	const int k = 1;	//1 for a finger, 5 for a hand
	int max_dis = 50;	

	std::array<cv::RotatedRect, k> nn;

	//calc distances to all rects and select k rects with the smallest distance
	for(auto& last_rect : last_frame_tracked)
	{
		double distance = calc_euclidean_distance(rect, last_rect.first);
		std::cout << "distanz: " << distance << std::endl;

		
		if(distance <= max_dis)
		{

			if(nn.empty())
			{
				nn.at(0) = 
			}


		}
		
	}
}
*/

double Helper::calc_euclidean_distance(cv::RotatedRect p, cv::RotatedRect q)
{
	return sqrt( pow((q.center.x - p.center.x),2) + pow((q.center.y - p.center.y),2) );
}

void Helper::add_RRect_to_last(cv::RotatedRect rect, int id)
{
	auto r_node = std::make_shared<Node>(rect, id);
	last_frame_tracked.push_back(r_node);
}




void Helper::add_RRect_to_current(cv::RotatedRect rect, int c_id)
{
	for(auto& rrect : current_frame_tracked)
	{
		if(c_id == rrect.get()->id)
		{
			c_id = current_frame_tracked.size();
		}
	}

	auto r_node = std::make_shared<Node>(rect, c_id);
	current_frame_tracked.push_back(r_node);
}


bool Helper::last_frame_empty()
{
	if(last_frame_tracked.empty())
		return true;
	else
	{
		return false;
	}
}


bool Helper::current_frame_empty()
{

	if(current_frame_tracked.empty())
		return true;
	else
	{
		return false;
	}
}


void Helper::overwright()
{
	last_frame_tracked.clear();
	last_frame_tracked = current_frame_tracked;
	current_frame_tracked.clear();
}

cv::RotatedRect Helper::normalize_rect(cv::RotatedRect rect)
{
	
	rect.center.x /= vwidth;
	rect.center.y /= vhight;
	
	return rect;
}

