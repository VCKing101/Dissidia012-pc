typedef struct MotionQuery {
    char pad_000[0x8];
    float frame;
    char pad_00C[0x20];
} MotionQuery;

typedef struct MotionProgress29C30 {
    char pad_000[0x8];
    int state;
    char pad_00C[0x10];
    char field_1C[0x28];
    char field_44[0xB0];
    int field_F4;
    int field_F8;
    int field_FC;
} MotionProgress29C30;

extern int func_08A22E2C(MotionQuery *out, int a, int b, int kind);
extern float func_08891348(int id);
extern void func_08A16CC4(void *sub, float rate);
extern void func_08A01C74(void *sub, int kind);
extern void func_08A16C58(void *sub);

int func_08A29C30(MotionProgress29C30 *self)
{
    MotionQuery query;
    if (func_08A22E2C(&query, self->field_F4, self->field_F8, 0x21) == 0) {
        float length = func_08891348(self->field_FC);
        if (length != 0.0f) {
            func_08A16CC4(self->field_44, query.frame / length);
        }
    } else {
        func_08A01C74(self->field_1C, 3);
        func_08A16C58(self->field_44);
        self->state = 4;
    }
    return 0;
}
