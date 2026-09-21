/* longjmp, LoongArch64 version.
   Copyright (C) 2024 uClibc-ng contributors.

   Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
*/

#include <sysdep.h>
#include <sys/asm.h>

ENTRY (__longjmp)
	REG_L	$ra,  $a0, 0*SZREG
	REG_L	$sp,  $a0, 1*SZREG
	REG_L	$r21, $a0, 2*SZREG
	REG_L	$fp,  $a0, 3*SZREG
	REG_L	$s0,  $a0, 4*SZREG
	REG_L	$s1,  $a0, 5*SZREG
	REG_L	$s2,  $a0, 6*SZREG
	REG_L	$s3,  $a0, 7*SZREG
	REG_L	$s4,  $a0, 8*SZREG
	REG_L	$s5,  $a0, 9*SZREG
	REG_L	$s6,  $a0, 10*SZREG
	REG_L	$s7,  $a0, 11*SZREG
	REG_L	$s8,  $a0, 12*SZREG

#ifndef __loongarch_soft_float
	FREG_L	$fs0, $a0, 13*SZREG + 0*SZFREG
	FREG_L	$fs1, $a0, 13*SZREG + 1*SZFREG
	FREG_L	$fs2, $a0, 13*SZREG + 2*SZFREG
	FREG_L	$fs3, $a0, 13*SZREG + 3*SZFREG
	FREG_L	$fs4, $a0, 13*SZREG + 4*SZFREG
	FREG_L	$fs5, $a0, 13*SZREG + 5*SZFREG
	FREG_L	$fs6, $a0, 13*SZREG + 6*SZFREG
	FREG_L	$fs7, $a0, 13*SZREG + 7*SZFREG
#endif

	sltui	$a0, $a1, 1
	add.d	$a0, $a0, $a1   # a0 = (a1 == 0) ? 1 : a1
	jr	$ra

END (__longjmp)
libc_hidden_def(__longjmp)