#ifndef VM_PAGE_H
#define VM_PAGE_H
#include "filesys/file.h"
#include <list.h>
struct spage
  {
    void* vir_addr;
    bool dirty;
    bool writable;
    bool allocated;
    struct file *file;
    uint32_t ofs;
    uint32_t read_bytes;
    uint32_t zero_bytes;
    struct list_elem elem;
    int swap_index;
  };

struct spage* get_spage (void*);
#endif
