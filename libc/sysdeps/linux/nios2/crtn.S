
   .section .init

    ldw ra, 4(sp)
    ldw r22, 0(sp)
    addi sp, sp, 8
    ret

   .section .fini

    ldw ra, 4(sp)
    ldw r22, 0(sp)
    addi sp, sp, 8
    ret
