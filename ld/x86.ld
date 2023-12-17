/* Entry Point */
ENTRY(_start)

/* Highest address of the user mode stack */
_estack = 0x20040000;	/* end of RAM */
/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 0x200;	/* required amount of heap  */
_Min_Stack_Size = 0x400;/* required amount of stack */

/* Specify the memory areas */
MEMORY {
	RAM (xrw)	: ORIGIN = 0x00000000, LENGTH = 256K
}

/* Define output sections */
SECTIONS {
	/* The startup code goes first into FLASH */
	.entrypoint : {
		. = ALIGN(1);
		KEEP(*(.entrypoint)) /* Startup code */
		. = ALIGN(1);
	} >RAM

	/* The program code and other data goes into FLASH */
	.text : {
		. = ALIGN(1);
		*(.text)	/* .text sections (code) */
		*(.text*)	/* .text* sections (code) */

		. = ALIGN(1);
		_etext = .;	/* define a global symbols at end of code */
	} >RAM

	/* Constant data goes into FLASH */
	.rodata : {
		. = ALIGN(1);
		*(.rodata)	/* .rodata sections (constants, strings, etc.) */
		*(.rodata*)	/* .rodata* sections (constants, strings, etc.) */
		. = ALIGN(1);
	} >RAM

	/* used by the startup to initialize data */
	_sidata = LOADADDR(.data);

	/* Initialized data sections goes into RAM, load LMA copy after code */
	.data : {
		. = ALIGN(1);
		_sdata = .;	/* create a global symbol at data start */
		*(.data)	/* .data sections */
		*(.data*)	/* .data* sections */

		. = ALIGN(1);
		_edata = .;	/* define a global symbol at data end */
	} >RAM


	/* Uninitialized data section */
	. = ALIGN(4);
	.bss : {
		/* This is used by the startup in order to initialize the .bss
		 * secion */
		_sbss = .;	/* define a global symbol at bss start */
		__bss_start__ = _sbss;
		*(.bss)
		*(.bss*)
		*(COMMON)

		. = ALIGN(1);
		_ebss = .;	/* define a global symbol at bss end */
		__bss_end__ = _ebss;
	} >RAM

	/* User_heap_stack section, used to check that there is enough RAM left */
	._user_heap_stack : {
		. = ALIGN(1);
		PROVIDE(end = .);
		PROVIDE(_end = .);
		/*
		. = . + _Min_Heap_Size;
		. = . + _Min_Stack_Size;
		. = ALIGN(8);
		*/
	} >RAM



	/* Remove information from the standard libraries */
	/*
	/DISCARD/ : {
		libc.a(*)
		libm.a(*)
		libgcc.a(*)
	}
	*/

	/DISCARD/ : {
		*(.note)
		*(.note*)
		*(.gnu)
		*(.gnu*)
		*(.dynsym)
		*(.dynstr)
		*(.dynamic)
	}
}
