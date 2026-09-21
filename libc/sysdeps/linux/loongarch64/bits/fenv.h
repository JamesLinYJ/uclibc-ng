/* Floating point environment, LoongArch64 version.
   Copyright (C) 2024 uClibc-ng contributors.

   Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
*/

#ifndef _FENV_H
# error "Never use <bits/fenv.h> directly; include <fenv.h> instead."
#endif

enum
  {
    FE_INEXACT   =
#define FE_INEXACT	(0x010000)
      FE_INEXACT,
    FE_UNDERFLOW =
#define FE_UNDERFLOW	(0x020000)
      FE_UNDERFLOW,
    FE_OVERFLOW  =
#define FE_OVERFLOW	(0x040000)
      FE_OVERFLOW,
    FE_DIVBYZERO =
#define FE_DIVBYZERO	(0x080000)
      FE_DIVBYZERO,
    FE_INVALID   =
#define FE_INVALID	(0x100000)
      FE_INVALID
  };

#define FE_ALL_EXCEPT \
	(FE_INEXACT | FE_DIVBYZERO | FE_UNDERFLOW | FE_OVERFLOW | FE_INVALID)

enum
  {
    FE_TONEAREST  =
#define FE_TONEAREST	(0x000)
      FE_TONEAREST,
    FE_TOWARDZERO =
#define FE_TOWARDZERO	(0x100)
      FE_TOWARDZERO,
    FE_UPWARD     =
#define FE_UPWARD	(0x200)
      FE_UPWARD,
    FE_DOWNWARD   =
#define FE_DOWNWARD	(0x300)
      FE_DOWNWARD
  };


typedef unsigned int fexcept_t;
typedef unsigned int fenv_t;

/* If the default argument is used we use this value.  */
#define FE_DFL_ENV	((__const fenv_t *) -1)

/* Type representing floating-point control modes.  */
typedef unsigned int femode_t;

/* Default floating-point control modes.  */
# define FE_DFL_MODE	((const femode_t *) -1L)