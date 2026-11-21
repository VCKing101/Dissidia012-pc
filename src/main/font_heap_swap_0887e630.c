typedef struct FontHeap0887E630 {
    unsigned char unk_00[0x10];
    void *base;                        /* +0x10 */
    void *heap;                        /* +0x14 */
    void *prev_base;                   /* +0x18 */
    void *prev_heap;                   /* +0x1C */
} FontHeap0887E630;

extern char D_089C9DAC[];              /* "SYSTEM FONT T2" */
extern void *func_0885C40C(void *base, int size, const char *name, int unk);

void func_0887E630(FontHeap0887E630 *self, void *base, int size)
{
    self->prev_base = self->base;
    self->prev_heap = self->heap;
    self->base = base;
    self->heap = func_0885C40C(base, size, D_089C9DAC, 0x10);
}
