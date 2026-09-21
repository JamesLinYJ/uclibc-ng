/* Assembly macros for LoongArch64.
   Copyright (C) 2024 uClibc-ng contributors.

   Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
*/

#ifndef _LINUX_LOONGARCH64_SYSDEP_H
#define _LINUX_LOONGARCH64_SYSDEP_H 1

#include <common/sysdep.h>
#include <sys/syscall.h>

#ifdef __ASSEMBLER__

# include <sys/asm.h>

# define ENTRY(name) LEAF(name)

# define L(label) .L ## label

/* Performs a system call, handling errors by setting errno.  Linux indicates
   errors by setting a0 to a value between -1 and -4095.  */
# undef PSEUDO
# define PSEUDO(name, syscall_name, args)			\
  .text;							\
  .align 2;							\
  ENTRY (name);							\
  li.w a7, SYS_ify (syscall_name);				\
  syscall 0;							\
  li.d a7, -4096;						\
  bgtu a0, a7, .Lsyscall_error ## name;

# undef PSEUDO_END
# define PSEUDO_END(sym) 					\
  SYSCALL_ERROR_HANDLER (sym)					\
  jr ra;							\
  END (sym)

# if !IS_IN_libc
#  if defined (__PIC__)
#   define SYSCALL_ERROR_HANDLER(name)				\
.Lsyscall_error ## name:					\
	sub.d a0, zero, a0;					\
	la.tls.ie t1, errno;					\
	add.d t1, t1, tp;					\
	st.w a0, t1, 0;					\
	li.w a0, -1;
#  else
#   define SYSCALL_ERROR_HANDLER(name)				\
.Lsyscall_error ## name:					\
	sub.d a0, zero, a0;					\
	la.tls.ie t1, errno;					\
	add.d t1, t1, tp;					\
	st.w a0, t1, 0;					\
	li.w a0, -1;
#  endif
# else
#  define SYSCALL_ERROR_HANDLER(name)				\
.Lsyscall_error ## name:					\
        b       __syscall_error;
# endif

/* Performs a system call, not setting errno.  */
# undef PSEUDO_NOERRNO
# define PSEUDO_NOERRNO(name, syscall_name, args)	\
  .align 2;						\
  ENTRY (name);						\
  li.w a7, SYS_ify (syscall_name);			\
  syscall 0;

# undef PSEUDO_END_NOERRNO
# define PSEUDO_END_NOERRNO(name)			\
  END (name)

# undef ret_NOERRNO
# define ret_NOERRNO jr ra

/* Performs a system call, returning the error code.  */
# undef PSEUDO_ERRVAL
# define PSEUDO_ERRVAL(name, syscall_name, args) 	\
  PSEUDO_NOERRNO (name, syscall_name, args)		\
  sub.d a0, zero, a0;

# undef PSEUDO_END_ERRVAL
# define PSEUDO_END_ERRVAL(name)			\
  END (name)

# undef ret_ERRVAL
# define ret_ERRVAL jr ra

#endif /* __ASSEMBLER__ */

/* In order to get __set_errno() definition in INLINE_SYSCALL.  */
#ifndef __ASSEMBLER__
# include <errno.h>
#endif

#undef SYS_ify
#define SYS_ify(syscall_name)	__NR_##syscall_name

#ifndef __ASSEMBLER__

/* List of system calls which are supported as vsyscalls.  */
# define HAVE_CLOCK_GETRES_VSYSCALL	1
# define HAVE_CLOCK_GETTIME_VSYSCALL	1
# define HAVE_GETTIMEOFDAY_VSYSCALL	1
# define HAVE_GETCPU_VSYSCALL		1

extern long int __syscall_error (long int neg_errno);

#endif /* ! __ASSEMBLER__ */

#endif /* linux/loongarch64/sysdep.h */