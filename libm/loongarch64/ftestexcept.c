/* Test exception in current environment.
   Copyright (C) 2024 uClibc-ng contributors.

   Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
*/

#include <fenv.h>
#include "fenv_private.h"

int
fetestexcept (int excepts)
{
  return libc_fetestexcept_loongarch (excepts);
}