#include "vm/swap.h"
#include "threads/vaddr.h"

void swap_table_init() {
    swap_slots = block_get_role(BLOCK_SWAP);
    int device_page_size = block_size(swap_slots) / (PGSIZE / BLOCK_SECTOR_SIZE);
    swap_table = bitmap_create(device_page_size);
    lock_init(&swap_lock);
}

int swap_out(void* frame) {
    lock_acquire(&swap_lock);
    int index = bitmap_scan_and_flip(swap_table, 0, 1, false);
    if (index == BITMAP_ERROR) PANIC("Swap slots are full");
    int i = 0;
    while (i < PGSIZE/BLOCK_SECTOR_SIZE) {
        block_write(swap_slots, index*(PGSIZE/BLOCK_SECTOR_SIZE)+i, frame+i*BLOCK_SECTOR_SIZE);
        i++;
    }
    lock_release(&swap_lock);
    return index;
}

void swap_in(void* frame, int index) {
    lock_acquire(&swap_lock);
    int i = 0;
    while (i < PGSIZE/BLOCK_SECTOR_SIZE) {
        block_read(swap_slots, index*(PGSIZE/BLOCK_SECTOR_SIZE)+i, frame+i*BLOCK_SECTOR_SIZE);
        i++;
    }
    lock_release(&swap_lock);
}
