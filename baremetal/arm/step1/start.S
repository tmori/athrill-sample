
#define UINT_C(val)		(val)		/* uint_t型の定数を作るマクロ */
#define ULONG_C(val)	(val)		/* ulong_t型の定数を作るマクロ */

#include "core_asm.inc"

	ATEXT
	AALIGN(2)
	AGLOBAL(_start)
ALABEL(_start)
    mov r2, #9
    add r1, r2, #11
	bl    main

