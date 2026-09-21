/* Store current floating-point environment and clear exceptions.
   Copyright (C) 2024 uClibc-ng contributors.

   Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
*/

#include <fenv.h>
#include "fenv_private.h"

int
feholdexcept (fenv_t *envp)
{
  libc_feholdexcept_loongarch (envp);
  return 0;
}