typedef struct BufferCopy21E28 {
    unsigned char active;
} BufferCopy21E28;

extern void *func_08A21DAC(BufferCopy21E28 *self, int size);
extern void func_08935524(void *dst, void *src, int size);

int func_08A21E28(BufferCopy21E28 *self, void *dst, int size)
{
    void *src;
    if (!self->active) {
        return -1;
    }
    if (!dst || size <= 0) {
        return -2;
    }
    src = func_08A21DAC(self, size);
    if (!src) {
        return -3;
    }
    func_08935524(dst, src, size);
    return 0;
}
