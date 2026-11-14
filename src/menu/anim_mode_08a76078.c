typedef struct ModeAnim {
    char pad_00[0x78];
    int field_78;
} ModeAnim;

extern void func_088D319C(ModeAnim *self);
extern void func_088D3174(ModeAnim *self);

void func_08A76078(ModeAnim *self, int mode)
{
    switch (mode) {
    case 0:
        func_088D319C(self);
        break;
    case 1:
        func_088D3174(self);
        break;
    }
    self->field_78 = mode;
}
