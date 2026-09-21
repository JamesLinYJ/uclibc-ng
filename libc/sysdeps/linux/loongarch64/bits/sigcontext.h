/* Machine-dependent signal context structure for Linux.  LoongArch64 version.
   Copyright (C) 2024 uClibc-ng contributors.

   Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
*/

#ifndef _BITS_SIGCONTEXT_H
#define _BITS_SIGCONTEXT_H 1

#if !defined _SIGNAL_H && !defined _SYS_UCONTEXT_H
# error "Never use <bits/sigcontext.h> directly; include <signal.h> instead."
#endif

struct sigcontext {
  unsigned long long int sc_pc;
  unsigned long long int sc_regs[32];
  unsigned int sc_flags;
  unsigned long long int sc_extcontext[0]
    __attribute__ ((__aligned__ (16)));
};

#endif