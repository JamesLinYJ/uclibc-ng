
   .section .init
   .balign 4
   .global _init
   .type   _init, @function
_init:
    addi sp, sp, -8
    /* The code gcc puts between crti and crtn reaches the GOT through r22,
       and r22 belongs to the caller here -- ld.so calls _init directly.  So
       set it up ourselves, as glibc's crti.S does.  */
    stw  r22, 0(sp)
    nextpc r22
1:  movhi r8, %hiadj(_gp_got - 1b)
    addi  r8, r8, %lo(_gp_got - 1b)
    add   r22, r22, r8
    stw  ra, 4(sp)

   .balign 4
   
   
   .section .fini
   .balign 4
   .global _fini
   .type   _fini, @function
_fini:
    addi sp, sp, -8
    /* The code gcc puts between crti and crtn reaches the GOT through r22,
       and r22 belongs to the caller here -- ld.so calls _init directly.  So
       set it up ourselves, as glibc's crti.S does.  */
    stw  r22, 0(sp)
    nextpc r22
1:  movhi r8, %hiadj(_gp_got - 1b)
    addi  r8, r8, %lo(_gp_got - 1b)
    add   r22, r22, r8
    stw  ra, 4(sp)
  .balign 4
