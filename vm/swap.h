#ifndef VM_SWAP_H
#define VM_SWAP_H
#include <bitmap.h>
#include "devices/block.h"
#include "threads/synch.h"

struct block* swap_slots;
struct bitmap* swap_table;
void swap_table_init ();
int swap_out (void*);
void swap_in (void*, int);

struct lock swap_lock;
#endif 
