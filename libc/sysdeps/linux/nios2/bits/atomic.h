/* Copyright (C) 2026 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library.  If not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef _NIOS2_BITS_ATOMIC_H
#define _NIOS2_BITS_ATOMIC_H

#include <stdint.h>

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

/* nios2 is uniprocessor, so a compiler barrier is enough.  */
#define atomic_full_barrier()	__asm__ __volatile__ ("" ::: "memory")
#define atomic_read_barrier()	atomic_full_barrier ()
#define atomic_write_barrier()	atomic_full_barrier ()

/* nios2 R1 has no atomic instruction.  libgcc implements __sync_* through
   the kernel's __kuser_cmpxchg helper at 0x1004, which the kernel restarts
   when it interrupts the load/store pair.  */
#define __arch_compare_and_exchange_val_8_acq(mem, newval, oldval) \
  __sync_val_compare_and_swap ((mem), (oldval), (newval))

#define __arch_compare_and_exchange_val_16_acq(mem, newval, oldval) \
  __sync_val_compare_and_swap ((mem), (oldval), (newval))

#define __arch_compare_and_exchange_val_32_acq(mem, newval, oldval) \
  __sync_val_compare_and_swap ((mem), (oldval), (newval))

/* No 64-bit CAS.  NPTL and include/atomic.h only use int- and
   pointer-sized objects, so the bysize dispatch never reaches this.  */
#define __arch_compare_and_exchange_val_64_acq(mem, newval, oldval) \
  ({ __typeof (*(mem)) __ret = *(mem); abort (); (void) (newval);	\
     (void) (oldval); __ret; })

#endif /* _NIOS2_BITS_ATOMIC_H */
