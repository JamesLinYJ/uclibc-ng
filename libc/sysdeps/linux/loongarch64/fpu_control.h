/* FPU control word bits.  LoongArch64 version.
   Copyright (C) 2024 uClibc-ng contributors.

   Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
*/

#ifndef _FPU_CONTROL_H
#define _FPU_CONTROL_H

/* LoongArch FPU floating point control register bits.
 *
 * Bits 8-9:   RM (rounding mode)
 *   00 = round to nearest
 *   01 = round towards zero
 *   10 = round towards positive infinity
 *   11 = round towards negative infinity
 *
 * Bits 16-20: Flags (cumulative exception flags)
 *   Bit 16: V (invalid operation)
 *   Bit 17: Z (division by zero)
 *   Bit 18: O (overflow)
 *   Bit 19: U (underflow)
 *   Bit 20: I (inexact)
 *
 * Bits 0-4:   Enables
 *   Bit 0: V enable
 *   Bit 1: Z enable
 *   Bit 2: O enable
 *   Bit 3: U enable
 *   Bit 4: I enable
 */

#include <features.h>

/* Masks for rounding mode bits.  */
#define _FPU_RC_NEAREST 0x000   /* RECOMMENDED */
#define _FPU_RC_ZERO    0x100
#define _FPU_RC_UP      0x200
#define _FPU_RC_DOWN    0x300

#define _FPU_RESERVED   0xFFE0E0E0  /* Reserved bits */
#define _FPU_DEFAULT    0x00000000
#define _FPU_IEEE       0x00000000

/* Type of the control word.  */
typedef unsigned int fpu_control_t;

/* Macros for accessing the hardware control word.  */
#define _FPU_GETCW(cw) \
  __asm__ volatile ("movfcsr2gr %0, $fcsr0" : "=r" (cw))
#define _FPU_SETCW(cw) \
  __asm__ volatile ("movgr2fcsr $fcsr0, %0" : : "r" (cw))

/* Default control word set at startup.  */
extern fpu_control_t __fpu_control;

#endif /* fpu_control.h */