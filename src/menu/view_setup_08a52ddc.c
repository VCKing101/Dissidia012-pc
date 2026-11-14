typedef struct ViewSetup {
    int field_00;
    char pad_04[0x70];
    int field_74;
    int field_78;
    int field_7C;
    unsigned char field_80;
} ViewSetup;

extern void func_08A52F38(ViewSetup *self, int width, int height, int depth);
extern void func_08A53124(ViewSetup *self, int mode);
extern void func_08A52E90(ViewSetup *self);

void func_08A52DDC(ViewSetup *self, int kind, int width, int height, int depth, int mode, unsigned char flag)
{
    self->field_74 = kind;
    self->field_78 = width;
    self->field_7C = mode;
    self->field_80 = flag;
    func_08A52F38(self, width, height, depth);
    func_08A53124(self, mode);
    func_08A52E90(self);
    self->field_00 = 1;
}
