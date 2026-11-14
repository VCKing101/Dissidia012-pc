extern float func_08891348(int);
extern void func_089F1F24(void *);

typedef struct Timer1A74 {
    char pad_000[0x8];
    int field_08;
    char pad_00C[4];
    float field_10;
    char field_14[0x18];
    int field_2C;
} Timer1A74;

int func_089F1A74(Timer1A74 *self, float dt)
{
    float limit = func_08891348(self->field_2C);
    self->field_10 += dt;
    if (self->field_10 >= limit) {
        func_089F1F24(self->field_14);
        self->field_08 = 3;
        self->field_10 = 0.0f;
    }
    return 0;
}
