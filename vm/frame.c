#include "vm/frame.h"
#include "threads/thread.h"
#include "threads/malloc.h"
#include <list.h>

void frame_table_init ()
{
    list_init(&frame_table);
}

void add_frame (void* frame)
{
    struct frame* entry = malloc(sizeof(struct frame));
    entry->phys_addr = frame;
    entry->owner = thread_current();
    list_push_back(&frame_table, &entry->elem);
}

void evict_frame (void* frame)
{
    return; //TODO
}

void* obtain_frame (enum palloc_flags flags)
{
    void* new_frame = palloc_get_page(flags);
    if (new_frame)
    {
        add_frame(new_frame);
        return new_frame;
    }
    return NULL;
}

void free_frame (void* frame)
{
    struct list_elem *felem, *b = list_begin(&frame_table), *e = list_end(&frame_table);
    for (felem = b; felem != e; felem = list_next(felem))
    {
        struct frame* entry = list_entry(felem, struct frame, elem);
        if (entry->phys_addr == frame)
        {
            list_remove(felem);
            free(entry);
            break;
        }
    }
    palloc_free_page(frame);
}
