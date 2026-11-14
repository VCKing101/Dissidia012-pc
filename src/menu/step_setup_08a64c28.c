typedef struct StepSetup {
    char pad_00[0x10];
    int field_10;
    char pad_14[0x54];
    unsigned int field_68;
    char pad_6C[0x28];
    int field_94;
} StepSetup;

extern void func_088D46E4(unsigned int *anim, int a, int b, int c, int d, int e);
extern void func_08A651A8(StepSetup *self, int x, int y, int width, int height);
extern void func_08A65344(StepSetup *self, int mode);

void func_08A64C28(StepSetup *self, int kind, int x, int y, int width, int height, int mode)
{
    self->field_94 = kind;
    func_088D46E4(&self->field_68, 0, 0, 1, 0, -1);
    self->field_68 = self->field_68 | 1;
    func_08A651A8(self, x, y, width, height);
    func_08A65344(self, mode);
    self->field_10 = 1;
}
