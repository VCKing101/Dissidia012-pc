/* Replaces the object's buffer with a heap copy of src (size bytes); returns 0, -1 or -3. */
typedef struct Buf38 { unsigned char active; char pad[3]; int size; void *buf; } Buf38;
typedef struct Heap38 { char pad[8]; void *heap; } Heap38;
extern void func_08A81EDC(Buf38 *self);
extern void *func_0885C6CC(void *heap, int size, int align, int a, int b, int c);
extern void func_08935524(void *dst, void *src, int size);
extern Heap38 *D_089D4958;
int func_08A81F20(Buf38 *self, void *src, int size)
{
    void *mem;
    func_08A81EDC(self);
    if (src == 0 || size <= 0) {
        return -1;
    }
    mem = self->buf = func_0885C6CC(D_089D4958->heap, size, 0x40, 0, 1, 0);
    if (mem == 0) {
        return -3;
    }
    func_08935524(mem, src, size);
    self->size = size;
    self->active = 1;
    return 0;
}
