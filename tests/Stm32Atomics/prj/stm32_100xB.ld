
	_STACKSIZE =1024;
	_HEAPSIZE = 128;

/* Specify the memory areas */
MEMORY
{
	FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 128K
	RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 8K
}

SECTIONS
{
	/* first section is .text which is used for code */

	.text :
	{
		CREATE_OBJECT_SYMBOLS
		KEEP(*(.isr_vectors))
		*(.text .text.*)
		*(.gnu.linkonce.t.*)
		*(.glue_7t) *(.glue_7) *(.vfp11_veneer)
		KEEP(*(.fini))
		*(.gcc_except_table)
        
        PROVIDE(__ctors_start__ = .);
		KEEP(*(SORT(.ctors.*)))
		KEEP(*(.ctors))
		PROVIDE(__ctors_end__ = .);
        
        
		PROVIDE(__dtors_start__ = .); 
		KEEP(*(SORT(.dtors.*)))
		KEEP(*(.dtors))
		PROVIDE(__dtors_end__ = .);
        
        *(.rodata .rodata.*)
		*(.gnu.linkonce.r.*)
	} >FLASH
	. = ALIGN(4);
    
    _etext = .;
	PROVIDE (etext = .);

	.init_array :
	{
		*(.init)
        *(.fini)
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP (*(.preinit_array))
		PROVIDE_HIDDEN (__preinit_array_end = .);
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array))
		PROVIDE_HIDDEN (__init_array_end = .);
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP (*(.fini_array))
		KEEP (*(SORT(.fini_array.*)))
		PROVIDE_HIDDEN (__fini_array_end = .);
	} >RAM

	. = ALIGN(4);

	/* .ARM.exidx is sorted, so has to go in its own output section.  */
	__exidx_start = .;
	.ARM.exidx :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} >RAM
	__exidx_end = .;

	

	/* .data section which is used for initialized data */

	.data : AT (_etext)
	{
		__data_start = .;
		*(.data .data.*)
		*(.gnu.linkonce.d.*)
		SORT(CONSTRUCTORS)
		. = ALIGN(4);
		*(.fastrun .fastrun.*)
	} >RAM
	. = ALIGN(4);
	
	_edata = .;
	PROVIDE (edata = .);

	/* .bss section which is used for uninitialized data */

	.bss :
	{
		__bss_start = .;
		__bss_start__ = .;
		*(.bss .bss.*)
		*(.gnu.linkonce.b.*)
		*(COMMON)
		. = ALIGN(4);
	} >RAM
	__bss_end__ = .;
	
	_end = .;
	PROVIDE(end = .);

	/* .heap section which is used for memory allocation */
	
	.heap (NOLOAD) :
	{
		__heap_start__ = .;
		*(.heap)
		. = MAX(__heap_start__ + _HEAPSIZE , .);
	} >RAM
	__heap_end__ = __heap_start__ + SIZEOF(.heap);
	
	/* .stack section - user mode stack */
	
	.stack (__heap_end__ + 3) / 4 * 4 (NOLOAD) :
	{
		. = ALIGN(8);
		__stack_start__ = .;
		*(.stack)
		. = ALIGN(8);
		. = MAX(__stack_start__ + _STACKSIZE , .);
	} >RAM
	__stack_end__ = __stack_start__ + SIZEOF(.stack);

}
