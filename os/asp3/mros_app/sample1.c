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
static const char *sample_publish_data = "Hello World!!";
static char subscribe_buffer0[1024];

void timer_interrupt_handler(void)
{
	uint8 is_rcv = 0;
	/*
	 * send can data:CANID 0x100
	 */
	sil_wrb_mem((void*)(VCAN_TX_FLAG(0) + 0), 1);

	/*
	 * read can data:CANID 0x400
	 */
	is_rcv = sil_reb_mem((void*)(VCAN_RX_FLAG(0)));
	if (is_rcv != 0) {
		syslog(LOG_NOTICE, "RECV CAN_DATA: %s", subscribe_buffer0);
		sil_wrb_mem((void*)(VCAN_RX_FLAG(0)), 0);
	}
	
	return;
}

/*
 *  メインタスク
 */
void main_task(intptr_t exinf)
{
	syslog(LOG_NOTICE, "main_task waked up");
	int datalen = strlen(sample_publish_data);

	sil_wrw_mem((void*)(VCAN_TX_DATA_0(0)), (uint32)sample_publish_data);
	sil_wrw_mem((void*)(VCAN_TX_DATA_1(0)), datalen);

	sil_wrw_mem((void*)(VCAN_RX_DATA_0(0)), (uint32)subscribe_buffer0);
	sil_wrw_mem((void*)(VCAN_RX_DATA_1(0)), sizeof(subscribe_buffer0));
	while (1) {
		dly_tsk(1000);
	}
}

void task1(intptr_t exinf)
{
	syslog(LOG_NOTICE, "task1 waked up");
	while (1) {
		dly_tsk(1000);
		timer_interrupt_handler();
	}
}
