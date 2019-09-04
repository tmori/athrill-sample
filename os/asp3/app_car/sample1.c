/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2016 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 * 
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 *  $Id: sample1.c 711 2016-03-29 14:45:54Z ertl-hiro $
 */


#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include "sample1.h"
#include "can.h"
#include "device_io.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

unsigned int athrill_device_func_call __attribute__ ((section(".athrill_device_section")));

/*
 *  割込みハンドラ
 */
#ifdef INTNO1

void intno1_isr(intptr_t exinf)
{
	intno1_clear();
}

#endif /* INTNO1 */

#ifdef CPUEXC1

void
cpuexc_handler(void *p_excinf)
{

}

#endif /* CPUEXC1 */

/*
 *  周期ハンドラ
 *
 *  HIGH_PRIORITY，MID_PRIORITY，LOW_PRIORITY の各優先度のレディキュー
 *  を回転させる．
 */
void cyclic_handler(intptr_t exinf)
{
}

/*
 *  アラームハンドラ
 *
 *  HIGH_PRIORITY，MID_PRIORITY，LOW_PRIORITY の各優先度のレディキュー
 *  を回転させる．
 */
void alarm_handler(intptr_t exinf)
{
}

/*
 *  例外処理タスク
 */
void exc_task(intptr_t exinf)
{
}

typedef struct {
	struct {
		int x;
		int z;
	} pos;
	struct {
		int left;
		int right;
	} stear;
	struct {
		int left;
		int right;
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
#define ARG_NUM	3
typedef struct {
	int argc;
	char *fmt[ARG_NUM];
	int argv[ARG_NUM];
} PubDataArgType;

static void create_pub_data(char *ptr, PubDataArgType *argp)
{
	if (argp->argc == 1) {
		sprintf(ptr, "%s=%u", argp->fmt[0], argp->argv[0]);
	}
	else {
		sprintf(ptr, "%s=%u:%s=%u", argp->fmt[0], argp->argv[0], argp->fmt[1], argp->argv[1]);
	}
	return;
}

static char *parse_sub_data(char *ptr, int *valuep)
{
	char *p = ptr;
	char *vp = NULL;
	//find found =
	while (vp == NULL) {
		switch ((*p)) {
		case '\0':
			return NULL;
		case '=':
			p++;
			vp = p;
			break;
		default:
			p++;
		}
	}

	//find : or \0
	while ((*p) != '\0') {
		if ((*p) == ':') {
			*p = '\0';
			p++;
			break;
		}
		p++;
	}
	*valuep = atoi(vp);
	return p;
}
static void read_sub_data(char *ptr, PubDataArgType *argp)
{
	int i;
	char *p;
	argp->argv[0] = 0;
	argp->argv[1] = 0;
	argp->argv[2] = 0;


	p = ptr;
	for (i = 0; i < argp->argc; i++) {
		p = parse_sub_data(p, &argp->argv[i]);
		if (p == NULL) {
			break;
		}
	}
	return;
}
typedef uint32 mRosPubIdType;
typedef uint32 mRosSubIdType;

typedef struct {
	mRosPubIdType motor;
	mRosPubIdType stearing;
	mRosPubIdType brake;
} ActuatorType;
static ActuatorType car_actuator;


static void mros_topic_set_subscribe_buffer(mRosSubIdType id, char *buffer, uint32 buflen)
{
	sil_wrw_mem((void*)(VCAN_RX_DATA_0(id)), (uint32)buffer);
	sil_wrw_mem((void*)(VCAN_RX_DATA_1(id)), buflen);
	return;
}
static void mros_topic_set_publish_buffer(mRosSubIdType id, char *buffer, uint32 buflen)
{
	sil_wrw_mem((void*)(VCAN_TX_DATA_0(id)), (uint32)buffer);
	sil_wrw_mem((void*)(VCAN_TX_DATA_1(id)), 0);
	return;
}

static int mros_topic_data_is_arrived(mRosSubIdType id)
{
	uint8 is_rcv = 0;
	/*
	 * read data
	 */
	is_rcv = sil_reb_mem((void*)(VCAN_RX_FLAG(id)));
	return is_rcv;
}

static void mros_topic_data_read_done(mRosSubIdType id)
{
	sil_wrb_mem((void*)(VCAN_RX_FLAG(id)), 0);
	return;
}

static void mros_topic_data_publish(mRosPubIdType id, int len)
{
	sil_wrw_mem((void*)(VCAN_TX_DATA_1(id)), len);
	sil_wrb_mem((void*)(VCAN_TX_FLAG(id)), 1);
}

#define MROS_TOPIC_PUB_NUM	3U
#define MROS_TOPIC_SUB_NUM	5U
#define MROS_TOPIC_BUFFER_SIZE	1024

static char mros_topic_pub_buffer[MROS_TOPIC_PUB_NUM][MROS_TOPIC_BUFFER_SIZE];
static char mros_topic_sub_buffer[MROS_TOPIC_SUB_NUM][MROS_TOPIC_BUFFER_SIZE];

static void topic_publish(mRosPubIdType id, int value)
{
	PubDataArgType arg;

	arg.argc = 1;
	arg.fmt[0] = (char*)"value";
	arg.argv[0] = value;
	create_pub_data(mros_topic_pub_buffer[id], &arg);
	syslog(LOG_NOTICE, "topic_id=%u %s", id, mros_topic_pub_buffer[id]);
	mros_topic_data_publish(id, strlen(mros_topic_pub_buffer[id]));
	return;
}

/*
DEBUG_FUNC_MROS_TOPIC_PUB_NUM   3
DEBUG_FUNC_MROS_TOPIC_PUB_0     control_motor_torque
DEBUG_FUNC_MROS_TOPIC_PUB_1     control_stearing
DEBUG_FUNC_MROS_TOPIC_PUB_2     control_brake
DEBUG_FUNC_MROS_TOPIC_SUB_NUM   5
DEBUG_FUNC_MROS_TOPIC_SUB_0     pose
DEBUG_FUNC_MROS_TOPIC_SUB_1     stearing
DEBUG_FUNC_MROS_TOPIC_SUB_2     speed
DEBUG_FUNC_MROS_TOPIC_SUB_3     obstacle
DEBUG_FUNC_MROS_TOPIC_SUB_4     line_sensor   
 */

static int car_mode = 0;
/*
 *  メインタスク
 */
void main_task(intptr_t exinf)
{
	int i;

	syslog(LOG_NOTICE, "main_task waked up");
	car_actuator.motor = 0;
	car_actuator.stearing = 1;
	car_actuator.brake = 2;

	for (i = 0; i < MROS_TOPIC_PUB_NUM; i++) {
		mros_topic_set_publish_buffer(i, mros_topic_pub_buffer[i], MROS_TOPIC_BUFFER_SIZE);
	}
	for (i = 0; i < MROS_TOPIC_SUB_NUM; i++) {
		mros_topic_set_subscribe_buffer(i, mros_topic_sub_buffer[i], MROS_TOPIC_BUFFER_SIZE);
	}

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
		dly_tsk(100);
	}
}

void pose_callback(void)
{
	PubDataArgType arg;
	if (mros_topic_data_is_arrived(0) == 0) {
		return;
	}
	arg.argc = 2;
	read_sub_data(mros_topic_sub_buffer[0], &arg);
	car_sensor.pos.x = arg.argv[0];
	car_sensor.pos.z = arg.argv[1];
	mros_topic_data_read_done(0);

	//syslog(LOG_NOTICE, "/pose x=%d z=%d", arg.argv[0], arg.argv[1]);
}
void stearing_callback(void)
{
	PubDataArgType arg;
	if (mros_topic_data_is_arrived(1) == 0) {
		return;
	}
	arg.argc = 2;
	read_sub_data(mros_topic_sub_buffer[1], &arg);
	car_sensor.stear.left = arg.argv[0];
	car_sensor.stear.right = arg.argv[1];
	mros_topic_data_read_done(1);
	//syslog(LOG_NOTICE, "/stearing left_stear=%d right_stear=%d", arg.argv[0], arg.argv[1]);
}
void speed_callback(void)
{
	PubDataArgType arg;
	if (mros_topic_data_is_arrived(2) == 0) {
		return;
	}
	arg.argc = 2;
	read_sub_data(mros_topic_sub_buffer[2], &arg);
	car_sensor.rpm.left = arg.argv[0];
	car_sensor.rpm.right = arg.argv[1];
	mros_topic_data_read_done(2);
	//syslog(LOG_NOTICE, "/speed left_rpm=%d right_rpm=%d", arg.argv[0], arg.argv[1]);
}
void obstacle_callback(void)
{
	PubDataArgType arg;
	if (mros_topic_data_is_arrived(3) == 0) {
		return;
	}
	arg.argc = 3;
	read_sub_data(mros_topic_sub_buffer[3], &arg);
	car_sensor.obstacle.found = arg.argv[0];
	car_sensor.obstacle.distance = arg.argv[1];
	car_sensor.obstacle.angle = arg.argv[2];
	mros_topic_data_read_done(3);
	//syslog(LOG_NOTICE, "/obstacle found=%d distance=%d angle=%d car_mode=%d", arg.argv[0], arg.argv[1], arg.argv[2], car_mode);
}
void line_sensor_callback(void)
{
	PubDataArgType arg;
	if (mros_topic_data_is_arrived(4) == 0) {
		return;
	}
	arg.argc = 1;
	read_sub_data(mros_topic_sub_buffer[4], &arg);
	car_sensor.line_sensor.light = arg.argv[0];
	mros_topic_data_read_done(4);
	//syslog(LOG_NOTICE, "/speed line_sensor=%d ", arg.argv[0]);
}

void task1(intptr_t exinf)
{
	syslog(LOG_NOTICE, "task1 waked up");
	while (1) {
		dly_tsk(100);

		pose_callback();
		stearing_callback();
		speed_callback();
		obstacle_callback();
		line_sensor_callback();
	}
}
