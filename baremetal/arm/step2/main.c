
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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



static char buffer[64]; 
int main(void)
{
#if 1
	char *ip = "192.168.11.40";
	char buf[5];
	int result;

	result = sscanf(ip, "%hhu.%hhu.%hhu.%hhu",
		&buf[0],
		&buf[1],
		&buf[2],
		&buf[3]);
#else
	int result;
	result = snprintf(&buffer[0], 1024, "http://%s:%u",
			"192.168.11.40", 11411);
	//result = snprintf(&buffer[0], 64, "%u", 98);
#endif
	while (1) {
	}
	return result;
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
