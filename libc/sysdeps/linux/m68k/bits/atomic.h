/* Atomic operations.  m68k version.
   Copyright (C) 2026 Ramin Moussavi <ramin.moussavi@yacoub.de>

   Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.

   The CAS instruction arrived with the 68020.  Where it exists we implement
   the compare-and-exchange primitive with cas.b/cas.w/cas.l and let
   include/atomic.h derive every higher-level operation from it.

   ColdFire, 68000 and 68010 have no atomic read-modify-write instruction at
   all.  There we use the kernel's atomic_cmpxchg_32 helper, which exists for
   exactly this reason and is available on every m68k (syscall 335, "common"
   in arch/m68k/kernel/syscalls/syscall.tbl, with an implementation for both
   the MMU and the noMMU port).

   Without this file m68k fell back to the generic non-atomic
   bits/atomic.h, whose "atomic" compare-and-exchange is a plain
   read-compare-write.  Under sustained contention a preemption between
   the read and the write corrupts NPTL's lock and condition-variable
   state, which showed up as lost wakeups (tst-cond16 hung until the test
   timeout).  */

#ifndef _M68K_BITS_ATOMIC_H
#define _M68K_BITS_ATOMIC_H	1

#include <stdint.h>

/* CAS came with the 68020.  gcc sets a separate macro per core and does not
   imply __mc68020__ on the later ones, hence the list; __mc68000__ is no help
   here, it is defined on every m68k, the 68040 and ColdFire included.
   Everything without CAS - ColdFire, 68000, 68010 - uses the kernel helper.  */
#if defined __mc68020__ || defined __mc68030__ \
    || defined __mc68040__ || defined __mc68060__
# define __M68K_HAVE_CAS	1
#endif

#ifndef __M68K_HAVE_CAS
# include <sys/syscall.h>
#endif

typedef int8_t atomic8_t;
typedef uint8_t uatomic8_t;
typedef int_fast8_t atomic_fast8_t;
typedef uint_fast8_t uatomic_fast8_t;

typedef int16_t atomic16_t;
typedef uint16_t uatomic16_t;
typedef int_fast16_t atomic_fast16_t;
typedef uint_fast16_t uatomic_fast16_t;

typedef int32_t atomic32_t;
typedef uint32_t uatomic32_t;
typedef int_fast32_t atomic_fast32_t;
typedef uint_fast32_t uatomic_fast32_t;

typedef int64_t atomic64_t;
typedef uint64_t uatomic64_t;
typedef int_fast64_t atomic_fast64_t;
typedef uint_fast64_t uatomic_fast64_t;

typedef intptr_t atomicptr_t;
typedef uintptr_t uatomicptr_t;
typedef intmax_t atomic_max_t;
typedef uintmax_t uatomic_max_t;

/* UP m68k; the compare-and-exchange below is itself the read-modify-write
   atomic, so a plain compiler barrier around it is enough.  (The kernel's
   sys_atomic_barrier is a no-op on uniprocessors.)  */
#define atomic_full_barrier()	__asm__ __volatile__ ("" ::: "memory")
#define atomic_read_barrier()	atomic_full_barrier ()
#define atomic_write_barrier()	atomic_full_barrier ()

#ifdef __M68K_HAVE_CAS

/* cas Dc,Du,<ea>: compare <ea> with Dc; if equal store Du, else load <ea>
   into Dc.  Either way Dc ends up holding the original *MEM, which is the
   value compare-and-exchange must return.  */
#define __arch_compare_and_exchange_val_8_acq(mem, newval, oldval) \
  ({ __typeof (*(mem)) __ret = (oldval);				\
     __asm__ __volatile__ ("cas%.b %0,%2,%1"				\
		       : "=d" (__ret), "+m" (*(mem))			\
		       : "d" (newval), "0" (__ret)			\
		       : "memory");					\
     __ret; })

#define __arch_compare_and_exchange_val_16_acq(mem, newval, oldval) \
  ({ __typeof (*(mem)) __ret = (oldval);				\
     __asm__ __volatile__ ("cas%.w %0,%2,%1"				\
		       : "=d" (__ret), "+m" (*(mem))			\
		       : "d" (newval), "0" (__ret)			\
		       : "memory");					\
     __ret; })

#define __arch_compare_and_exchange_val_32_acq(mem, newval, oldval) \
  ({ __typeof (*(mem)) __ret = (oldval);				\
     __asm__ __volatile__ ("cas%.l %0,%2,%1"				\
		       : "=d" (__ret), "+m" (*(mem))			\
		       : "d" (newval), "0" (__ret)			\
		       : "memory");					\
     __ret; })

#else /* !__M68K_HAVE_CAS */

/* d0 = syscall number, d1 = newval, d2 = oldval, a0 = mem; the helper
   returns the previous value.  The swap happens inside the syscall, so
   nothing in userspace can interpose between the load and the store.  */
#define __arch_compare_and_exchange_val_32_acq(mem, newval, oldval) \
  ({ register uint32_t __d0 __asm__ ("d0") = __NR_atomic_cmpxchg_32;	\
     register uint32_t __d1 __asm__ ("d1") = (uint32_t) (newval);	\
     register uint32_t __d2 __asm__ ("d2") = (uint32_t) (oldval);	\
     register uint32_t *__a0 __asm__ ("a0") = (uint32_t *) (mem);	\
     __asm__ __volatile__ ("trap #0"					\
		       : "+d" (__d0), "+m" (*__a0)			\
		       : "a" (__a0), "d" (__d2), "d" (__d1)		\
		       : "memory");					\
     (__typeof (*(mem))) __d0; })

/* The kernel helper is 32-bit only.  uClibc's atomic users are int- and
   pointer-sized, so these stay non-atomic rather than have a 32-bit access
   clobber the neighbouring bytes.  */
#define __arch_compare_and_exchange_val_8_acq(mem, newval, oldval) \
  ({ __typeof (mem) __gmem = (mem);					\
     __typeof (*(mem)) __gret = *__gmem;				\
     if (__gret == (oldval))						\
       *__gmem = (newval);						\
     __gret; })

#define __arch_compare_and_exchange_val_16_acq(mem, newval, oldval) \
  __arch_compare_and_exchange_val_8_acq (mem, newval, oldval)

#endif /* __M68K_HAVE_CAS */

/* m68k has no 64-bit CAS; NPTL and include/atomic.h only use int- and
   pointer-sized objects, so the bysize dispatch never reaches this.  */
#define __arch_compare_and_exchange_val_64_acq(mem, newval, oldval) \
  ({ __typeof (*(mem)) __ret = *(mem); abort (); (void) (newval);	\
     (void) (oldval); __ret; })

#endif /* _M68K_BITS_ATOMIC_H */
