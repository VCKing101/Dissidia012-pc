typedef struct GuardTask {
    void *unit;
    int state;
    float field_08;
    float field_0C;
    float field_10;
    int pad_14;
    float field_18;
    unsigned char flag_1C;
    void *handles[2];
    int field_28;
} GuardTask;

extern "C" {

GuardTask *func_08ACA490(GuardTask *self, void *unit)
{
    self->unit = unit;
    self->state = 0;
    self->field_10 = 0.0f;
    self->field_18 = 0.0f;
    self->field_0C = 0.0f;
    self->field_08 = 0.0f;
    for (int i = 0; i < 2; i++) {
        self->handles[i] = 0;
    }
    self->flag_1C = 0;
    self->field_28 = 0;
    return self;
}

}
