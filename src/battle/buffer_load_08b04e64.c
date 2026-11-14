typedef struct BufferLoadC {
    char pad_00[0xC];
    char field_0C[0x4];
} BufferLoadC;

extern void func_08B04D10(BufferLoadC *self);
extern int func_088908E0(void);
extern int func_0887AC70(void *buffer, void *data, int size, int heap);

int func_08B04E64(BufferLoadC *self, void *data, int size)
{
    func_08B04D10(self);
    if (func_0887AC70(self->field_0C, data, size, func_088908E0()) == 0) {
        return -1;
    }
    return 0;
}
