typedef struct BufferLoad {
    char pad_00[0x8];
    char field_08[0x4];
} BufferLoad;

extern void func_089EA40C(BufferLoad *self);
extern int func_088908E0(void);
extern int func_0887AC70(void *buffer, void *data, int size, int heap);

int func_089EA388(BufferLoad *self, void *data, int size)
{
    func_089EA40C(self);
    if (data == 0 || size <= 0) {
        return -1;
    }
    if (func_0887AC70(self->field_08, data, size, func_088908E0()) == 0) {
        return -2;
    }
    return 0;
}
