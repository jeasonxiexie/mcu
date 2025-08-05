    .syntax unified
    .cpu cortex-m0plus
    .thumb

    .section .isr_vector, "a", %progbits
    .type isr_vector, %object
    
isr_vector:
    .word   0x20002000  /* Stack pointer */
    .word   Reset_Handler
    .word   Default_Handler
    .word   Default_Handler
    /* ... other vectors ... */
    .fill   60, 4, Default_Handler
    
    .section .text.Reset_Handler
    .weak Reset_Handler
    .type Reset_Handler, %function
Reset_Handler:
    /* Call Early_PowerLatch first */
    bl      Early_PowerLatch
    
    /* Initialize data and bss */
    ldr     r0, =_sdata
    ldr     r1, =_edata
    ldr     r2, =_sidata
    movs    r3, #0
    b       LoopCopyDataInit
    
CopyDataInit:
    ldr     r4, [r2, r3]
    str     r4, [r0, r3]
    adds    r3, r3, #4
    
LoopCopyDataInit:
    adds    r4, r0, r3
    cmp     r4, r1
    bcc     CopyDataInit
    
    /* Zero bss */
    ldr     r2, =_sbss
    ldr     r4, =_ebss
    movs    r3, #0
    b       LoopFillZerobss
    
FillZerobss:
    str     r3, [r2]
    adds    r2, r2, #4
    
LoopFillZerobss:
    cmp     r2, r4
    bcc     FillZerobss
    
    /* Call main */
    bl      main
    b       .
    
    .section .text.Default_Handler
    .type Default_Handler, %function
Default_Handler:
    b       .
    
    .end