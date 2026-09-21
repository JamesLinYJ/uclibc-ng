/* Determine floating-point rounding mode within libc.  LoongArch64 version.
   Copyright (C) 2024 uClibc-ng contributors.

   Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
*/

#ifndef _LOONGARCH64_GET_ROUNDING_MODE_H
#define _LOONGARCH64_GET_ROUNDING_MODE_H

#include <fenv.h>

/* Return the floating-point rounding mode.  */

static inline int
get_rounding_mode (void)
{
  unsigned int fcsr;
  __asm__ volatile ("movfcsr2gr %0, $fcsr0" : "=r" (fcsr));
  return fcsr & 0x300;
}

#endif /* get-rounding-mode.h */