/* Opens a data buffer: allocates room for the data and copies it in. */
struct DataBuffer {
    unsigned char open;
    unsigned char pad_01[3];
    int size;
    void *data;
};

extern void func_08A21CE4(struct DataBuffer *buffer);
extern void *func_08890900(int size);
extern void func_08935524(void *dest, void *src, int size);

int func_08A21D20(struct DataBuffer *buffer, void *data, int size)
{
    func_08A21CE4(buffer);
    if (data == 0 || size <= 0) {
        return -1;
    }
    buffer->data = func_08890900(size);
    if (buffer->data == 0) {
        return -2;
    }
    func_08935524(buffer->data, data, size);
    buffer->size = size;
    buffer->open = 1;
    return 0;
}
