/* Entry Point */
ENTRY(_start)

/* Specify the memory areas */
MEMORY {
	RAM (xrw)	: ORIGIN = 0x00000000, LENGTH = 256K
}

/* Define output sections */
SECTIONS {
	/* The startup code goes first into RAM */
	.entrypoint : {
		. = ALIGN(1);
		KEEP(*(.entrypoint)) /* Startup code */
		. = ALIGN(1);
	} >RAM

	/* The program code and other data goes into RAM */
	.text : {
		. = ALIGN(1);
		*(.text)	/* .text sections (code) */
		*(.text*)	/* .text* sections (code) */

		. = ALIGN(1);
		_etext = .;	/* define a global symbols at end of code */
	} >RAM

	/* Constant data goes into RAM */
	.rodata : {
		. = ALIGN(1);
		*(.rodata)	/* .rodata sections (constants, strings, etc.) */
		*(.rodata*)	/* .rodata* sections (constants, strings, etc.) */
		. = ALIGN(1);
	} >RAM

	/* Initialized data sections goes into RAM */
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
		_sbss = .;	/* define a global symbol at bss start */
		__bss_start__ = _sbss;
		*(.bss)
		*(.bss*)
		*(COMMON)

		. = ALIGN(1);
		_ebss = .;	/* define a global symbol at bss end */
		__bss_end__ = _ebss;

		/* end of memory */
		. = ALIGN(1);
		PROVIDE(end = .);
		PROVIDE(_end = .);
	} >RAM

	/* Remove unnecessary information */
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
