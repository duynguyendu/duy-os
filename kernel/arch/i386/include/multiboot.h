#ifndef __MULTIBOOT_H__
#define __MULTIBOOT_H__

#include <types.h>

/* The magic field should contain this. */
#define MULTIBOOT_HEADER_MAGIC 0x1BADB002

/* This should be in %eax. */
#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

// TODO define other flags and stuff bits

/* Multiboot header */
struct multiboot_header {
    /* Magic number for header, which must be 0x1BADB002 */
    uint32_t magic;
    uint32_t flags;
    uint32_t checksum;

    /* Is used when flags[16] is set
     * Boot loader will use this instead of executable header to
     * caculate where to load OS.
     * It is not needed for ELF kernel, but must be provided in
     * a.out format or so. */
    uint32_t header_addr;   // Beginning of MB header where magic value is
                            // supposed to be loaded
    uint32_t load_addr;     // Phy addr of the beginning of text seg, must be <=
                            // header_addr
    uint32_t load_end_addr; // Phy addr of the end of data seg. (load_end_addr -
                            // load_addr) is how much data will be loaded
    uint32_t bss_end_addr;  // Phy addr of the end of bss seg. Boot loader will
                            // init this to 0
    uint32_t entry_addr;    // Phy addr to jump to run OS

    /* Information about video mode table when flags[2] is set */
    uint32_t
        mode_type;   // 0 for linear graphics mode, 1 for EGA-standard text mode
    uint32_t width;  // Numbers of column, in px or characters
    uint32_t height; // Numbers of row, in px or characters
    uint32_t depth;  // Number of bits in a px, 0 for text mode
} __attribute__((packed));
typedef struct multiboot_header multiboot_header_t;

/* Multiboot module */
struct multiboot_module {
    uint32_t mod_start; // Start address of the structure
    uint32_t mod_end;   // End address of the structure

    uint32_t string; // Can be a command line or pathname

    uint32_t ignored; // Must be set to 0 and is ignored by kernel
} __attribute__((packed));
typedef struct multiboot_module multiboot_module_t;

/* Symbol table from a.out kernel image */
struct multiboot_symbol_table_excutable {
    /* | array size | ... array of nlist structures ... | string size | ...
     * string ... | */
    uint32_t tabsize; // Size of array of nlist structures
    uint32_t strsize; // Size of string
    uint32_t addr;    // Start address (point to size of array);
    uint32_t ignored; // Ignored by kernel (for alignment)
} __attribute__((packed));
typedef struct multiboot_symbol_table_excutable
    multiboot_symbol_table_excutable_t;

/* Section header table from an ELF kernel */
struct multiboot_section_header_table_elf {
    uint32_t num;   // Number of entries
    uint32_t size;  // Size of each entries
    uint32_t addr;  // Start addr of the table
    uint32_t shndx; // FIXME I do not know what is this for
} __attribute__((packed));
typedef struct multiboot_section_header_table_elf
    multiboot_section_header_table_elf_t;

/* Memory map entry */
struct multiboot_mmap_entry {
    uint32_t size;      // Size of the entry
    uint64_t base_addr; // Start address
    uint64_t length;    // Size of the memory region in bytes
#define MULTIBOOT_MEMORY_AVAILABLE_RAM 1;
#define MULTIBOOT_MEMORY_ACPI 3;
#define MULTIBOOT_MEMORY_RESERVED_HIBERNATION 4;
#define MULTIBOOT_MEMORY_DEFECTIVE_RAM 5;
    uint32_t type; // Type of the memory
} __attribute__((packed));
typedef struct multiboot_mmap_entry multiboot_mmap_entry_t;

/* Drive structure */
struct multiboot_drive {
    uint32_t size;            // Size of this strucure (depends on ports)
    uint8_t drive_num;        // BIOS drive number
#define MULTIBOOT_DISK_CHS 0; // Traditional cylinders/head/sector mode
#define MULTIBOOT_DISK_LBA 1; // Logical block address mode
    uint8_t drive_mode;       // Access mode used by bootloader
    uint16_t drive_cylinders; // Number of cylinders detected by BIOS
    uint8_t drive_headers;    // Number of headers detected by BIOS
    uint8_t drive_sections;   // Number of sections per track detected by BIOS
    // FIXME drive ports
} __attribute__((packed));
typedef struct multiboot_drive multiboot_drive_t;

/* Advanced Power Management table */
struct multiboot_apm_table {
    uint16_t version; // Version number

    uint16_t cseg;   // Protected mode 32-bit code segment
    uint32_t offset; // Offset of entry point

    uint16_t cseg_16; // Protected mode 16-bit code segment
    uint16_t dseg;    // Protected mode 16-bit data segment
    uint16_t flags;   // Flags

    uint16_t cseg_len;    // Length of the 32-bit code seg
    uint16_t cseg_16_len; // Length of the 16-bit code seg
    uint16_t dseg_len;    // Length of the 16-bit data seg
} __attribute__((packed));
typedef struct multiboot_apm_table multiboot_apm_table_t;

/* Upon entry to OS, EBX will stores phy addr of this structure */
struct multiboot_info {
    uint32_t flags; // specifies the present of below fields

    /* flags[0]
     * Lower memory starts at 0, max 640Kb
     * Upper memory starts at 1MB, max = max upper_addr - 1MB */
    uint32_t mem_lower; // Amount of lower memory
    uint32_t mem_upper; // Amount of upper memory

    /* flags[1]
     * Indicate which BIOS disk device to load OS image from */
    uint32_t boot_device;

    /* flags[2]
     * Pass command from user to kernel */
    uint32_t cmd_line;

    /* flags[3]
     * Indicate the kernel what modules were loaded and where are they */
    uint32_t mods_count; // Number of modules loaded
    uint32_t mods_addr;  // Address of first module structure

    /* flags[4] and flags[5] are mutual exclusive
     * flags[4] indicate the kernel is a.out image
     * flags[5] indicate the kernel is elf file */
    union {
        struct multiboot_symbol_table_excutable aout_sym;  // flags[4]
        struct multiboot_section_header_table_elf elf_sec; // flags[5]
    } table;

    /* flags[6]
     * Indicate memory map buffer of the machine provided by BIOS */
    uint32_t mmap_length; // Size of the buffer
    uint32_t mmap_addr;   // Address of the buffer

    /* flags[7]
     * Disk drives information */
    uint32_t drives_length; // Total size of drive structure
    uint32_t drives_addr;   // Address of drive structure

    /* flags[8]
     * Indicate address of ROM configuration table */
    uint32_t config_table; // Addr of ROM configuration table

    /* flags[9]
     * Boot loader name */
    uint32_t boot_loader_name;

    /* flags[10]
     * Address of APM table */
    uint32_t apm_table; // Addr of APM table

    /* flags[11]
     * VBE (VESA BIOS Extensions) is available */
    uint32_t vbe_control_info; // Addr of control info
    uint32_t vbe_mode_info;    // Addr of mode info
    uint16_t vbe_mode;         // Current video format
    /* Below fields contain table of protected mode interface define in VBE 2.0
     */
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;

    /* flags[12]
     * Indicate framebuffer table
     * Framebuffer is a piece of memory shared between CPU and GPU.
     * CPU writes RGB pixels to the buffer and the GPU will render. */
    uint64_t framebuffer_addr;   // Phy addr of framebuffer table
    uint32_t framebuffer_pitch;  // Pitch (#bytes in each row) of framebuffer
    uint32_t framebuffer_width;  // Framebuffer width in pixel
    uint32_t framebuffer_height; // Framebuffer height in pixel
    uint8_t framebuffer_bpp;     // Number of bits per pixel
#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED 0
#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB 1
#define MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT                                    \
    2 // Text mode, charcter will replace px in width, height
    uint8_t framebuffer_type; // Framebuffer type
    union {
        // Framebuffer type 1
        struct {
            uint32_t framebuffer_palette_addr;
            uint16_t framebuffer_palette_num_colors;
        } color_info;
        // Framebuffer type 2
        struct {
            uint8_t framebuffer_red_field_position;
            uint8_t framebuffer_red_mask_size;
            uint8_t framebuffer_green_field_position;
            uint8_t framebuffer_green_mask_size;
            uint8_t framebuffer_blue_field_position;
            uint8_t framebuffer_blue_mask_size;
        } color_type;
    };
};
typedef struct multiboot_info multiboot_info_t;

struct multiboot_color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

void _main(multiboot_mmap_entry_t *);
long multiboot_read_bios_data(multiboot_info_t *mbd, uint32_t magic);

#endif // !__MULTIBOOT_H__
