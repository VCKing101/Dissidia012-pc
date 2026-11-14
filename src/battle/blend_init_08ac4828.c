typedef struct BlendState {
    int enabled;
    int field_04;
    float weight;
    char pad_0C[0x24];
    int field_30;
    int field_34;
    int field_38;
} BlendState;

BlendState *func_08AC4828(BlendState *self)
{
    self->enabled = 1;
    self->field_34 = 0;
    self->field_38 = 0;
    self->field_30 = 0;
    self->field_04 = 0;
    self->weight = 0.0f;
    return self;
}
