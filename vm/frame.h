#include "threads/thread.h"
#include "vm/page.h"
#include <bitmap.h>

struct frame
  {
    struct thread* owner;
    struct page* pg_entry;
    (void*) phys_addr;
  };


struct frame_table
  {
    struct bitmap *used_frames;
    struct frame* frames[383];
    
  };

void *get_frame ();
void init_frame(struct frame*);
