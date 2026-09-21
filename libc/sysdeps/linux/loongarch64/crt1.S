/* Startup code compliant to the ELF LoongArch64 ABI.
   Copyright (C) 2024 uClibc-ng contributors.

   Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
*/

#define __ASSEMBLY__ 1

#include <sysdep.h>
#include <sys/asm.h>

/* The entry point's job is to call __uClibc_main.  Per the ABI,
   a0 contains the address of a function to be passed to atexit.
   __uClibc_main wants this in a5.  */

.text
	.globl	_start
	.type	_start,%function

_start:
	move   $s0, $a0  /* rtld_fini.  */
#ifdef L_rcrt1
	la.pcrel $a0, __ehdr_start
	bl      reloc_static_pie
#endif
	move   $a5, $s0  /* rtld_fini.  */
	/* main may be in a shared library.  */
	la.got $a0, main
	ld.d   $a1, $sp, 0       /* argc.  */
	addi.d $a2, $sp, SZREG   /* argv.  */
	move   $a3, $zero
	move   $a4, $zero
	bstrins.d $sp, $zero, 3, 0  /* Align stack to 16 bytes.  */
	move   $a6, $sp  /* stack_end.  */

	bl     __uClibc_main

	.size	_start,.-_start

/* Define a symbol for the first piece of initialized data.  */
	.data
	.globl __data_start
__data_start:
	.weak data_start
	data_start = __data_start