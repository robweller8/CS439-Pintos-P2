#include "vm/frame.h"
#include "threads/thread.h"
#include "threads/malloc.h"
#include <list.h>

void frame_table_init ()
{
    list_init(&frame_table);
    lock_init(&frame_lock);
}

void add_frame (void* frame, struct spage* sp)
{
    lock_acquire(&frame_lock);
    struct frame* entry = malloc(sizeof(struct frame));
    entry->phys_addr = frame;
    entry->owner = thread_current();
    entry->spage = sp;
    list_push_back(&frame_table, &entry->elem);
    lock_release(&frame_lock);
}

void* evict_frame (enum palloc_flags flags)
{
    lock_acquire(&frame_lock);
    struct list_elem* e;
    for (e = list_begin(&frame_table); e != list_end(&frame_table);) {
        struct frame* entry = list_entry(e, struct frame, elem);
        if (pagedir_is_accessed(entry->owner->pagedir, entry->spage->vir_addr))
            pagedir_set_accessed(entry->owner->pagedir, entry->spage->vir_addr, false);
        else {
            if (pagedir_is_dirty(entry->owner->pagedir, entry->spage->vir_addr)) 
                entry->spage->swap_index = swap_out(entry->phys_addr);
            list_remove(&entry->elem);
            pagedir_clear_page(entry->owner->pagedir, entry->spage->vir_addr);
            free_frame(entry->phys_addr);
            void* return_value = obtain_frame(flags, entry->spage);
            free(entry);
            return return_value;
        }
        e = list_next(e);
        if (e == list_end(&frame_table)) e = list_begin(&frame_table);
    }
}

void* obtain_frame (enum palloc_flags flags, struct spage* sp)
{
    void* new_frame = palloc_get_page(flags);
    if (new_frame)
    {
        add_frame(new_frame, sp);
        return new_frame;
    }
    else
    {
        new_frame = evict_frame(flags);
        lock_release(&frame_lock);
        if (!new_frame) PANIC ("New frame cannot be obtained.");
        add_frame(new_frame, sp);
        return new_frame;
    }
    return NULL;
}

void free_frame (void* frame)
{
    lock_acquire(&frame_lock);
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
    lock_release(&frame_lock);
}
