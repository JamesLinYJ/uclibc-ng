/* Set current rounding direction.
   Copyright (C) 2024 uClibc-ng contributors.

   Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
*/

#include <fenv.h>
#include "fenv_private.h"

int
fesetround (int round)
{
  switch (round)
    {
    case FE_TONEAREST:
    case FE_TOWARDZERO:
    case FE_DOWNWARD:
    case FE_UPWARD:
      loongarch_setround (round);
      return 0;
    default:
      return round; /* A nonzero value.  */
    }
}