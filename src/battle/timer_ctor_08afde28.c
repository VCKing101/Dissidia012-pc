typedef struct TimerObject {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
    float field_10;
    int field_14;
    char pad_18[0x8];
    unsigned char field_20;
    char pad_21[0x3];
    const void *vtable;
} TimerObject;

extern const char D_08B00018[];

TimerObject *func_08AFDE28(TimerObject *self)
{
    self->vtable = D_08B00018;
    self->field_00 = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_14 = 0;
    self->field_20 = 0;
    return self;
}
