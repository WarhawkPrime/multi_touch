#include "Additions.h"



int Helper::calc_id(cv::RotatedRect& rect)
{
	//define k and the max distance 
	const int k = 1;	//1 for a finger, 5 for a hand
	int max_dis = 20;


	double distance = DBL_MAX;
	double calc_dist;
	int current_id = -1;


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
				current_id = last_rect.get()->id;
			}
		}

		//no existing id found -> create new id. no dublicate with last and current 
		if(current_id == -1)
		{
			current_id = this->get_unique_id();
			this->increment_unique_id();

			try
			{
				add_RRect_to_current(rect, current_id);
			}
			catch(const std::exception& e)
			{
				std::cerr << " double id not found : " << current_id << " _ " << e.what();
			}
			
			return current_id;
		}
		// id found
		else
		{
			try
			{
				add_RRect_to_current(rect, current_id);
			}
			catch(const std::exception& e)
			{
				std::cerr << " double id but found: " << current_id << " _ " << e.what();
			}
			return current_id;
		}
	}
	//no existing in last_frame -> create new id and add it to the current_frame
	else
	{

		current_id = this->get_unique_id();
		this->increment_unique_id();

		try
		{
			add_RRect_to_current(rect, current_id);
		}
		catch(const std::exception& e)
		{
			std::cerr << " double id but found: " << current_id << " _ " << e.what();
		}
		return current_id;
	}

	return 0;
}


double Helper::calc_euclidean_distance(cv::RotatedRect p, cv::RotatedRect q)
{
	return sqrt( pow((q.center.x - p.center.x),2) + pow((q.center.y - p.center.y),2) );
}


double Helper::calc_x_velocity(double old_x, double new_x)
{
	
	double s = std::abs(new_x - old_x);
	double t = this->delta_t;	//millisek
	
	if(t == 0)
	{
		return 0;
	}

	return s/t;
}

double Helper::calc_y_velocity(double old_y, double new_y)
{
	double s = std::abs(new_y - old_y);
	double t = this->delta_t;

	if(t == 0)
	{
		return 0;
	}

	return s / t;
}

double Helper::calc_acc(double v_old, double v_new)
{
	

	double t = this->delta_t;
	if(t == 0)
	{
		return 0;
	}

	return std::abs((v_old-v_new))/delta_t;
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
			throw std::bad_typeid();
			c_id = this->get_unique_id();
			this->increment_unique_id();
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
	//check in last frame if there are ids not in current but still "live". 
	//if so, keep them to the next last_frame but decrement live counter 
	//doesn't draw those elements but keeps them alive to skip one frame
	
	//set live
	l_frame_it = last_frame_tracked.begin();

	for( ; l_frame_it != last_frame_tracked.end();)
	{
		
		if(l_frame_it->get()->live == 0)
		{
			l_frame_it = last_frame_tracked.erase(l_frame_it);
		}
		else
		{
			l_frame_it->get()->live --;
			++l_frame_it;
		}
	}

	//now compare id's, doubles are replaced
	if(!current_frame_tracked.empty())
	{
		for(auto& new_node : current_frame_tracked)
		{
			bool duplicate = false;
			for(auto& node : last_frame_tracked)
			{
				if(new_node.get()->id == node.get()->id)
				{
					node = new_node;
					duplicate = true;
				}
			}
			
			if(!duplicate)
			{
				last_frame_tracked.push_back(new_node);
			}
		}
	}

	//last_frame_tracked.clear();
	//last_frame_tracked = current_frame_tracked;
	//this->last_frame_tracked.insert(last_frame_tracked.end(), this->current_frame_tracked.begin(), this->current_frame_tracked.end());
	current_frame_tracked.clear();
}



cv::RotatedRect Helper::normalize_rect(cv::RotatedRect rect)
{
	
	rect.center.x /= vwidth;
	rect.center.y /= vhight;
	
	return rect;
}

void Helper::add_blob(TUIO::TuioCursor* tc)
{
	tuioBlobs.push_back(tc);
}

void Helper::handle_blobs_deletion(TUIO::TuioServer* server)
{
	std::list<TUIO::TuioCursor*> serverTuioBlobs;
	serverTuioBlobs = server->getTuioCursors();

	tuioBlobsIter = tuioBlobs.begin();

	for(; tuioBlobsIter != tuioBlobs.end(); )
	{
		bool live = false;
		for(auto& node : current_frame_tracked)
		{
			if( (*tuioBlobsIter)->getCursorID() == node.get()->id )
			{
				live = true;
			}
		}
		
		if(!live)
		{
			server->removeExternalTuioCursor((*tuioBlobsIter));
			tuioBlobsIter = tuioBlobs.erase(tuioBlobsIter);
		}
		else
		{
			++tuioBlobsIter;
		}
	}
}