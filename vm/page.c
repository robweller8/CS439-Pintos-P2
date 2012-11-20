#include "vm/page.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include <list.h>

struct spage* get_spage (void* vir_addr) {
    vir_addr = pg_round_down (vir_addr);
    struct thread* cur = thread_current();
    struct list_elem* e;
    if (list_empty(&cur->spage_table)) return NULL;
    for (e = list_begin(&cur->spage_table); e != list_end(&cur->spage_table); e = list_next(e)) {
        struct spage* sp = list_entry(e, struct spage, allelem);
        if(vir_addr == sp->vir_addr) return sp;
    }
    return NULL;
}

