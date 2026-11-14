typedef struct Track { float keys[8]; char pad_20[0x214]; int field_234; unsigned char field_238; } Track;
Track *func_08873880(Track *self)
{
    unsigned char one = 1;
    self->keys[0] = 0.0f;
    self->keys[1] = 0.0f;
    self->keys[2] = 0.0f;
    self->keys[3] = 0.0f;
    self->keys[4] = 0.0f;
    self->keys[5] = 0.0f;
    self->keys[6] = 0.0f;
    self->keys[7] = 0.0f;
    self->field_234 = 0;
    self->field_238 = one;
    return self;
}
