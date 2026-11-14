typedef struct SetupSource {
    char pad_00[0xC];
    int field_0C;
} SetupSource;

typedef struct SetupObj {
    char pad_00[4];
    int field_04;
    char pad_08[0x70 - 8];
    int field_70;
    char pad_74[0x8C - 0x74];
    SetupSource *field_8C;
} SetupObj;

extern void func_08A528C8(SetupObj *self, int a, int b, int c, int d);
extern void func_08A52A4C(SetupObj *self, int value);
extern void func_08A5C3F4(void *target, SetupSource *source);

void func_08A524A0(SetupObj *self, SetupSource *source, int a, int b, int c, int d, int mode)
{
    self->field_8C = source;
    self->field_70 = mode;
    func_08A5C3F4((char *)self + 0x74, source);
    func_08A5C3F4((char *)self + 0x7C, (SetupSource *)((char *)self->field_8C + 0xC));
    func_08A528C8(self, a, b, c, d);
    func_08A52A4C(self, mode);
    self->field_04 = 1;
}
