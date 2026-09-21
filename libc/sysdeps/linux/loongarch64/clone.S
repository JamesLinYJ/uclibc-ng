/* Wrapper around clone system call.  LoongArch64 version.
   Copyright (C) 2024 uClibc-ng contributors.

   Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
*/

/* clone() is even more special than fork() as it mucks with stacks
   and invokes a function in the right context after its all over.  */

#include <sys/asm.h>
#include <sysdep.h>
#define _ERRNO_H	1
#include <bits/errno.h>

/* int clone(int (*fn)(void *arg), void *child_stack, int flags, void *arg,
	     void *parent_tidptr, void *tls, void *child_tidptr) */

	.text
LEAF (clone)

	/* Align stack to a 16-byte boundary as per LoongArch ABI.  */
	bstrins.d	$a1, $zero, 3, 0

	/* Sanity check arguments.  */
	beqz		$a0, L(invalid)	/* No NULL function pointers.  */
	beqz		$a1, L(invalid)	/* No NULL stack pointers.  */

	addi.d		$a1, $a1, -16	/* Reserve argument save space.  */
	st.d		$a0, $a1, 0	/* Save function pointer.  */
	st.d		$a3, $a1, 8	/* Save argument pointer.  */

	/* The syscall expects the args to be in different slots.  */
	or		$a0, $a2, $zero
	or		$a2, $a4, $zero
	or		$a3, $a6, $zero
	or		$a4, $a5, $zero

	/* Do the system call.  */
	li.w		$a7, __NR_clone
	syscall		0

	blt		$a0, $zero, L(error)
	beqz		$a0, L(thread_start)

	/* Successful return from the parent.  */
	jr		$ra

L(invalid):
	li.w		$a0, -EINVAL
	/* Something bad happened -- no child created.  */
L(error):
	b		__syscall_error
	END (clone)

/* Load up the arguments to the function.  Put this block of code in
   its own function so that we can terminate the stack trace with our
   debug info.  */

ENTRY (__thread_start)
L(thread_start):
	/* Restore the arg for user's function.  */
	ld.d		$t0, $sp, 0	/* Function pointer.  */
	ld.d		$a0, $sp, 8	/* Argument pointer.  */

	/* Call the user's function.  */
	jirl		$ra, $t0, 0

	/* Call exit with the function's return value.  */
	li.w		$a7, __NR_exit
	syscall		0

	END (__thread_start)