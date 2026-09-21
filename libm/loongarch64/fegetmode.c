/* Store current floating-point control modes.  LoongArch64 version.
   Copyright (C) 2024 uClibc-ng contributors.

   Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
*/

#include <fenv.h>
#include <fpu_control.h>

int
fegetmode (femode_t *modep)
{
  _FPU_GETCW (*modep);
  return 0;
}