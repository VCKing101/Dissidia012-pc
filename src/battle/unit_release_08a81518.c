typedef struct UnitLink {
    char pad_00[0x4];
    int id;
    int handle;
    char pad_0C[0x58];
    int field_64;
} UnitLink;

extern void *func_08AB32B8(int id);
extern int func_08B30200(int id, int arg);
extern int func_08A96BAC(int handle, int mode);

int func_08A81518(UnitLink *self)
{
    if (func_08AB32B8(self->id) == 0) {
        return -1;
    }
    if (func_08B30200(self->id, 0) == 0) {
        return 0;
    }
    if (func_08A96BAC(self->handle, 0) != 0) {
        return -2;
    }
    self->field_64 = 0;
    return 0;
}
