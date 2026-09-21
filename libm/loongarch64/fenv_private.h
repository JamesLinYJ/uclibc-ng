/* Private floating point rounding and exceptions handling.  LoongArch64 version.
   Copyright (C) 2024 uClibc-ng contributors.

   Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
*/

#ifndef LOONGARCH64_FENV_PRIVATE_H
#define LOONGARCH64_FENV_PRIVATE_H 1

#include <fenv.h>
#include <fpu_control.h>
#include "get-rounding-mode.h"

static __always_inline int
loongarch_getround (void)
{
  return get_rounding_mode ();
}

static __always_inline void
loongarch_setround (int rm)
{
  unsigned int fcsr;
  _FPU_GETCW (fcsr);
  fcsr = (fcsr & ~0x300) | (rm & 0x300);
  _FPU_SETCW (fcsr);
}

static __always_inline int
loongarch_getflags (void)
{
  unsigned int fcsr;
  _FPU_GETCW (fcsr);
  return fcsr & FE_ALL_EXCEPT;
}

static __always_inline void
loongarch_setflags (int flags)
{
  unsigned int fcsr;
  _FPU_GETCW (fcsr);
  fcsr = (fcsr & ~FE_ALL_EXCEPT) | (flags & FE_ALL_EXCEPT);
  _FPU_SETCW (fcsr);
}

static __always_inline void
libc_feholdexcept_loongarch (fenv_t *envp)
{
  unsigned int fcsr;
  _FPU_GETCW (fcsr);
  *envp = fcsr;
  fcsr &= ~FE_ALL_EXCEPT;
  _FPU_SETCW (fcsr);
}

#define libc_feholdexcept  libc_feholdexcept_loongarch
#define libc_feholdexceptf libc_feholdexcept_loongarch
#define libc_feholdexceptl libc_feholdexcept_loongarch

static __always_inline void
libc_fesetround_loongarch (int round)
{
  loongarch_setround (round);
}

#define libc_fesetround  libc_fesetround_loongarch
#define libc_fesetroundf libc_fesetround_loongarch
#define libc_fesetroundl libc_fesetround_loongarch

static __always_inline void
libc_feholdexcept_setround_loongarch (fenv_t *envp, int round)
{
  libc_feholdexcept_loongarch (envp);
  libc_fesetround_loongarch (round);
}

#define libc_feholdexcept_setround  libc_feholdexcept_setround_loongarch
#define libc_feholdexcept_setroundf libc_feholdexcept_setround_loongarch
#define libc_feholdexcept_setroundl libc_feholdexcept_setround_loongarch

static __always_inline int
libc_fetestexcept_loongarch (int ex)
{
  return loongarch_getflags () & ex;
}

#define libc_fetestexcept  libc_fetestexcept_loongarch
#define libc_fetestexceptf libc_fetestexcept_loongarch
#define libc_fetestexceptl libc_fetestexcept_loongarch

static __always_inline void
libc_fesetenv_loongarch (const fenv_t *envp)
{
  unsigned int env = (envp != FE_DFL_ENV ? *envp : 0);
  _FPU_SETCW (env);
}

#define libc_fesetenv  libc_fesetenv_loongarch
#define libc_fesetenvf libc_fesetenv_loongarch
#define libc_fesetenvl libc_fesetenv_loongarch
#define libc_feresetround_noex  libc_fesetenv_loongarch
#define libc_feresetround_noexf libc_fesetenv_loongarch
#define libc_feresetround_noexl libc_fesetenv_loongarch

static __always_inline int
libc_feupdateenv_test_loongarch (const fenv_t *envp, int ex)
{
  unsigned int flags = loongarch_getflags ();
  unsigned int env = *envp;
  _FPU_SETCW (env | flags);
  return flags & ex;
}

#define libc_feupdateenv_test  libc_feupdateenv_test_loongarch
#define libc_feupdateenv_testf libc_feupdateenv_test_loongarch
#define libc_feupdateenv_testl libc_feupdateenv_test_loongarch

static __always_inline void
libc_feupdateenv_loongarch (const fenv_t *envp)
{
  unsigned int env = (envp != FE_DFL_ENV ? *envp : 0);
  _FPU_SETCW (env | loongarch_getflags ());
}

#define libc_feupdateenv  libc_feupdateenv_loongarch
#define libc_feupdateenvf libc_feupdateenv_loongarch
#define libc_feupdateenvl libc_feupdateenv_loongarch

static __always_inline void
libc_feholdsetround_loongarch (fenv_t *envp, int round)
{
  unsigned int fcsr;
  _FPU_GETCW (fcsr);
  *envp = fcsr & 0x300;
  fcsr = (fcsr & ~0x300) | (round & 0x300);
  _FPU_SETCW (fcsr);
}

#define libc_feholdsetround  libc_feholdsetround_loongarch
#define libc_feholdsetroundf libc_feholdsetround_loongarch
#define libc_feholdsetroundl libc_feholdsetround_loongarch

static __always_inline void
libc_feresetround_loongarch (fenv_t *envp)
{
  loongarch_setround (*envp);
}

#define libc_feresetround  libc_feresetround_loongarch
#define libc_feresetroundf libc_feresetround_loongarch
#define libc_feresetroundl libc_feresetround_loongarch

#endif