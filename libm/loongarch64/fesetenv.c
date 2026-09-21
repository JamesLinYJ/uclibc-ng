/* Install given floating-point environment.
   Copyright (C) 2024 uClibc-ng contributors.

   Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
*/

#include <fenv.h>
#include "fenv_private.h"

int
fesetenv (const fenv_t *envp)
{
  libc_fesetenv_loongarch (envp);
  return 0;
}