typedef struct BlendTimer {
    int field_00;
    int field_04;
    int field_08;
    float field_0C;
    float field_10;
    unsigned char field_14;
    unsigned char field_15;
} BlendTimer;

void func_08B2CE04(BlendTimer *timer)
{
    timer->field_15 = 0;
    timer->field_14 = 0;
    timer->field_08 = 0;
    timer->field_04 = 0;
    timer->field_00 = 0;
    timer->field_0C = 0.0f;
    timer->field_10 = 0.0f;
}
