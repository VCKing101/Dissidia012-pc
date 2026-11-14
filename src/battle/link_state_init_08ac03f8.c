typedef struct LinkState {
    unsigned char active;
    char pad_01[0x3];
    char target_04[0xC];
    float weight;
    unsigned char flag_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    float scale;
    void *first;
    void *second;
    void *third;
} LinkState;

extern void func_089A34E0(void *target);

void func_08AC03F8(LinkState *self)
{
    self->active = 0;
    func_089A34E0(self->target_04);
    self->weight = 0.0f;
    self->flag_14 = 0;
    self->field_1C = 0;
    self->field_18 = 0;
    self->field_20 = 0;
    self->field_28 = 0;
    self->field_24 = 0;
    self->field_2C = 0;
    self->field_30 = 0;
    self->scale = 0.0f;
    self->first = 0;
    self->second = 0;
    self->third = 0;
}
