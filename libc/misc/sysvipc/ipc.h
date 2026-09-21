#ifndef IPC_H
#define IPC_H
#include <syscall.h>
#include <bits/kernel-features.h>
#include <bits/wordsize.h>
#ifdef __mips__
#include <sgidefs.h>
#endif

#ifndef __ARCH_HAS_DEPRECATED_SYSCALLS__
#  define __IPC_64	0x0
#elif defined __mips__ && _MIPS_SIM != _ABIO32
/* n32/n64 route the *ctl syscalls through sys_old_*ctl (n32: the compat_
   variants), which call ipc_parse_version() and so strip IPC_64 out of cmd to
   select the layout on every kernel version.  The bit must therefore always be
   set, otherwise the kernel returns the ancient struct and e.g. sem_nsems
   comes back as 0.  */
#  define __IPC_64	0x100
#elif defined __m68k__ || defined __i386__ || defined __mips__ \
   || defined __sh__ || defined __powerpc__ || defined __sparc__ \
   || defined __hppa__
/* A direct semctl/msgctl/shmctl pointing at sys_*ctl sets IPC_64 in the
   kernel and rejects a cmd that carries the bit; without such a number the
   call goes through ipc(), which reads it out of cmd.  arm, xtensa,
   microblaze and alpha stay out: their direct number is sys_old_*ctl, which
   wants the bit either way.  */
# ifdef __NR_semctl
#  define __IPC_64      0x0
# else
#  define __IPC_64      0x100
# endif
#else
# if __WORDSIZE == 32 || defined __alpha__
#  define __IPC_64	0x100
# else
#  define __IPC_64	0x0
# endif
#endif

#ifdef __NR_ipc

/* The actual system call: all functions are multiplexed by this.  */
extern int __syscall_ipc (unsigned int __call, long __first, long __second,
					  long __third, void *__ptr, void *__fifth) attribute_hidden;


/* The codes for the functions to use the multiplexer `__syscall_ipc'.  */
#define IPCOP_semop	 1
#define IPCOP_semget	 2
#define IPCOP_semctl	 3
#define IPCOP_semtimedop 4
#define IPCOP_msgsnd	11
#define IPCOP_msgrcv	12
#define IPCOP_msgget	13
#define IPCOP_msgctl	14
#define IPCOP_shmat	21
#define IPCOP_shmdt	22
#define IPCOP_shmget	23
#define IPCOP_shmctl	24

#endif

#endif							/* IPC_H */
