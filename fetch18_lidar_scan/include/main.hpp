#ifndef MAIN_HPP
#define MAIN_HPP

#include "ros/ros.h"
#include "iostream"
#include "cmath"
#include "vector"
#include "stdlib.h"
#include "stdio.h"
#include "time.h" 
#include "cstdlib"
#include "pthread.h"
#include <random>

#include "geometry_msgs/Pose2D.h"
#include "nav_msgs/OccupancyGrid.h"
#include "nav_msgs/Odometry.h"
#include "visualization_msgs/Marker.h"
#include "geometry_msgs/Vector3.h"
#include "std_msgs/String.h"
#include "std_msgs/Header.h"
#include "sensor_msgs/LaserScan.h"
#include "tf2_msgs/TFMessage.h"
#include "tf/transform_datatypes.h"
#include "geometry_msgs/Pose2D.h"
#include "nav_msgs/OccupancyGrid.h"
#include "nav_msgs/Odometry.h"
#include "sensor_msgs/Imu.h"

#include "angle_functions.hpp"



#define pi 3.1415926
#define NUM_THREADS 2

struct Particle
{	
	geometry_msgs::Pose2D pose;
	float weight;
};

struct Pose_xyt
{
	std_msgs::Header header;
	geometry_msgs::Pose2D pose;
};





#endif 
