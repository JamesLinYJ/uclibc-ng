/* Install given floating-point environment.
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
#include <fpu_control.h>

int
fesetenv (const fenv_t *envp)
{
  fenv_union_t old, new;

  /* get the currently set exceptions.  */
  new.fenv = *envp;
  old.fenv = fegetenv_control ();

  __TEST_AND_EXIT_NON_STOP (old.l, new.l);
  __TEST_AND_ENTER_NON_STOP (old.l, new.l);

  fesetenv_register (new.fenv);

  /* Success.  */
  return 0;
}
