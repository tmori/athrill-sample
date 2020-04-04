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

using namespace std;

unsigned int athrill_device_func_call __attribute__ ((section(".athrill_device_section")));


typedef struct {
	struct {
		int x;
		int y;
	} pos;
	struct {
		int left;
	} stear;
	struct {
		int left;
	} rpm;
	struct {
		int found;
		int distance;
		int angle;
	} obstacle;
	struct {
		int light;
	} line_sensor;
} SensorInfoType;
static SensorInfoType car_sensor;

/*****mROS user task code*******/

static char str_buf[1024];
#define ARG_NUM	3
typedef struct {
	int argc;
	int argv[ARG_NUM];
} PubDataArgType;
static void create_pub_data(char *ptr, PubDataArgType *argp)
{
	if (argp->argc == 1) {
		sprintf(ptr, "v:%d", argp->argv[0]);
	}
	else {
		sprintf(ptr, "v:%d:%d",  argp->argv[0], argp->argv[1]);
	}
	return;
}

static void topic_publish(ros::Publisher &pub, unsigned int value)
{
	std_msgs::String str;
	PubDataArgType arg;

	arg.argc = 1;
	arg.argv[0] = value;
	create_pub_data(str_buf, &arg);
	str.data = string(str_buf);
	pub.publish(str);
	return;
}

typedef struct {
	ros::Publisher motor;
	ros::Publisher stearing;
	ros::Publisher brake;
} ActuatorType;

static ActuatorType car_actuator;

static int car_mode = 1;
void usr_task1(void)
{
	syslog(LOG_NOTICE,"========Activate user task1========");
	int argc = 0;
	char *argv = NULL;
	int i = 0;
	ros::init(argc,argv,"vehicle_controller");
	ros::NodeHandle n;
	ros::Rate loop_rate(1000);

	car_actuator.motor = n.advertise<std_msgs::String>("control_motor_torque", 1);
	car_actuator.stearing = n.advertise<std_msgs::String>("control_stearing", 1);
	car_actuator.brake = n.advertise<std_msgs::String>("control_brake", 1);

	topic_publish(car_actuator.motor, 30);
	while (1) {
		if (car_mode == 1) {
			if (car_sensor.obstacle.found != 0) {
				syslog(LOG_NOTICE, "FOUND!!");
				topic_publish(car_actuator.brake, 1);
				topic_publish(car_actuator.motor, 0);
				car_mode = 0;
			}
		}
		else {
			if (car_sensor.obstacle.found == 0) {
				syslog(LOG_NOTICE, "NOT FOUND!!");
				topic_publish(car_actuator.motor, 30);
				topic_publish(car_actuator.brake, 0);
				car_mode = 1;
			}
		}

		i++;
		loop_rate.sleep();
	}
}



/*****mROS user task code*******/
void pose_callback(std_msgs::String *msg)
{
	syslog(LOG_NOTICE,"pose_callback():%s", msg->data.c_str());

	sscanf(msg->data.c_str(), "v:%d:%d", &car_sensor.pos.x, &car_sensor.pos.y);
}
void stearing_callback(std_msgs::String *msg)
{
	syslog(LOG_NOTICE,"stearing_callback():%s", msg->data.c_str());
	sscanf(msg->data.c_str(), "v:%d", &car_sensor.stear.left);
}
void speed_callback(std_msgs::String *msg)
{
	syslog(LOG_NOTICE,"speed_callback():%s", msg->data.c_str());
	sscanf(msg->data.c_str(), "v:%d", &car_sensor.rpm.left);
}
void obstacle_callback(std_msgs::String *msg)
{
	syslog(LOG_NOTICE,"obstacle_callback():%s", msg->data.c_str());
	sscanf(msg->data.c_str(), "v:%d:%d", &car_sensor.obstacle.distance, &car_sensor.obstacle.angle);
	if (car_sensor.obstacle.distance > 0) {
		car_sensor.obstacle.found = 1;
		wup_tsk(USR_TASK1);
	}
	else {
		car_sensor.obstacle.found = 0U;
	}
}
void line_sensor_callback(std_msgs::String *msg)
{
	sscanf(msg->data.c_str(), "v:%d", &car_sensor.line_sensor.light);
}

void usr_task2(void)
{
	syslog(LOG_NOTICE,"========Activate user task2========");
	int argc = 0;
	char *argv = NULL;
	ros::init(argc,argv,"vehicle_controller_sensing");
	ros::NodeHandle n;
	ros::Subscriber sub;

	 sub = n.subscribe("pose",1, pose_callback);
	 sub = n.subscribe("stearing",1, stearing_callback);
	 sub = n.subscribe("speed",1, speed_callback);
	 sub = n.subscribe("obstacle",1, obstacle_callback);
	 sub = n.subscribe("line_sensor",1, line_sensor_callback);

	ros::spin();
}
