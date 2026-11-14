/* Allocates the pool items and clears their weights. Retail keeps `p` at
   0($sp) across the allocation; `heap` is copied to a local first so SNC
   keeps it in a1 until the call (no parameter-register swap). */
#include "main/agent25_types.h"
extern "C" {
typedef struct PoolItem08854950 { char pad_00[0x44]; float weight; char pad_48[0x18]; } PoolItem08854950;
typedef struct Pool08854950 { int enabled; int field_04; PoolItem08854950 *items; int fC; int f10; void *heap; } Pool08854950;
extern void *func_0885C6CC(void *heap, int size, int a, int b, int one, int c);
void func_08854950(Pool08854950 *p, void *heap)
{
    void *h = heap;
    if (p->enabled != 0) {
        Kept<Pool08854950 *> saved(p);
        p->heap = h;
        int one = 1;
        void *items = func_0885C6CC(h, 0xF00, 0, 0, one, 0);
        p = saved.value;
        p->items = (PoolItem08854950 *)items;
        int i;
        for (i = 0; i < 40; i++) {
            p->items[i].weight = 0.0f;
        }
    }
    p->f10 = 0;
    p->fC = 0;
}
}
