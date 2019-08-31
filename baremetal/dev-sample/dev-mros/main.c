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

void timer_interrupt_handler(void)
{
	uint8 c;
	int i;
	/*
	 * send can data:CANID 0x100
	 */
	sil_wrb_mem((void*)(VCAN_TX_FLAG(0) + 0), 1);

	/*
	 * read can data:CANID 0x400
	 */
	test_print("RECV CAN_DATA:");
	for (i = 0; i < 8; i++) {
		c = sil_reb_mem((void*)(VCAN_RX_DATA_0(0) + i)));
		test_print_one((const char*)&c);
	}
	c = '\n';
	test_print_one((const char*)&c);
	return;
}


int main(void)
{
	timer_init();

	timer_start(10000U);

	sil_wrb_mem((void*)(VCAN_TX_DATA_0(0) + 0), 'H');
	sil_wrb_mem((void*)(VCAN_TX_DATA_0(0) + 1), 'e');
	sil_wrb_mem((void*)(VCAN_TX_DATA_0(0) + 2), 'l');
	sil_wrb_mem((void*)(VCAN_TX_DATA_0(0) + 3), 'l');
	sil_wrb_mem((void*)(VCAN_TX_DATA_1(0) + 0), 'o');
	sil_wrb_mem((void*)(VCAN_TX_DATA_1(0) + 1), '!');
	sil_wrb_mem((void*)(VCAN_TX_DATA_1(0) + 2), '!');
	sil_wrb_mem((void*)(VCAN_TX_DATA_1(0) + 3), '!');


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

