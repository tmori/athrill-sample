
#include "serial.h"

/*
 * RAM BSS
 */
extern unsigned char bss_start;
extern unsigned char bss_end;

/*
 * RAM IDATA
 */
extern unsigned char data_start;
extern unsigned char sdata_start;

/*
 * ROM IDATA
 */
extern unsigned char idata_start;
extern unsigned char idata_end;
extern unsigned char isdata_start;
extern unsigned char isdata_end;

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
	unsigned char *p = &bss_start;
	unsigned char *e = &bss_end;
	for (;p < e; p++) {
		*p = 0;
	}
	return;
}

void data_init(void)
{
	unsigned char *p_rom = &idata_start;
	unsigned char *e_rom = &idata_end;
	unsigned char *p_ram = &data_start;

	for (;p_rom < e_rom; p_ram++, p_rom++) {
		*p_ram = *p_rom;
	}
}
