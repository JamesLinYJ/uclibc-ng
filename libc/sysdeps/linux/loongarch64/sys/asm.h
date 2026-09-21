/* Miscellaneous macros for LoongArch64.
   Copyright (C) 2024 uClibc-ng contributors.

   Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
*/

#ifndef _SYS_ASM_H
#define _SYS_ASM_H

#define SZREG	8
#define SZFREG	8
#define REG_S	st.d
#define REG_L	ld.d
#define FREG_S	fst.d
#define FREG_L	fld.d
#define PTRLOG	3

/* Declare leaf routine.  */
#define	LEAF(symbol)				\
		.globl	symbol;			\
		.align	2;			\
		.type	symbol,@function;	\
symbol:						\
		cfi_startproc;

/* Mark end of function.  */
#undef END
#define END(function)				\
		cfi_endproc;			\
		.size	function,.-function

/* Stack alignment -- LoongArch requires 16-byte alignment.  */
#define ALMASK	~15

#endif /* sys/asm.h */