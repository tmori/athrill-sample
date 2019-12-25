
#define STACK_SIZE	1024
unsigned char stack_data[STACK_SIZE] __attribute__ ((section(".bss_noclr")));


int main(void)
{
	stack_data[0] = 1;
	while (1) {
	}
}


