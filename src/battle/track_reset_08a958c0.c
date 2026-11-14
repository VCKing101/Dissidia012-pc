typedef struct Vec4 {
    float x;
    float y;
    float z;
    float w;
} Vec4;

typedef struct TrackReset {
    unsigned char active;
    char pad_01[0x3];
    int field_04;
    char pad_08[0x28];
    Vec4 field_30;
    char pad_40[0x2C];
    int field_6C;
    char pad_70[0x34];
    unsigned char field_A4;
} TrackReset;

extern void func_089A3890(Vec4 *v);

int func_08A958C0(TrackReset *self)
{
    int none = -1;
    if (!self->active) {
        return none;
    }
    self->field_04 = none;
    func_089A3890(&self->field_30);
    self->field_6C = none;
    self->field_A4 = 0;
    return 0;
}
