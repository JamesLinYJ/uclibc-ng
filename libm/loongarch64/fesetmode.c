/* Install given floating-point control modes.  LoongArch64 version.
   Copyright (C) 2024 uClibc-ng contributors.

   Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
*/

#include <fenv.h>
#include <fpu_control.h>

int
fesetmode (const femode_t *modep)
{
  unsigned int fcsr;
  _FPU_GETCW (fcsr);
  /* Keep only exception flags, clear everything else.  */
  fcsr &= FE_ALL_EXCEPT;

  if (modep != FE_DFL_MODE)
    fcsr |= (*modep & ~FE_ALL_EXCEPT);

  _FPU_SETCW (fcsr);
  return 0;
}