/* Define the machine-dependent type `jmp_buf'.  LoongArch64 version.
   Copyright (C) 2024 uClibc-ng contributors.

   Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
*/

#ifndef _LOONGARCH64_BITS_SETJMP_H
#define _LOONGARCH64_BITS_SETJMP_H

typedef struct __jmp_buf_internal_tag
  {
    /* Return address.  */
    long int __pc;
    /* Stack pointer.  */
    long int __sp;
    /* Reserved register r21.  */
    long int __r21;
    /* Frame pointer.  */
    long int __fp;
    /* Callee-saved registers s0-s8.  */
    long int __regs[9];

    /* Callee-saved floating point registers.  */
#if defined __loongarch_double_float
   double __fpregs[8];
#elif !defined __loongarch_soft_float
# error unsupported FLEN
#endif
  } __jmp_buf[1];

#endif /* _LOONGARCH64_BITS_SETJMP_H */