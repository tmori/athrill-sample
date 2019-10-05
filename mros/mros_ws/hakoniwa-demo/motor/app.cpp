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

#include "actuator_device.h"

using namespace std;

unsigned int athrill_device_func_call __attribute__ ((section(".athrill_device_section")));

/*****mROS user task code*******/

void torque_callback(std_msgs::String *msg)
{
	uint32 data;
	sscanf(msg->data.c_str(), "%u", &data);
	actuator_device_write(DEVICE_ACTUATOR_ADDR_MOTOR_TORQUE, data);
}
void stearing_callback(std_msgs::String *msg)
{
	uint32 data;
	sscanf(msg->data.c_str(), "%u", &data);
	actuator_device_write(DEVICE_ACTUATOR_ADDR_STEARING, data);
}
void brake_callback(std_msgs::String *msg)
{
	uint32 data;
	sscanf(msg->data.c_str(), "%u", &data);
	actuator_device_write(DEVICE_ACTUATOR_ADDR_BRAKE, data);
}

void usr_task1(void)
{
	syslog(LOG_NOTICE,"========Activate user task1========");
	int argc = 0;
	char *argv = NULL;
	ros::init(argc,argv,"vehicle_motor");
	ros::NodeHandle n;
	ros::Subscriber sub;
	ros::Rate loop_rate(1000);

	 sub = n.subscribe("control_motor_torque",1, torque_callback);
	 sub = n.subscribe("control_stearing",1, stearing_callback);
	 sub = n.subscribe("control_brake",1, brake_callback);

	while (1) {
		actuator_device_sync(DEVICE_ACTUATOR_ADDR_SYNC);
		loop_rate.sleep();
	}
}


