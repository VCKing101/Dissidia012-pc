typedef struct GridSetup {
    int field_00;
    char pad_04[0x48];
    int field_4C;
    int field_50;
} GridSetup;

extern void func_08A5C854(GridSetup *self, int x, int y, int width, int height, int gap);
extern void func_08A5C9E0(GridSetup *self, int mode);
extern void func_08A5CAE0(GridSetup *self);

void func_08A5C64C(GridSetup *self, int kind, int count, int x, int y, int width, int height, int gap, int mode)
{
    self->field_4C = kind;
    self->field_50 = count;
    func_08A5C854(self, x, y, width, height, gap);
    func_08A5C9E0(self, mode);
    func_08A5CAE0(self);
    self->field_00 = 1;
}
