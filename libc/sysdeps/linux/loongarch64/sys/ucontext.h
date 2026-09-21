/* struct ucontext definition, LoongArch64 version.
   Copyright (C) 2024 uClibc-ng contributors.

   Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
*/

#ifndef _SYS_UCONTEXT_H
#define _SYS_UCONTEXT_H	1

#include <features.h>
#include <signal.h>
#include <bits/sigcontext.h>

#ifdef __USE_MISC
# define NGREG	32

# define LARCH_REG_RA  1
# define LARCH_REG_SP  3
# define LARCH_REG_S0  23
# define LARCH_REG_S1  24
# define LARCH_REG_A0  4
# define LARCH_REG_S2  25
# define LARCH_REG_NARGS 8

typedef unsigned long int greg_t;

/* Container for all general registers.  */
typedef unsigned long int gregset_t[32];
#endif

typedef struct mcontext_t
  {
    unsigned long long int __pc;
    unsigned long long int __gregs[32];
    unsigned int __flags;
    unsigned long long int __extcontext[0]
      __attribute__ ((__aligned__ (16)));
  } mcontext_t;

/* Userlevel context.  */
typedef struct ucontext
  {
    unsigned long int  __uc_flags;
    struct ucontext   *uc_link;
    stack_t            uc_stack;
    sigset_t           uc_sigmask;
    /* Padding to allow sigset_t to be expanded in the future.  */
    char               __glibc_reserved[1024 / 8 - sizeof (sigset_t)];
    mcontext_t uc_mcontext;
  } ucontext_t;

#endif /* sys/ucontext.h */