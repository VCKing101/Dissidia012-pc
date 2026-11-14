/* Frees a heap block: steps back over the aligned header, unlinks it and
   updates the used total. */
typedef struct HeapBlock { struct HeapBlock *prev; struct HeapBlock *next; int size; } HeapBlock;
typedef struct Heap0885C538 { int field_00; HeapBlock *first; int used; int align; HeapBlock *cursor; } Heap0885C538;
void func_0885C538(Heap0885C538 *h, char *data, int align)
{
    HeapBlock *b;
    int def = h->align;
    int mask;
    if (align <= 0) {
        align = def;
    }
    mask = align - 1;
    b = (HeapBlock *)(data - ((mask + 0x10) & ~mask));
    if (h->cursor == b) {
        h->cursor = b->next;
    }
    h->used -= b->size;
    if (b->prev != h->first) {
        b->prev->next = b->next;
    }
    b->next->prev = b->prev;
}
