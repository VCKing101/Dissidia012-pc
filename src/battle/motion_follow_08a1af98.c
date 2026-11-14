typedef struct MotionFollow {
    char pad_00[0x8];
    int state;
    unsigned char ready;
    char pad_0D[0x3];
    float elapsed;
    char pad_14[0x8];
    char field_1C[0x4];
    char pad_20[0x24];
    char field_44[0x4];
    char pad_48[0x12C];
    int field_174;
} MotionFollow;

extern char D_08A1B4B8[];
extern float func_0889140C(int unit);
extern int func_08A22DEC(float time);
extern void func_08A19080(void *sub, int frame);
extern void func_08A01BC8(void *sub, int mode, int count);
extern void func_08A19038(void *sub, int mode, void *data);

int func_08A1AF98(MotionFollow *self)
{
    float time = func_0889140C(self->field_174);
    int frame = func_08A22DEC(time);
    func_08A19080(self->field_44, frame);
    if (self->ready == 1) {
        func_08A01BC8(self->field_1C, 1, -1);
        func_08A19038(self->field_44, 1, D_08A1B4B8);
        self->state = 2;
        self->elapsed = 0.0f;
    }
    return 0;
}
