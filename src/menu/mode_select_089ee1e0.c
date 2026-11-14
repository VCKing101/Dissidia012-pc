typedef struct ModeHolder {
    int field_00;
    char pad_04[0x10];
    int field_14;
} ModeHolder;

extern void func_088D9CFC(int handle);
extern void func_088D9CD8(int handle);

void func_089EE1E0(ModeHolder *self, int mode)
{
    self->field_14 = mode;
    switch (mode) {
    case 0:
        func_088D9CFC(self->field_00);
        break;
    case 1:
        func_088D9CD8(self->field_00);
        break;
    }
}
