typedef struct MotionFollow {
    char pad_00[0x8];
    int state;
    unsigned char ready;
    char pad_0D[0x3];
    float elapsed;
    char field_14[0x4];
    char pad_18[0x24];
    char field_3C[0x4];
    char pad_40[0x110];
    int field_150;
} MotionFollow;

extern char D_08A18C94[];
extern float func_0889140C(int unit);
extern int func_08A22DEC(float time);
extern void func_08A19080(void *sub, int frame);
extern void func_08A01BC8(void *sub, int mode, int count);
extern void func_08A19038(void *sub, int mode, void *data);

int func_08A18764(MotionFollow *self)
{
    float time = func_0889140C(self->field_150);
    int frame = func_08A22DEC(time);
    func_08A19080(self->field_3C, frame);
    if (self->ready == 1) {
        func_08A01BC8(self->field_14, 1, -1);
        func_08A19038(self->field_3C, 1, D_08A18C94);
        self->state = 2;
        self->elapsed = 0.0f;
    }
    return 0;
}
