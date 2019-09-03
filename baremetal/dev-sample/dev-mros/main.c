#include "test_serial.h"
#include "test_reg.h"
#include "section.h"
#include "interrupt.h"
#include <string.h>
#include "timer.h"
#include "v850_ins.h"
#include <stdlib.h>
#include "can.h"
#include "device_io.h"

unsigned char stack_data[STACK_SIZE] __attribute__ ((section(".bss_noclr")));

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
		test_print("RECV CAN_DATA:");
		test_print(subscribe_buffer0);
		test_print_one("\n");
		sil_wrb_mem((void*)(VCAN_RX_FLAG(0)), 0);
	}
	
	return;
}


int main(void)
{
	int datalen = strlen(sample_publish_data);
	timer_init();

	timer_start(10000U);

	sil_wrw_mem((void*)(VCAN_TX_DATA_0(0)), (uint32)sample_publish_data);
	sil_wrw_mem((void*)(VCAN_TX_DATA_1(0)), datalen);

	sil_wrw_mem((void*)(VCAN_RX_DATA_0(0)), (uint32)subscribe_buffer0);
	sil_wrw_mem((void*)(VCAN_RX_DATA_1(0)), sizeof(subscribe_buffer0));


	while (1) {
		do_idle();
	}
}

void bss_clear(void)
{
	unsigned char *p = &_bss_start;
	unsigned char *e = &_bss_end;
	for (;p < e; p++) {
		*p = 0;
	}
	return;
}

void data_init(void)
{
	unsigned char *p_rom = &_idata_start;
	unsigned char *e_rom = &_idata_end;
	unsigned char *p_ram = &_data_start;

	for (;p_rom < e_rom; p_ram++, p_rom++) {
		*p_ram = *p_rom;
	}
}

