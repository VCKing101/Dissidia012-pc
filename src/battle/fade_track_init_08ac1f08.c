typedef struct FadeTrackState {
    unsigned char active;
    float time;
    float speed;
    int field_0C;
    float scale;
    unsigned char flag_14;
    int index;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
} FadeTrackState;

void func_08AC1F08(FadeTrackState *self)
{
    self->active = 0;
    self->time = 0.0f;
    self->speed = 0.0f;
    self->field_0C = 0;
    self->scale = 0.0f;
    self->flag_14 = 0;
    self->index = -1;
    self->field_1C = 0;
    self->field_20 = 0;
    self->field_24 = 0;
    self->field_28 = 0;
    self->field_2C = 0;
    self->field_30 = 0;
}
