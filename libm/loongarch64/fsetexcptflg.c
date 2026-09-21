/* Set floating-point environment exception handling.
   Copyright (C) 2024 uClibc-ng contributors.

   Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
*/

#include <fenv.h>
#include <fpu_control.h>

int
fesetexceptflag (const fexcept_t *flagp, int excepts)
{
  unsigned int fcsr;
  fexcept_t flags = *flagp;
  _FPU_GETCW (fcsr);
  fcsr &= ~(excepts & FE_ALL_EXCEPT);
  fcsr |= (flags & excepts & FE_ALL_EXCEPT);
  _FPU_SETCW (fcsr);
  return 0;
}