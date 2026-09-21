/* Clear given exceptions in current floating-point environment.
   Copyright (C) 2024 uClibc-ng contributors.

   Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
*/

#include <fenv.h>
#include <fpu_control.h>

int
feclearexcept (int excepts)
{
  unsigned int fcsr;
  _FPU_GETCW (fcsr);
  fcsr &= ~(excepts & FE_ALL_EXCEPT);
  _FPU_SETCW (fcsr);
  return 0;
}