typedef struct MotionQueryA3E4 {
    char pad_000[0x8];
    float frame;
    char pad_00C[0x20];
} MotionQueryA3E4;

typedef struct MotionProgress2A3E4 {
    char pad_000[0x8];
    int state;
    char pad_00C[0x8];
    char field_14[0x28];
    char field_3C[0x30];
    int field_6C;
    int field_70;
    int field_74;
} MotionProgress2A3E4;

extern int func_08A22E2C(MotionQueryA3E4 *out, int a, int b, int kind);
extern float func_08891348(int id);
extern void func_08A16CC4(void *sub, float rate);
extern void func_08A01C74(void *sub, int kind);
extern void func_08A16C58(void *sub);

int func_08A2A3E4(MotionProgress2A3E4 *self)
{
    MotionQueryA3E4 query;
    if (func_08A22E2C(&query, self->field_6C, self->field_70, 0x1F) == 0) {
        float length = func_08891348(self->field_74);
        if (length != 0.0f) {
            func_08A16CC4(self->field_3C, query.frame / length);
        }
    } else {
        func_08A01C74(self->field_14, 2);
        func_08A16C58(self->field_3C);
        self->state = 3;
    }
    return 0;
}
