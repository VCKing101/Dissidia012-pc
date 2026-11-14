typedef struct BufferCopy82044 {
    unsigned char active;
} BufferCopy82044;

extern void *func_08A81FC8(BufferCopy82044 *self, int size);
extern void func_08935524(void *dst, void *src, int size);

int func_08A82044(BufferCopy82044 *self, void *dst, int size)
{
    void *src;
    if (!self->active) {
        return -1;
    }
    if (!dst || size <= 0) {
        return -2;
    }
    src = func_08A81FC8(self, size);
    if (!src) {
        return -3;
    }
    func_08935524(dst, src, size);
    return 0;
}
