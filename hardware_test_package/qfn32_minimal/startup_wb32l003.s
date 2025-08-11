/**
 * @file startup_wb32l003.s
 * @brief WB32L003 启动文件（精简版）
 */

    .syntax unified
    .cpu cortex-m0
    .thumb

.global g_pfnVectors
.global Default_Handler

/* 堆栈顶部地址 */
.word _estack

/* 复位向量 */
.word Reset_Handler

/* 异常向量 */
.word NMI_Handler
.word HardFault_Handler
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word SVC_Handler
.word 0
.word 0
.word PendSV_Handler
.word SysTick_Handler

/* 外部中断向量（简化，全部指向默认处理） */
.rept 32
.word Default_Handler
.endr

/* 复位处理 */
.section .text.Reset_Handler
.weak Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
    /* 设置堆栈指针 */
    ldr r0, =_estack
    mov sp, r0

    /* 复制初始化数据从Flash到RAM */
    ldr r0, =_sdata
    ldr r1, =_edata
    ldr r2, =_sidata
    movs r3, #0
    b LoopCopyDataInit

CopyDataInit:
    ldr r4, [r2, r3]
    str r4, [r0, r3]
    adds r3, r3, #4

LoopCopyDataInit:
    adds r4, r0, r3
    cmp r4, r1
    bcc CopyDataInit

    /* 清零BSS段 */
    ldr r2, =_sbss
    ldr r4, =_ebss
    movs r3, #0
    b LoopFillZerobss

FillZerobss:
    str r3, [r2]
    adds r2, r2, #4

LoopFillZerobss:
    cmp r2, r4
    bcc FillZerobss

    /* 调用main函数 */
    bl main
    bx lr

.size Reset_Handler, .-Reset_Handler

/* 默认中断处理 */
.section .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
    b Infinite_Loop
.size Default_Handler, .-Default_Handler

/* 弱定义的中断处理函数 */
.weak NMI_Handler
.thumb_set NMI_Handler,Default_Handler

.weak HardFault_Handler
.thumb_set HardFault_Handler,Default_Handler

.weak SVC_Handler
.thumb_set SVC_Handler,Default_Handler

.weak PendSV_Handler
.thumb_set PendSV_Handler,Default_Handler

.weak SysTick_Handler
.thumb_set SysTick_Handler,Default_Handler