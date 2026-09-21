/* Clear given exceptions in current floating-point environment.
   Copyright (C) 1997-2026 Free Software Foundation, Inc.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#include "fenv_libc.h"

int
feclearexcept (int excepts)
{
  fenv_union_t u, n;

  /* Get the current state.  */
  u.fenv = fegetenv_register ();

  /* Clear the relevant bits.  */
  n.l = u.l & ~((-(excepts >> (31 - FPSCR_VX) & 1) & FE_ALL_INVALID)
		| (excepts & FPSCR_STICKY_BITS));

  /* Put the new state in effect.  */
  if (u.l != n.l)
    fesetenv_register (n.fenv);

  /* Success.  */
  return 0;
}
