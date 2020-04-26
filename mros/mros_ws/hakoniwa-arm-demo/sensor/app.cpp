#include "app.h"
#include "ros.h"
#include "std_msgs/String.h"

#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <kernel_cfg.h>

#include "sensor_device.h"

using namespace std;

unsigned int athrill_device_func_call __attribute__ ((section(".athrill_device_section")));

/*****mROS user task code*******/

static char str_buf[1024];

static void topic_publish(ros::Publisher &pub, int32 value)
{
	std_msgs::String str;
	sprintf(str_buf, "v:%d", value);
	str.data = string(str_buf);
	pub.publish(str);
	return;
}
static void topic_publish2(ros::Publisher &pub, uint32 value1, int32 value2)
{
	std_msgs::String str;
	sprintf(str_buf, "v:%d:%d", value1, value2);
	str.data = string(str_buf);
	pub.publish(str);
	return;
}

typedef struct {
	ros::Publisher pose;
	ros::Publisher stearing;
	ros::Publisher speed;
	ros::Publisher obstacle;
	ros::Publisher line_sensor;
} SensorType;

static SensorType car_sensor;

void usr_task1(void)
{
	syslog(LOG_NOTICE,"========Activate user task1========");
	int argc = 0;
	char *argv = NULL;
	int32 data1;
	int32 data2;
	ros::init(argc,argv,"vehicle_sensor");
	ros::NodeHandle n;
	ros::Rate loop_rate(1);

	car_sensor.pose = n.advertise<std_msgs::String>("pose", 1);
	car_sensor.stearing = n.advertise<std_msgs::String>("stearing", 1);
	car_sensor.speed = n.advertise<std_msgs::String>("speed", 1);
	car_sensor.obstacle = n.advertise<std_msgs::String>("obstacle", 1);
	car_sensor.line_sensor = n.advertise<std_msgs::String>("line_sensor", 1);

	while (1) {
		//pose
		sensor_device_read(DEVICE_SENSOR_ADDR_POSE_X, (uint32*)&data1);
		sensor_device_read(DEVICE_SENSOR_ADDR_POSE_Y, (uint32*)&data2);
		topic_publish2(car_sensor.pose, data1, data2);

		//stearing
		sensor_device_read(DEVICE_SENSOR_ADDR_STEARING, (uint32*)&data1);
		topic_publish(car_sensor.stearing, data1);

		//speed
		sensor_device_read(DEVICE_SENSOR_ADDR_SPEED, (uint32*)&data1);
		topic_publish(car_sensor.speed, data1);

		//obstacle
		sensor_device_read(DEVICE_SENSOR_ADDR_OBSTACLE_DISTANCE, (uint32*)&data1);
		sensor_device_read(DEVICE_SENSOR_ADDR_OBSTACLE_ANGLE, (uint32*)&data2);
		topic_publish2(car_sensor.obstacle, data1, data2);

		//line_sensor
		sensor_device_read(DEVICE_SENSOR_ADDR_LINE_SENSOR, (uint32*)&data1);
		topic_publish(car_sensor.line_sensor, data1);

		loop_rate.sleep();
	}
}


