#ifndef VM_FRAME_H
#define VM_FRAME_H
#include "threads/thread.h"
#include "vm/page.h"
#include "threads/palloc.h"
#include <list.h>
struct list frame_table;

struct frame
  {
    struct thread* owner;
    struct spage* spage;
    void* phys_addr;
    struct list_elem elem;
  };

void frame_table_init ();
void add_frame (void*);
void evict_frame (void*);
void* obtain_frame (enum palloc_flags);
void free_frame (void*);
#endif
