typedef struct BufferLoadB {
    char pad_00[0x4];
    char field_04[0x4];
} BufferLoadB;

extern void func_08AAD54C(BufferLoadB *self);
extern int func_08A81F20(void *buffer, void *data, int size);
extern int func_08AAD8F8(BufferLoadB *self);

int func_08AABD38(BufferLoadB *self, void *data, int size)
{
    func_08AAD54C(self);
    if (data == 0 || size <= 0) {
        return -1;
    }
    if (func_08A81F20(self->field_04, data, size) != 0) {
        return -2;
    }
    if (func_08AAD8F8(self) != 0) {
        return -3;
    }
    return 0;
}
