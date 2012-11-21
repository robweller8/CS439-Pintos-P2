#ifndef VM_SWAP_H
#define VM_SWAP_H
#include <bitmap.h>
#include "devices/block.h"

struct block* swap_slots;
struct bitmap* swap_table;
void swap_table_init ();
void write_swap (void*);
void read_swap (void*, int);

#endif 
