
#define UINT_C(val)		(val)		/* uint_t型の定数を作るマクロ */
#define ULONG_C(val)	(val)		/* ulong_t型の定数を作るマクロ */

#include "core_asm.inc"

	ATEXT
	AALIGN(2)
	AGLOBAL(_start)
ALABEL(_start)
	ldr  r0, =stack_data
	add  r0, #128
	mov  sp, r0
	bl bss_clear
	bl data_init

	b    main

