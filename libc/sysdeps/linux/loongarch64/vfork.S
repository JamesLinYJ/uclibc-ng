/* vfork for Linux, LoongArch64 version.
   Copyright (C) 2024 uClibc-ng contributors.

   Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
*/

/* vfork() is just a special case of clone().  */

#include <sys/asm.h>
#include <sysdep.h>
#define __ASSEMBLY__
#include <linux/sched.h>
#include <asm/signal.h>

	.text
LEAF (__vfork)

	li.w	$a0, (CLONE_VFORK | CLONE_VM | SIGCHLD)
	move	$a1, $sp

	li.w	$a7, __NR_clone
	syscall	0

	blt	$a0, $zero, 1f
	jr	$ra

1:	b	__syscall_error
END (__vfork)
libc_hidden_def(vfork)

weak_alias (__vfork, vfork)