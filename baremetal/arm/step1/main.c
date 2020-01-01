
#include "serial.h"

/*
 * RAM BSS
 */
extern unsigned char _bss_start;
extern unsigned char _bss_end;

/*
 * RAM IDATA
 */
extern unsigned char _data_start;
extern unsigned char _sdata_start;

/*
 * ROM IDATA
 */
extern unsigned char _idata_start;
extern unsigned char _idata_end;
extern unsigned char _isdata_start;
extern unsigned char _isdata_end;

extern void bss_clear(void);
extern void data_init(void);

#define STACK_SIZE	128
unsigned char stack_data[STACK_SIZE];


int main(void)
{
	test_print("Hello ARM World!!");
	while (1) {
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
