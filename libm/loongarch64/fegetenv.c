/* Store current floating-point environment.
   Copyright (C) 2024 uClibc-ng contributors.

   Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
*/

#include <fenv.h>
#include <fpu_control.h>

int
fegetenv (fenv_t *envp)
{
  _FPU_GETCW (*envp);
  return 0;
}