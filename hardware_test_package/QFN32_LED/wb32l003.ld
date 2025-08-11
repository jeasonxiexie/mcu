/* Linker Script for WB32L003 (Cortex-M0+) */

/* Memory Definitions */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 32K
    RAM (rwx)   : ORIGIN = 0x20000000, LENGTH = 4K
}

/* Stack size */
_Min_Stack_Size = 0x400; /* 1KB stack */

/* Entry Point */
ENTRY(Reset_Handler)

/* Sections */
SECTIONS
{
    /* Vector table goes first in FLASH */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } >FLASH

    /* Program code and constants */
    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        *(.glue_7)
        *(.glue_7t)
        *(.eh_frame)

        KEEP (*(.init))
        KEEP (*(.fini))

        . = ALIGN(4);
        _etext = .;
    } >FLASH

    /* ARM exception unwind info */
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } >FLASH

    .ARM :
    {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } >FLASH

    /* Data section (initialized data) */
    _sidata = LOADADDR(.data);
    
    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } >RAM AT> FLASH

    /* BSS section (uninitialized data) */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
    } >RAM

    /* User stack */
    ._user_heap_stack :
    {
        . = ALIGN(8);
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + _Min_Stack_Size;
        . = ALIGN(8);
    } >RAM

    /* Stack pointer for vector table */
    _estack = ORIGIN(RAM) + LENGTH(RAM);

    /* Remove debugging information */
    /DISCARD/ :
    {
        *(.ARM.exidx)
        *(.ARM.exidx.*)
    }

    .ARM.attributes 0 : { *(.ARM.attributes) }
}