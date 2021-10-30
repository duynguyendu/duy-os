#ifndef __PAGING_H__
#define __PAGING_H__

#include <isr.h>
#include <types.h>

/* Fields in page table entry */
struct page {
    uint32_t present : 1;
    uint32_t rw : 1;
    uint32_t us : 1;
    uint32_t reserved_low : 2;
    uint32_t accessed : 1;
    uint32_t dirty : 1;
    uint32_t reserved_high : 2;
    uint32_t avail : 3;
    uint32_t frame_addr : 20;
};
typedef struct page page_t;

struct page_table {
    page_t pages[1024];
};
typedef struct page_table page_table_t;

/* Level 2 page table */
struct page_table_directory {
    page_table_t *tables[1024];

    /* Physical address of above tables to load into CR3 */
    uint32_t tables_physical[1024];

    /* Physical address of tables_physical, used when we implemnt heap
    When we enable paging, new addr will be virtualized, so we need to store
    physical addr so we can copy page_dir */
    uint32_t physical_addr;
};
typedef struct page_table_directory page_table_directory_t;

void paging_init();
void switch_page_directory(page_table_directory_t *new_dir);
page_t *get_page(uint32_t addr, int make, page_table_directory_t *dir);
void page_fault_handler(registers_t r);

/* Frame code */
void allocate_frame(page_t *page, uint32_t is_kernel, uint32_t is_writable);
void free_frame(page_t *page);

#endif // !__PAGING_H__
