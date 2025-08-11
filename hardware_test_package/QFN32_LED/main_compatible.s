    .syntax unified
    .cpu cortex-m0plus
    .thumb

    .global TIM10_IRQHandler
    .section .text.TIM10_IRQHandler
    .thumb_func
TIM10_IRQHandler:
    push {r4, lr}
    
    @ Check TIM10 flag
    ldr r0, =0x40014410  @ TIM10->SR
    ldr r1, [r0]
    tst r1, #1
    beq .L_exit
    
    @ Clear flag
    movs r1, #0
    str r1, [r0]
    
    @ Toggle PC0 (not PC4)
    ldr r0, =0x48000818  @ GPIOC->BSRR
    ldr r1, =toggle_state
    ldr r2, [r1]
    cmp r2, #0
    beq .L_set_high
    
.L_set_low:
    movs r3, #1      @ Set PC0 low (LED on)
    str r3, [r0]
    movs r2, #0
    str r2, [r1]
    b .L_exit
    
.L_set_high:
    movs r3, #1
    lsls r3, #16     @ Set PC0 high (LED off)
    str r3, [r0]
    movs r2, #1
    str r2, [r1]
    
.L_exit:
    pop {r4, pc}

    .section .data
toggle_state:
    .word 0
