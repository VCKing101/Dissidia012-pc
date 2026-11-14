typedef struct LazyOwner {
    char pad_00[0x20];
    unsigned char ready;
    float time;
    char data[0x1C];
} LazyOwner;

extern void func_08935828(void *dst, int value, int size);

int func_08AC9468(LazyOwner *self)
{
    self->ready = 0;
    self->time = 0.0f;
    func_08935828(self->data, 0, 0x1C);
    return 0;
}
