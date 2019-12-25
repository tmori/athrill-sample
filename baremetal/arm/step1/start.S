
#define UINT_C(val)		(val)		/* uint_t型の定数を作るマクロ */
#define ULONG_C(val)	(val)		/* ulong_t型の定数を作るマクロ */

#include "core_asm.inc"

	ATEXT
	AALIGN(2)
	AGLOBAL(_start)
ALABEL(_start)
    mov r2, #0x20000000
    mov r1, #10
    add r1, r1, #5
    str r1, [r2, #+4]
    ldr r5, [r2, #+4]
	bl    main

