/*
 * LoongArch64 PLT resolver stub.
 * Copyright (C) 2024 uClibc-ng contributors.
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#include <features.h>
#include <sysdep.h>
#include <sys/asm.h>

/* Assembler veneer called from the PLT header code for lazy loading.
   The PLT header passes its own args in t0-t2.  */

#define FRAME_SIZE (-((-10 * SZREG - 8 * SZFREG) & ALMASK))

ENTRY (_dl_linux_resolve)
  /* Save arguments to stack.  */
  addi.d $sp, $sp, -FRAME_SIZE
  REG_S $ra,  $sp, 9*SZREG
  REG_S $a0,  $sp, 1*SZREG
  REG_S $a1,  $sp, 2*SZREG
  REG_S $a2,  $sp, 3*SZREG
  REG_S $a3,  $sp, 4*SZREG
  REG_S $a4,  $sp, 5*SZREG
  REG_S $a5,  $sp, 6*SZREG
  REG_S $a6,  $sp, 7*SZREG
  REG_S $a7,  $sp, 8*SZREG

#ifndef __loongarch_soft_float
  FREG_S $fa0, $sp, (10*SZREG + 0*SZFREG)
  FREG_S $fa1, $sp, (10*SZREG + 1*SZFREG)
  FREG_S $fa2, $sp, (10*SZREG + 2*SZFREG)
  FREG_S $fa3, $sp, (10*SZREG + 3*SZFREG)
  FREG_S $fa4, $sp, (10*SZREG + 4*SZFREG)
  FREG_S $fa5, $sp, (10*SZREG + 5*SZFREG)
  FREG_S $fa6, $sp, (10*SZREG + 6*SZFREG)
  FREG_S $fa7, $sp, (10*SZREG + 7*SZFREG)
#endif

  /* Update .got.plt and obtain runtime address of callee.
     t0 = link map, t1 = .got.plt offset (in entries).  */
  slli.d $a1, $t1, 1
  or     $a0, $t0, $zero   /* link map */
  add.d  $a1, $a1, $t1     /* reloc offset (== thrice the .got.plt offset) */
  la.local $t2, _dl_fixup
  jirl   $ra, $t2, 0
  or     $t1, $a0, $zero

  /* Restore arguments from stack.  */
  REG_L $ra,  $sp, 9*SZREG
  REG_L $a0,  $sp, 1*SZREG
  REG_L $a1,  $sp, 2*SZREG
  REG_L $a2,  $sp, 3*SZREG
  REG_L $a3,  $sp, 4*SZREG
  REG_L $a4,  $sp, 5*SZREG
  REG_L $a5,  $sp, 6*SZREG
  REG_L $a6,  $sp, 7*SZREG
  REG_L $a7,  $sp, 8*SZREG

#ifndef __loongarch_soft_float
  FREG_L $fa0, $sp, (10*SZREG + 0*SZFREG)
  FREG_L $fa1, $sp, (10*SZREG + 1*SZFREG)
  FREG_L $fa2, $sp, (10*SZREG + 2*SZFREG)
  FREG_L $fa3, $sp, (10*SZREG + 3*SZFREG)
  FREG_L $fa4, $sp, (10*SZREG + 4*SZFREG)
  FREG_L $fa5, $sp, (10*SZREG + 5*SZFREG)
  FREG_L $fa6, $sp, (10*SZREG + 6*SZFREG)
  FREG_L $fa7, $sp, (10*SZREG + 7*SZFREG)
#endif

  addi.d $sp, $sp, FRAME_SIZE

  /* Invoke the callee.  */
  jr $t1
END (_dl_linux_resolve)