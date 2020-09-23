/*
 * Memory Map Notes 
 * Set this up for a 16k/4k config, because those are nice round numbers.
 * 
 */

__stack_size = 64;

MEMORY
{
    /* 192k for the supervisor - 32k for bootloader - 4k for later */
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 16k

    /* See above for memory Map */
    SRAM     (rwx) :   ORIGIN = 0x20000000, LENGTH = 4k
}

SECTIONS
{
    .text :
    {
        _text = .;
        KEEP(*(.boot_stack_pointer))
        KEEP(*(.isr_vector))
        *(.text*)
        *(.rodata*)
        _etext = .;
    } > FLASH

    .data : AT(ADDR(.text) + SIZEOF(.text))
    {
        _data = .;
        _edata = .;
    } > SRAM

    .bss :
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > SRAM

    .supstack :
    {
	. = ALIGN(16); /* Align for dump readability */
	_stack_bottom = .;
	. += __stack_size;
	_stack_top = .;
    } > SRAM

}
