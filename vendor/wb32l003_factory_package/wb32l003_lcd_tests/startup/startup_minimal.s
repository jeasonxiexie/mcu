    .syntax unified
    .cpu cortex-m0plus
    .thumb

    .section .isr_vector, "a", %progbits
    .type isr_vector, %object
    
isr_vector:
    .word   0x20002000          /* Stack pointer */
    .word   Reset_Handler       /* Reset handler */
    
    /* Fill remaining vectors with Default_Handler */
    .rept   62
    .word   Default_Handler
    .endr
    
    .section .text.Reset_Handler
    .weak Reset_Handler
    .type Reset_Handler, %function
Reset_Handler:
    /* Call Early_PowerLatch first */
    bl      Early_PowerLatch
    
    /* Initialize data section */
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
    
    /* Zero bss section */
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
    
    /* Call SystemInit */
    bl      SystemInit
    
    /* Call main */
    bl      main
    
    /* Infinite loop if main returns */
.L_hang:
    b       .L_hang
    
    .section .text.Default_Handler, "ax", %progbits
Default_Handler:
    b       .
    
    /* Simple memcpy implementation */
    .section .text.memcpy, "ax", %progbits
    .global memcpy
    .type memcpy, %function
memcpy:
    movs    r3, #0
memcpy_loop:
    cmp     r3, r2
    bcs     memcpy_done
    ldrb    r4, [r1, r3]
    strb    r4, [r0, r3]
    adds    r3, r3, #1
    b       memcpy_loop
memcpy_done:
    bx      lr
    
    .weak   Early_PowerLatch
    .weak   SystemInit
    
    .end
