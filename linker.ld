/* The bootloader will look at this image and start execution at the symbol
   designated as the entry point. */
ENTRY(_start)
 
/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
	/* Begin putting sections at 1 MiB, a conventional place for kernels to be
	   loaded at by the bootloader. */
	. = 1M;
 
    /* Kernel will load to the higher half memory at Gb 3*/
    _kernel_offset = 0xC0000000;
    _kernel_start = .;

    /* First put the multiboot header, as it is required to be put very early
	   early in the image or the bootloader won't recognize the file format.
	   Next we'll put the .text section. */
    . += _kernel_offset;
    /* AT will set the LMA (local address). We want to load to lower memory 
     * before we can support paging */
    /* Code */
	.text ALIGN(4K) : AT (ADDR (.text) - _kernel_offset)
	{
		*(.multiboot)
		*(.text)
	}
 
	/* Read-only data. */
	.rodata ALIGN(4K) : AT (ADDR (.rodata) - _kernel_offset)
	{
		*(.rodata)
	}
 
	/* Read-write data (initialized) */
	.data ALIGN(4K) : AT (ADDR (.data) - _kernel_offset)
	{
		*(.data)
	}
 
	/* Read-write data (uninitialized) and stack */
	.bss ALIGN(4K) : AT (ADDR (.bss) - _kernel_offset)
	{
		*(COMMON)
		*(.bss)
	}

    /DISCARD/ : {
        *(.note.gnu.property)
    }

    /* Symbol to indicate the end of kernel */
    _kernel_end = .;
 
	/* The compiler may produce other sections, by default it will put them in
	   a segment with the same name. Simply add stuff here as needed. */
}
