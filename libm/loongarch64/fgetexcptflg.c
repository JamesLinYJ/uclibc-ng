/* Store current representation for exceptions.
   Copyright (C) 2024 uClibc-ng contributors.

   Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
*/

#include <fenv.h>
#include "fenv_private.h"

int
fegetexceptflag (fexcept_t *flagp, int excepts)
{
  *flagp = loongarch_getflags () & excepts;
  return 0;
}