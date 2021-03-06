#include "mapping.hpp"


Mapping::Mapping(float maxLaserDistance, int8_t hitOdds, int8_t missOdds)
: kMaxLaserDistance_(maxLaserDistance)
, kHitOdds_(hitOdds)
, kMissOdds_(missOdds)
{
}


void Mapping::updateMap(const sensor_msgs::LaserScan& laser_msg, const geometry_msgs::Pose2D& pose, nav_msgs::OccupancyGrid& map, float offset)
{
    int logtemp;
    //////////////// TODO: Implement your occupancy grid algorithm here ///////////////////////
	float dist, theta;
	float x_t, y_t;//where ray terminates
	geometry_msgs::Point grid_r, grid_t;

	for(int scannum=0; scannum<laser_msg.ranges.size(); scannum++)//determin the line 
	{	
	    pose_r.x = pose.x+offset*cos(pose.theta);//laser scan frame translation 0.23 
	    pose_r.y = pose.y+offset*sin(pose.theta);
	    pose_r.theta = pose.theta;
	    
	    theta = pose_r.theta+laser_msg.angle_min+laser_msg.angle_increment*scannum;
	    
	    if (isnan(laser_msg.ranges[scannum]))
	    {
	    	dist = kMaxLaserDistance_;
	    }
	    else
	    	dist = laser_msg.ranges[scannum];

	    pose_t.x = pose_r.x+dist*cos(theta);
	    pose_t.y = pose_r.y+dist*sin(theta);
	    

	    /*grid index at the original pose and the terminal of the ray*/
	    grid_r = global_position_to_grid_cell(pose_r, map);
	    grid_t = global_position_to_grid_cell(pose_t, map);

	    i0 = (int)grid_r.x;
	    j0 = (int)grid_r.y;
	    it = (int)grid_t.x;
	    jt = (int)grid_t.y;

	    check_range(i0, j0, map);
	    check_range(it, jt, map);

		//printf("%d\t%d\t%d\t%d\n",i0, j0, it, jt);    
/*
	    printf("grid_r: %d %d\n",i0, j0);
	    printf("grid_r: %d %d\n",it, jt);
*/	    
	    if(laser_msg.ranges[scannum] < kMaxLaserDistance_)
	    {
		    logtemp = getlog_odds(it, jt, map);
			if( logtemp <= (100-kHitOdds_))
			{
			    logtemp = logtemp+kHitOdds_;
	    		setlog_odds(it,jt,logtemp,map);
			}
			
			Bresenham(i0, j0, it, jt, map);
			
			logtemp = getlog_odds(it, jt, map);
		    
			if( logtemp <= (100-kHitOdds_))
			{
			    logtemp = logtemp+kHitOdds_;
	    		setlog_odds(it,jt,logtemp,map);	
			}

			
			
	    }
	    else if(laser_msg.ranges[scannum] >= kMaxLaserDistance_)
	    {
			Bresenham(i0, j0, it, jt, map);
	    }
	}
    
}

void Mapping::check_range(int& i, int& j, const nav_msgs::OccupancyGrid& map)
{
	int width = map.info.width;
	int height = map.info.height;

	if (i > width)
		i = width;
	if (j>height)
		j = height;
}

void Mapping::Bresenham(int x0, int y0, int xl, int yl, nav_msgs::OccupancyGrid& map)
{
    int logtemp;
    int deltax, deltay, error, jstep, j;
    bool steep = ifsteep(x0, y0, xl, yl);

    if(steep)
    {
		std::swap(x0,y0);
		std::swap(xl,yl);
    }
    if(x0 > xl)
    {
		std::swap(x0,xl);
		std::swap(y0,yl);
    }
    deltax = xl-x0;
    deltay = abs(yl-y0);
    error = deltax/2;
    j = y0;
    if(y0 < yl)
		jstep = 1;
    else
		jstep = -1;
    for(int i=x0; i<=xl; i++)
    {
		if(steep)
		{
			logtemp = getlog_odds(j,i,map);

		    if( logtemp >= kMissOdds_ )
		    {
			    logtemp = getlog_odds(j,i,map)-kMissOdds_;
	    	    setlog_odds(j,i,logtemp,map);
		    }

		}
		else
		{
			logtemp = getlog_odds(i,j,map);
		    
		    if( logtemp >= kMissOdds_)
		    {
		    	logtemp = getlog_odds(i,j,map)-kMissOdds_;
			    setlog_odds(i,j,logtemp,map);
		    }
			    
		}
		error = error - deltay;
		if(error < 0)
		{
		    j = j+jstep;
		    error = error + deltax;
		}   
    }  
}


bool Mapping::ifsteep(int x0, int y0, int xl, int yl)
{
    if(abs(yl-y0)>abs(xl-x0))
    {
      return true;
    }
    else
      return false;  
}

bool Mapping::setlog_odds(int xi, int yi, int logvalue, nav_msgs::OccupancyGrid& map)
{
	if((xi<=map.info.width)&&(yi<=map.info.height))
	{
		int index = (yi-1)*(map.info.height) + (xi-1);
		map.data[index] = logvalue;
		return true;
	}
	else
		return false;
}

int Mapping::getlog_odds(int xi, int yi, const nav_msgs::OccupancyGrid& map)
{
	if((xi<=map.info.width)&&(yi<=map.info.height))
	{
		int index = (yi-1)*(map.info.height) + (xi-1);
		return map.data[index];
	}
	else
		return -2; //wrong number
}

geometry_msgs::Point Mapping::global_position_to_grid_cell(const geometry_msgs::Pose2D pose, const nav_msgs::OccupancyGrid& map)
{
	geometry_msgs::Point Grid;
	float offsetx = -map.info.origin.position.x;
	float offsety = -map.info.origin.position.y;
    //printf("%f\t%f\t%f\n",pose.x+offsetx,pose.y+offsety,pose.theta);
	Grid.x = floor((pose.x+offsetx)/(map.info.resolution));
	Grid.y = floor((pose.y+offsety)/(map.info.resolution));

	return Grid;
}






