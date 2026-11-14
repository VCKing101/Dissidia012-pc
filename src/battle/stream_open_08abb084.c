typedef struct StreamOwner {
    char pad_00[0x60];
    unsigned char ready;
    char pad_61[0x53];
    char field_B4[0x4];
} StreamOwner;

extern void func_08ABAE78(StreamOwner *self);
extern int func_08ABA924(void *stream, void *header, void *data, int size);

int func_08ABB084(StreamOwner *self, void *data, int size)
{
    func_08ABAE78(self);
    if (data == 0 || size <= 0) {
        return -1;
    }
    if (!self->ready) {
        return -2;
    }
    if (func_08ABA924(self->field_B4, &self->ready, data, size) != 0) {
        return -3;
    }
    return 0;
}
