#include <memory.h>
#include <paging.h>
#include <stdio.h>
#include <string.h>

uint32_t *frames;
uint32_t nframes;

extern uint32_t placement_address;

#define KERNEL_OFFSET 0xC0000000

#define INDEX(bits) bits / (4 * 8)
#define OFFSET(bits) bits % (4 * 8)

static void set_frame(uint32_t frame_addr);
static void clear_frame(uint32_t frame_addr);
static uint32_t get_frame(uint32_t frame_addr);
static uint32_t first_frame();
static void enable_paging();

page_table_directory_t *current_dir;

void paging_init() {
    // TODO FIX THIS STUPID THINGS (MORE MEMORY)
    // TODO: number of frames depend on the amount of ram
    uint32_t end_mem_addr = 0x8000000;
    nframes = end_mem_addr / 0x1000;
    frames = (uint32_t *)malloc(INDEX(nframes));
    memset(frames, 0, INDEX(nframes));

    page_table_directory_t *kernel_directory =
        (page_table_directory_t *)malloc_a(sizeof(page_table_directory_t));
    memset(kernel_directory, 0, sizeof(page_table_directory_t));

    for (uint32_t i = 0; i < 1024; i++) {
        allocate_frame(get_page(i * 0x1000, 1, kernel_directory), 1, 1);
        copy_page(get_page(i * 0x1000, 1, kernel_directory),
                  get_page(i * 0x1000 + KERNEL_OFFSET, 1, kernel_directory));
    }

    for (uint32_t i = 0; i < 1024; i++) {
        allocate_frame(
            get_page(i * 0x1000 + KERNEL_OFFSET + PAGE_DIRECTORY_ENTRY_SIZE, 1, kernel_directory), 1, 1);
    }

    register_interrupt_handler(14, page_fault_handler);
    switch_page_directory(kernel_directory);
}

void enable_4mb_pages() {
    uint32_t cr4;
    __asm__ volatile("mov %%cr4, %0" : "=r"(cr4) :);
    cr4 |= 0x00000010;
    __asm__ volatile("mov %0, %%cr4" : : "r"(cr4));
}

void disable_4mb_pages() {
    uint32_t cr4;
    __asm__ volatile("mov %%cr4, %0" : "=r"(cr4) :);
    cr4 &= ~0x00000010;
    __asm__ volatile("mov %0, %%cr4" : : "r"(cr4));
}

void switch_page_directory(page_table_directory_t *new_dir) {
    current_dir = new_dir;
    __asm__ volatile("mov %0, %%cr3"
                     :
                     : "r"((uint32_t)&new_dir->tables_physical - KERNEL_OFFSET));
}

static void enable_paging() {
    uint32_t cr0;
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0) :);
    cr0 |= 0x80000000; // Enable paging!
    __asm__ volatile("mov %0, %%cr0" : : "r"(cr0));
}

page_t *get_page(uint32_t addr, int make, page_table_directory_t *dir) {
    addr /= 0x1000;

    uint32_t table_index = addr / 1024;
    if (dir->tables[table_index]) {
        return &dir->tables[table_index]->pages[addr % 1024];
    } else if (make) {
        uint32_t temp;
        dir->tables[table_index] =
            (page_table_t *)malloc_ap(sizeof(page_table_t), &temp);
        memset(dir->tables[table_index], 0, 0x1000);
        dir->tables_physical[table_index] = (temp - KERNEL_OFFSET) | 0x3;
        return &dir->tables[table_index]->pages[addr % 1024];
    } else {
        return 0;
    }
}

void page_fault_handler(registers_t r) {
    uint32_t faulting_address;
    __asm__ volatile("mov %%cr2, %0" : "=r"(faulting_address) :);
    int present = !(r.err_code & 0x1);
    int rw = !(r.err_code & 0x2);
    int us = !(r.err_code & 0x4);
    int reserved = !(r.err_code & 0x8);
    int id = !(r.err_code & 0x10);

    kprintf("Page Fault (");
    if (present) {
        kprintf("present ");
    }
    if (rw) {
        kprintf("read-only ");
    }
    if (us) {
        kprintf("user-mode ");
    }
    if (reserved) {
        kprintf("reserved");
    }
    kprintf(") at 0x%X\n", faulting_address);
    while (1)
        ;
}

static void set_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / 0x1000;
    uint32_t index = INDEX(frame);
    uint32_t offset = OFFSET(frame);
    frames[index] |= 1 << offset;
}

static void clear_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / 0x1000;
    uint32_t index = INDEX(frame);
    uint32_t offset = OFFSET(frame);
    frames[index] &= ~(1 << offset);
}

static uint32_t get_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / 0x1000;
    uint32_t index = INDEX(frame);
    uint32_t offset = OFFSET(frame);
    return frames[index] & (1 << offset);
}

static uint32_t first_frame() {
    for (uint32_t i = 0; i < INDEX(nframes); i++) {
        if (frames[i] != 0xFFFFFFFF) {
            for (uint32_t j = 0; j < 32; j++) {
                if (!(frames[i] & (1 << j))) {
                    return 32 * i + j;
                }
            }
        }
    }
    return -1;
}

void allocate_frame(page_t *page, uint32_t is_kernel, uint32_t is_writable) {
    uint32_t index = first_frame();
    if (index == -1) {
        /* Loop forever */
        kprintf("Out of memory\n");
        while (1)
            ;
    }

    set_frame(index * 0x1000);
    page->present = 1;
    page->us = is_kernel ? 1 : 0;
    page->rw = is_writable ? 1 : 0;
    page->frame_addr = index;
}

void map_to_frame(page_t *page, uint32_t frame_index, uint32_t is_kernel,
                   uint32_t is_writable) {
    set_frame(frame_index * 0x1000);
    page->present = 1;
    page->us = is_kernel ? 1 : 0;
    page->rw = is_writable ? 1 : 0;
    page->frame_addr = frame_index;
}

void copy_page(page_t *src, page_t *dest) {
    dest->frame_addr = src->frame_addr;
    dest->present = src->present;
    dest->us = src->us;
    dest->rw = src->rw;
}

void free_frame(page_t *page) {
    uint32_t frame = page->frame_addr;
    if (!frame) {
        return;
    } else {
        clear_frame(frame * 0x1000);
        page->frame_addr = 0x0;
    }
}
