
SECTIONS {
    
    /* startup/crt code segment */
    .startup ORIGIN(REGION_TEXT) : {
        *crt0.o(.text .text.*)
    } >REGION_TEXT

    /* Code intended to be copied to REGION_RAM before execution */
    .ram_text : ALIGN(SECTION_ALIGNMENT) {
        PROVIDE(__RAM_TEXT_START__ = .);
        *(.small_ram_text)
        . = ORIGIN(REGION_RAM) + 0xC0;
        PROVIDE(__TRAP_TEXT_START__ = .);
        KEEP(*crt0.o(.trap_text))
        KEEP(*(.trap_text))
        *(.ram_text)
        . = ALIGN(4);
        PROVIDE(__RAM_TEXT_END__ = .);
    } >REGION_RAM AT>REGION_TEXT
    
    __RAM_TEXT_IMAGE_START__ = LOADADDR(.ram_text);
    __RAM_TEXT_IMAGE_END__ = LOADADDR(.ram_text) + SIZEOF(.ram_text);
    ASSERT(__RAM_TEXT_IMAGE_END__ < ORIGIN(REGION_TEXT) + LENGTH(REGION_TEXT), "REGION_TEXT segment overflows")

    
    .text : ALIGN(SECTION_ALIGNMENT) {
        PROVIDE(__TEXT_START__ = .);
        *(.text.SmallSystemInit)
        *(.text)
        *(.text.*)
        *(.gnu.linkonce.t.*)
        *(.rodata)
        *(.rodata.*)
        *(.gnu.linkonce.r.*)
        . = ALIGN(4);
        PROVIDE(__TEXT_END__ = .);
    } >REGION_TEXT 

    /* thread-local data segment */
    .tdata : ALIGN(SECTION_ALIGNMENT) {
        PROVIDE(_tls_data = .);
        PROVIDE(_tdata_start = .);
        *(.tdata .tdata.*)
        *(.gnu.linkonce.td.*)
        . = ALIGN(4);
        PROVIDE(_tdata_end = .);
    } >REGION_RAM AT>REGION_TEXT

    .tbss : ALIGN(SECTION_ALIGNMENT) {
        PROVIDE(_tbss_start = .);
        *(.tbss .tbss.*)
        *(.gnu.linkonce.tb.*)
        . = ALIGN(4);
        PROVIDE(_tbss_end = .);
    } >REGION_RAM

    __TDATA_IMAGE_START__ = LOADADDR(.tdata);
    __TDATA_IMAGE_END__ = LOADADDR(.tdata) + SIZEOF(.tdata);
    
    /* data segment */
    .data : ALIGN(SECTION_ALIGNMENT) {
        PROVIDE(__DATA_START__ = .);
        *(.data .data.*)
        *(.gnu.linkonce.d.*)
        . = ALIGN(4);
        PROVIDE(__DATA_END__ = .);
    } >REGION_RAM AT>REGION_TEXT

    /* small data segment */
    .sdata : ALIGN(SECTION_ALIGNMENT) {
        PROVIDE(__SDATA_START__ = .);
        *(.sdata .sdata.* .sdata2.* .gnu.linkonce.s.*)
    } >REGION_RAM AT>REGION_TEXT

    /* small read-only data segment */
    .srodata : ALIGN(SECTION_ALIGNMENT) {
        PROVIDE(__SRODATA_START__ = .);
        *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2) *(.srodata*)
    } >REGION_RAM AT>REGION_TEXT
    
    __DATA_IMAGE_START__ = LOADADDR(.data);
    __DATA_IMAGE_END__ = LOADADDR(.srodata) + SIZEOF(.srodata);


    /* small bss segment */
    .sbss : ALIGN(SECTION_ALIGNMENT) {
        PROVIDE(__SBSS_START__ = .);
        *(.sbss .sbss.*)
        *(.gnu.linkonce.sb.*)
        *(.scommon)
        . = ALIGN(4);
        PROVIDE(__SBSS_END__ = .);
    } >REGION_RAM

    /* bss segment */
    .bss : ALIGN(SECTION_ALIGNMENT) {
        PROVIDE(__BSS_START__ = .);
        *(.bss .bss.*)
        *(.gnu.linkonce.b.*)
        *(COMMON)
        . = ALIGN(4);
        PROVIDE(__BSS_END__ = .);
    } >REGION_RAM

    /* We must cover as much of sdata as possible if it exists.  If sdata+bss is
    smaller than 0x1000 then we should start from bss end to cover as much of
    the program as possible. */

    __global_pointer$ = MIN(__SDATA_START__ + 2016, MAX(__DATA_START__ + 2016, __BSS_END__ - 2016));

    ASSERT(__BSS_END__ < ORIGIN(REGION_RAM) + LENGTH(REGION_RAM) - __stack_size, "REGION_RAM section overflows")

    /* End of uninitalized data segement */
    PROVIDE(_end = .);
    PROVIDE(__end = .);
    PROVIDE(end = .);   /* Define the starting position of Heap */


    .stack ORIGIN(REGION_RAM) + LENGTH(REGION_RAM) - __stack_size : {
        . = ALIGN(16);
        PROVIDE( _heap_end = . );   /* Define the end of the Heap, and end of the stack. */
        PROVIDE(__STACK_START__ = .);
        . = __stack_size;
        PROVIDE(__C_STACK_TOP__ = .);
        PROVIDE(__STACK_END__ = .);
    } >REGION_RAM

    /DISCARD/ : {
        *(.eh_frame .eh_frame.*)
    }
}