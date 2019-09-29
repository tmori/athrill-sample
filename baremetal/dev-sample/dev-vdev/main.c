#include "test_serial.h"
#include "test_reg.h"
#include "section.h"
#include "interrupt.h"
#include <string.h>
#include "timer.h"
#include "v850_ins.h"
#include <stdlib.h>
#include "vdev.h"
#include "device_io.h"

unsigned char stack_data[STACK_SIZE] __attribute__ ((section(".bss_noclr")));


void timer_interrupt_handler(void)
{
	uint32 data;
	uint8 *array = (uint8*)&data;
	array[0] = 'H';
	array[1] = 'W';
	array[2] = 'W';
	array[3] = '\0';
	/*
	 * send can data:CANID 0x100
	 */
	//sil_wrw_mem((void*)VDEV_TX_DATA_BASE, data);
	//sil_wrb_mem((void*)VDEV_TX_FLAG_BASE, 1);
	/*
	 * read can data:CANID 0x400
	 */
	
	data = sil_rew_mem((void*)VDEV_RX_DATA_BASE);
	test_print("START\n");
	test_print_one((const char*)&array[0]);
	test_print_one((const char*)&array[1]);
	test_print_one((const char*)&array[2]);
	test_print_one((const char*)&array[3]);
	test_print("END\n");
	return;
}


int main(void)
{
	timer_init();

	timer_start(10000U);

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

