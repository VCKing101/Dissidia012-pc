typedef struct Vec4 {
    float x, y, z, w;
} Vec4;

typedef struct LinkPoint {
    unsigned char active;
    char pad_01[0xF];
    Vec4 position;
    int field_20;
    unsigned char field_24;
} LinkPoint;

extern void func_089A3890(Vec4 *v);

int func_08A86C1C(LinkPoint *self)
{
    if (self->active == 0) {
        return -1;
    }
    func_089A3890(&self->position);
    self->field_20 = 0;
    self->field_24 = 0;
    return 0;
}
