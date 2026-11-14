typedef struct Vec3 {
    float x, y, z;
} Vec3;

typedef struct ShotState {
    unsigned char active;
    unsigned char field_01;
    unsigned char field_02;
    char pad_03[0x1];
    int field_04;
    int field_08;
    Vec3 field_0C;
    Vec3 field_18;
    int field_24;
    int field_28;
    float field_2C;
    int field_30;
    int field_34;
    unsigned char done;
    char pad_39[0x3];
    int field_3C;
    int field_40;
    int field_44;
} ShotState;

int func_08AA5C44(ShotState *self)
{
    if (self->active == 0) {
        return -1;
    }
    self->field_02 = 0;
    self->field_04 = 0;
    self->done = 0;
    self->field_40 = 0;
    self->field_44 = 0;
    return 0;
}
