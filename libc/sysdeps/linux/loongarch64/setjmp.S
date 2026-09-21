/* setjmp for LoongArch64.
   Copyright (C) 2024 uClibc-ng contributors.

   Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
*/

#include <sysdep.h>
#include <sys/asm.h>

ENTRY (_setjmp)
	li.w	$a1, 0
	b	HIDDEN_JUMPTARGET (__sigsetjmp)
END (_setjmp)
ENTRY (setjmp)
	li.w	$a1, 1
	/* Fallthrough */
END (setjmp)
ENTRY (__sigsetjmp)
	REG_S	$ra,  $a0, 0*SZREG
	REG_S	$sp,  $a0, 1*SZREG
	REG_S	$r21, $a0, 2*SZREG
	REG_S	$fp,  $a0, 3*SZREG
	REG_S	$s0,  $a0, 4*SZREG
	REG_S	$s1,  $a0, 5*SZREG
	REG_S	$s2,  $a0, 6*SZREG
	REG_S	$s3,  $a0, 7*SZREG
	REG_S	$s4,  $a0, 8*SZREG
	REG_S	$s5,  $a0, 9*SZREG
	REG_S	$s6,  $a0, 10*SZREG
	REG_S	$s7,  $a0, 11*SZREG
	REG_S	$s8,  $a0, 12*SZREG

#ifndef __loongarch_soft_float
	FREG_S	$fs0, $a0, 13*SZREG + 0*SZFREG
	FREG_S	$fs1, $a0, 13*SZREG + 1*SZFREG
	FREG_S	$fs2, $a0, 13*SZREG + 2*SZFREG
	FREG_S	$fs3, $a0, 13*SZREG + 3*SZFREG
	FREG_S	$fs4, $a0, 13*SZREG + 4*SZFREG
	FREG_S	$fs5, $a0, 13*SZREG + 5*SZFREG
	FREG_S	$fs6, $a0, 13*SZREG + 6*SZFREG
	FREG_S	$fs7, $a0, 13*SZREG + 7*SZFREG
#endif

#if !IS_IN_libc && IS_IN_rtld
	/* In ld.so we never save the signal mask.  */
	li.w	$a0, 0
	jr	$ra
#else
	/* Make a tail call to __sigjmp_save; it takes the same args.  */
	b	__sigjmp_save
#endif


END (__sigsetjmp)

hidden_def (__sigsetjmp)
weak_alias (_setjmp, __GI__setjmp)