
main_original.o:     file format elf32-littlearm


Disassembly of section .rev16_text:

00000000 <__asm___6_main_c_delay____REV16>:
   0:	ba40      	rev16	r0, r0
   2:	4770      	bx	lr

Disassembly of section .revsh_text:

00000000 <__asm___6_main_c_delay____REVSH>:
   0:	bac0      	revsh	r0, r0
   2:	4770      	bx	lr

Disassembly of section i.TIM10_IRQHandler:

00000000 <TIM10_IRQHandler>:
   0:	b510      	push	{r4, lr}
   2:	4806      	ldr	r0, [pc, #24]	@ (1c <TIM10_IRQHandler+0x1c>)
   4:	f7ff fffe 	bl	0 <BaseTim_GetFlagStatus>
   8:	2800      	cmp	r0, #0
   a:	d006      	beq.n	1a <TIM10_IRQHandler+0x1a>
   c:	4803      	ldr	r0, [pc, #12]	@ (1c <TIM10_IRQHandler+0x1c>)
   e:	f7ff fffe 	bl	0 <BaseTim_ClearFlag>
  12:	2110      	movs	r1, #16
  14:	4802      	ldr	r0, [pc, #8]	@ (20 <TIM10_IRQHandler+0x20>)
  16:	f7ff fffe 	bl	0 <GPIO_ToggleBits>
  1a:	bd10      	pop	{r4, pc}
  1c:	40001800 	.word	0x40001800
  20:	40021800 	.word	0x40021800

Disassembly of section i.Tim1_Config:

00000000 <Tim1_Config>:
   0:	b500      	push	{lr}
   2:	b087      	sub	sp, #28
   4:	2101      	movs	r1, #1
   6:	2004      	movs	r0, #4
   8:	f7ff fffe 	bl	0 <RCC_AHBPeriphClockCmd>
   c:	4a21      	ldr	r2, [pc, #132]	@ (94 <Tim1_Config+0x94>)
   e:	2140      	movs	r1, #64	@ 0x40
  10:	4821      	ldr	r0, [pc, #132]	@ (98 <Tim1_Config+0x98>)
  12:	f7ff fffe 	bl	0 <GPIO_Init>
  16:	2101      	movs	r1, #1
  18:	0288      	lsls	r0, r1, #10
  1a:	f7ff fffe 	bl	0 <RCC_APBPeriphClockCmd>
  1e:	2100      	movs	r1, #0
  20:	481e      	ldr	r0, [pc, #120]	@ (9c <Tim1_Config+0x9c>)
  22:	f7ff fffe 	bl	0 <TIM_ARRPreloadConfig>
  26:	2109      	movs	r1, #9
  28:	4668      	mov	r0, sp
  2a:	8281      	strh	r1, [r0, #20]
  2c:	210b      	movs	r1, #11
  2e:	8201      	strh	r1, [r0, #16]
  30:	2100      	movs	r1, #0
  32:	82c1      	strh	r1, [r0, #22]
  34:	8241      	strh	r1, [r0, #18]
  36:	7601      	strb	r1, [r0, #24]
  38:	a904      	add	r1, sp, #16
  3a:	4818      	ldr	r0, [pc, #96]	@ (9c <Tim1_Config+0x9c>)
  3c:	f7ff fffe 	bl	0 <TIM_TimeBaseInit>
  40:	2101      	movs	r1, #1
  42:	4816      	ldr	r0, [pc, #88]	@ (9c <Tim1_Config+0x9c>)
  44:	f7ff fffe 	bl	0 <TIM_ARRPreloadConfig>
  48:	2160      	movs	r1, #96	@ 0x60
  4a:	4668      	mov	r0, sp
  4c:	8001      	strh	r1, [r0, #0]
  4e:	2105      	movs	r1, #5
  50:	80c1      	strh	r1, [r0, #6]
  52:	2101      	movs	r1, #1
  54:	8041      	strh	r1, [r0, #2]
  56:	2100      	movs	r1, #0
  58:	8081      	strh	r1, [r0, #4]
  5a:	8101      	strh	r1, [r0, #8]
  5c:	8141      	strh	r1, [r0, #10]
  5e:	21ff      	movs	r1, #255	@ 0xff
  60:	3101      	adds	r1, #1
  62:	8181      	strh	r1, [r0, #12]
  64:	0049      	lsls	r1, r1, #1
  66:	81c1      	strh	r1, [r0, #14]
  68:	4669      	mov	r1, sp
  6a:	480c      	ldr	r0, [pc, #48]	@ (9c <Tim1_Config+0x9c>)
  6c:	f7ff fffe 	bl	0 <TIM_OC1Init>
  70:	2108      	movs	r1, #8
  72:	480a      	ldr	r0, [pc, #40]	@ (9c <Tim1_Config+0x9c>)
  74:	f7ff fffe 	bl	0 <TIM_OC1PreloadConfig>
  78:	2101      	movs	r1, #1
  7a:	4808      	ldr	r0, [pc, #32]	@ (9c <Tim1_Config+0x9c>)
  7c:	f7ff fffe 	bl	0 <TIM_CCPreloadControl>
  80:	2101      	movs	r1, #1
  82:	4806      	ldr	r0, [pc, #24]	@ (9c <Tim1_Config+0x9c>)
  84:	f7ff fffe 	bl	0 <TIM_Cmd>
  88:	2101      	movs	r1, #1
  8a:	4804      	ldr	r0, [pc, #16]	@ (9c <Tim1_Config+0x9c>)
  8c:	f7ff fffe 	bl	0 <TIM_CtrlPWMOutputs>
  90:	b007      	add	sp, #28
  92:	bd00      	pop	{pc}
  94:	00110001 	.word	0x00110001
  98:	40021800 	.word	0x40021800
  9c:	40001000 	.word	0x40001000

Disassembly of section i.__NVIC_SetPriority:

00000000 <__NVIC_SetPriority>:
   0:	b510      	push	{r4, lr}
   2:	2800      	cmp	r0, #0
   4:	db19      	blt.n	3a <__NVIC_SetPriority+0x3a>
   6:	4a1a      	ldr	r2, [pc, #104]	@ (70 <__NVIC_SetPriority+0x70>)
   8:	2303      	movs	r3, #3
   a:	021b      	lsls	r3, r3, #8
   c:	18d2      	adds	r2, r2, r3
   e:	0883      	lsrs	r3, r0, #2
  10:	009b      	lsls	r3, r3, #2
  12:	58d2      	ldr	r2, [r2, r3]
  14:	0783      	lsls	r3, r0, #30
  16:	0edc      	lsrs	r4, r3, #27
  18:	23ff      	movs	r3, #255	@ 0xff
  1a:	40a3      	lsls	r3, r4
  1c:	439a      	bics	r2, r3
  1e:	078b      	lsls	r3, r1, #30
  20:	0e1b      	lsrs	r3, r3, #24
  22:	0784      	lsls	r4, r0, #30
  24:	0ee4      	lsrs	r4, r4, #27
  26:	40a3      	lsls	r3, r4
  28:	431a      	orrs	r2, r3
  2a:	4b11      	ldr	r3, [pc, #68]	@ (70 <__NVIC_SetPriority+0x70>)
  2c:	2403      	movs	r4, #3
  2e:	0224      	lsls	r4, r4, #8
  30:	191b      	adds	r3, r3, r4
  32:	0884      	lsrs	r4, r0, #2
  34:	00a4      	lsls	r4, r4, #2
  36:	511a      	str	r2, [r3, r4]
  38:	e018      	b.n	6c <__NVIC_SetPriority+0x6c>
  3a:	4a0e      	ldr	r2, [pc, #56]	@ (74 <__NVIC_SetPriority+0x74>)
  3c:	0703      	lsls	r3, r0, #28
  3e:	0f1b      	lsrs	r3, r3, #28
  40:	3b08      	subs	r3, #8
  42:	089b      	lsrs	r3, r3, #2
  44:	009b      	lsls	r3, r3, #2
  46:	58d2      	ldr	r2, [r2, r3]
  48:	0783      	lsls	r3, r0, #30
  4a:	0edc      	lsrs	r4, r3, #27
  4c:	23ff      	movs	r3, #255	@ 0xff
  4e:	40a3      	lsls	r3, r4
  50:	439a      	bics	r2, r3
  52:	078b      	lsls	r3, r1, #30
  54:	0e1b      	lsrs	r3, r3, #24
  56:	0784      	lsls	r4, r0, #30
  58:	0ee4      	lsrs	r4, r4, #27
  5a:	40a3      	lsls	r3, r4
  5c:	431a      	orrs	r2, r3
  5e:	4b05      	ldr	r3, [pc, #20]	@ (74 <__NVIC_SetPriority+0x74>)
  60:	0704      	lsls	r4, r0, #28
  62:	0f24      	lsrs	r4, r4, #28
  64:	3c08      	subs	r4, #8
  66:	08a4      	lsrs	r4, r4, #2
  68:	00a4      	lsls	r4, r4, #2
  6a:	511a      	str	r2, [r3, r4]
  6c:	bd10      	pop	{r4, pc}
  6e:	0000      	.short	0x0000
  70:	e000e100 	.word	0xe000e100
  74:	e000ed1c 	.word	0xe000ed1c

Disassembly of section i.delay:

00000000 <delay>:
   0:	bf00      	nop
   2:	4601      	mov	r1, r0
   4:	1e40      	subs	r0, r0, #1
   6:	2900      	cmp	r1, #0
   8:	d1fb      	bne.n	2 <delay+0x2>
   a:	4770      	bx	lr

Disassembly of section i.main:

00000000 <main>:
   0:	f7ff fffe 	bl	0 <SystemCoreClockUpdate>
   4:	f7ff fffe 	bl	0 <main>
   8:	2101      	movs	r1, #1
   a:	2004      	movs	r0, #4
   c:	f7ff fffe 	bl	0 <RCC_AHBPeriphClockCmd>
  10:	4a1f      	ldr	r2, [pc, #124]	@ (90 <main+0x90>)
  12:	2110      	movs	r1, #16
  14:	481f      	ldr	r0, [pc, #124]	@ (94 <main+0x94>)
  16:	f7ff fffe 	bl	0 <GPIO_Init>
  1a:	2101      	movs	r1, #1
  1c:	2008      	movs	r0, #8
  1e:	f7ff fffe 	bl	0 <RCC_AHBPeriphClockCmd>
  22:	4a1d      	ldr	r2, [pc, #116]	@ (98 <main+0x98>)
  24:	2110      	movs	r1, #16
  26:	481d      	ldr	r0, [pc, #116]	@ (9c <main+0x9c>)
  28:	f7ff fffe 	bl	0 <GPIO_Init>
  2c:	2101      	movs	r1, #1
  2e:	200e      	movs	r0, #14
  30:	f7ff fffe 	bl	0 <main>
  34:	200e      	movs	r0, #14
  36:	2800      	cmp	r0, #0
  38:	db05      	blt.n	46 <main+0x46>
  3a:	06c2      	lsls	r2, r0, #27
  3c:	0ed2      	lsrs	r2, r2, #27
  3e:	2101      	movs	r1, #1
  40:	4091      	lsls	r1, r2
  42:	4a17      	ldr	r2, [pc, #92]	@ (a0 <main+0xa0>)
  44:	6011      	str	r1, [r2, #0]
  46:	bf00      	nop
  48:	2101      	movs	r1, #1
  4a:	2040      	movs	r0, #64	@ 0x40
  4c:	f7ff fffe 	bl	0 <RCC_APBPeriphClockCmd>
  50:	4814      	ldr	r0, [pc, #80]	@ (a4 <main+0xa4>)
  52:	f7ff fffe 	bl	0 <BaseTim_DeInit>
  56:	2000      	movs	r0, #0
  58:	4913      	ldr	r1, [pc, #76]	@ (a8 <main+0xa8>)
  5a:	6008      	str	r0, [r1, #0]
  5c:	6048      	str	r0, [r1, #4]
  5e:	6088      	str	r0, [r1, #8]
  60:	20ff      	movs	r0, #255	@ 0xff
  62:	3001      	adds	r0, #1
  64:	60c8      	str	r0, [r1, #12]
  66:	2040      	movs	r0, #64	@ 0x40
  68:	6108      	str	r0, [r1, #16]
  6a:	2000      	movs	r0, #0
  6c:	6148      	str	r0, [r1, #20]
  6e:	6188      	str	r0, [r1, #24]
  70:	61c8      	str	r0, [r1, #28]
  72:	480e      	ldr	r0, [pc, #56]	@ (ac <main+0xac>)
  74:	6208      	str	r0, [r1, #32]
  76:	480b      	ldr	r0, [pc, #44]	@ (a4 <main+0xa4>)
  78:	f7ff fffe 	bl	0 <BaseTim_Init>
  7c:	2101      	movs	r1, #1
  7e:	4809      	ldr	r0, [pc, #36]	@ (a4 <main+0xa4>)
  80:	f7ff fffe 	bl	0 <BaseTim_ITConfig>
  84:	2101      	movs	r1, #1
  86:	4807      	ldr	r0, [pc, #28]	@ (a4 <main+0xa4>)
  88:	f7ff fffe 	bl	0 <BaseTim_Cmd>
  8c:	bf00      	nop
  8e:	e7fe      	b.n	8e <main+0x8e>
  90:	10100000 	.word	0x10100000
  94:	40021800 	.word	0x40021800
  98:	00110006 	.word	0x00110006
  9c:	40021c00 	.word	0x40021c00
  a0:	e000e100 	.word	0xe000e100
  a4:	40001800 	.word	0x40001800
  a8:	00000000 	.word	0x00000000
  ac:	0000ff9b 	.word	0x0000ff9b
