typedef struct GuardTrack {
    unsigned char active;
    char pad_01[0xB];
    char track[0x14];
    unsigned char field_20;
    unsigned char field_21;
    char pad_22[0x2E];
    unsigned char field_50;
    unsigned char field_51;
} GuardTrack;

extern int func_08AB3848(void *track);

int func_08A9657C(GuardTrack *self)
{
    if (self->active == 0) {
        return -1;
    }
    if (func_08AB3848(self->track) != 0) {
        return -2;
    }
    self->field_21 = self->field_20;
    self->field_20 = 0;
    self->field_51 = self->field_50;
    self->field_50 = 0;
    return 0;
}
