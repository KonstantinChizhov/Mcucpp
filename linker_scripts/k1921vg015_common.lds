/*
*  @brief Common part of bare metal linker script
*/

/*
*  Some external definitions are requeued:
* - output format, arch and entry point (OUTPUT_FORMAT, OUTPUT_ARCH, ENTRY commands)
* - memory layout (MEMORY command)
* - memory regions' aliases (REGION_ALIAS comand)
* - stack size (STACK_SIZE symbol, i.e. "STACK_SIZE = 2048;")
* - size of heap ("HEAP_FIXED_AFTER_BSS=<size>", default it is a space between end of .bss and start of .stack)
*/

STACK_SIZE = DEFINED(STACK_SIZE) ? STACK_SIZE : 2048;

SECTIONS {
  /* startup/crt code segment */
  .text.startup ORIGIN(REGION_TEXT) : {
    *(.startup.entry .startup.*)
    . = ALIGN(16);
    *(.init.rodata .init.rodata.*)
    . = ALIGN(16);
    *(.init.data .init.data.*)
    . = ALIGN(16);
    *(.init.text .init.text.*)
    . = ALIGN(16);
    /* Constructors and Destructors */
    __ctors_start__ = .;
		KEEP(SORT(*)(.init_array)) 
		__ctors_end__ = .;
    . = ALIGN(16);
		PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array))
    PROVIDE_HIDDEN (__fini_array_end = .);

    /* End Constructors and Destructors */

  } >REGION_TEXT

  __TEXT_INIT_START__ = LOADADDR(.text.startup);
  __TEXT_INIT_SIZE__ = SIZEOF(.text.startup);

  .text.crt : ALIGN(16) {
    *(.text.crt*)
  } >REGION_TEXT

  /* code segment */
  .text : ALIGN(4) {
    PROVIDE(__TEXT_START__ = .);
    *(.text .text.*)
    PROVIDE(__TEXT_END__ = .);
  } >REGION_TEXT

  /* thread-local data segment */
  .tdata : ALIGN(4) {
    PROVIDE(_tls_data = .);
    PROVIDE(_tdata_start = .);
    *(.tdata .tdata.*)
    PROVIDE(_tdata_end = .);
  } >REGION_RODATA

  .tbss : ALIGN(4) {
    PROVIDE(_tbss_start = .);
    *(.tbss .tbss.*)
    . = ALIGN(4);
    PROVIDE(_tbss_end = .);
  } >REGION_RODATA

  __TLS_SIZE__ = (_tbss_end - _tdata_start + 0xf) & ~0xf;

  /* read-only data segment */
  .rodata : ALIGN(4) {
    *(.rodata) *(.rodata.*) *(.gnu.linkonce.r.*)
  } >REGION_RODATA

  /* small read-only data segment */
  .srodata : {
    *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2) *(.srodata*)
  } >REGION_RODATA

  /* small data segment */
  .sdata : ALIGN(4) {
    __SDATA_BEGIN__ = .;
    *(.sdata .sdata.* .sdata2.* .gnu.linkonce.s.*)
  } >REGION_DATA AT>REGION_TEXT

  /* data segment */
  .data : ALIGN(4) {
    __DATA_BEGIN__ = .;
    *(.data .data.* .gnu.linkonce.d.*)
    _edata = .; PROVIDE (edata = .);
  } >REGION_DATA AT>REGION_TEXT
  
  PROVIDE( __data_source_start = LOADADDR(.data) );
  PROVIDE( __data_target_start = ADDR(.data) );
  PROVIDE( __data_target_end = ADDR(.data) + SIZEOF(.data) );
  
  PROVIDE( __sdata_source_start = LOADADDR(.sdata) );
  PROVIDE( __sdata_target_start = ADDR(.sdata) );
  PROVIDE( __sdata_target_end = ADDR(.sdata) + SIZEOF(.sdata) );

  /* bss segment */
  .sbss : {
    PROVIDE(__bss_start = .);
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.scommon)
  } >REGION_BSS

  .bss : ALIGN(4) {
    *(.bss .bss.* .gnu.linkonce.b.*)
    *(COMMON)
    . = ALIGN(4);
    __BSS_END__ = .;
    PROVIDE(__bss_end = .);
    PROVIDE(_end = .);
    PROVIDE(__end = .);
  } >REGION_BSS

  /* End of uninitalized data segement */

  __global_pointer$ = MIN(__DATA_BEGIN__ + 0x780,
                          MAX(__SDATA_BEGIN__ + 0x780, __BSS_END__ - 0x780));

  /* Stack segement */
  .stack ORIGIN(REGION_STACK) + LENGTH(REGION_STACK) - STACK_SIZE - __TLS_SIZE__ : {
    PROVIDE(__STACK_START__ = .);
    . += STACK_SIZE;
    PROVIDE(__C_STACK_TOP__ = .);
    PROVIDE(__STACK_END__ = .);
    PROVIDE(__TLS0_BASE__ = .);
  } >REGION_STACK

  _heap_start = ALIGN(__bss_end, 16);
  _heap_end   = DEFINED(HEAP_FIXED_AFTER_BSS) ? MIN(ALIGN(__STACK_START__, 16), (_heap_start + HEAP_FIXED_AFTER_BSS)) : ALIGN(__STACK_START__, 16);

  /* discard relocation code */
  /* plf_init_relocate = plf_init_noreloc;*/

  /DISCARD/ : {
    *(.init.text.plf_init_relocate)
    *(.eh_frame .eh_frame.*)
  }
}
