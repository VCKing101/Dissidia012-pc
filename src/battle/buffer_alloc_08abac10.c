typedef struct BufferOwner {
    char pad_00[0x8];
    void *buffer;
} BufferOwner;

extern void *func_0899B9F0(int heap);
extern void func_0899BB68(void *buffer, void *data, int size, int flags);

int func_08ABAC10(BufferOwner *self, void *data, int size)
{
    if (data == 0 || size <= 0) {
        return -1;
    }
    self->buffer = func_0899B9F0(0);
    if (self->buffer == 0) {
        return -2;
    }
    func_0899BB68(self->buffer, data, size, 0);
    return 0;
}
