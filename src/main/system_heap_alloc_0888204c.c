/* The game's system object; only its main heap handle is used here. */
typedef struct System0888204C {
    char unk_000000[0xECDC00];
    void *heap;                        /* +0xECDC00 */
} System0888204C;

extern System0888204C *D_089D69A0;

/* heap_alloc(heap, size, align (0 = the heap's default), tag, one, from_end) */
extern void *func_0885C6CC(void *heap, int size, int align, int tag, int one, int from_end);

/* system_heap_alloc: a block from the system heap. The graphics object takes its
 * 0x780000-byte GRAPHICS::GRAPHICS heap from here. */
void *func_0888204C(int size)
{
    int zero = 0;
    void *heap = D_089D69A0->heap;
    int one = 1;

    return func_0885C6CC(heap, size, 0, zero, one, 0);
}
