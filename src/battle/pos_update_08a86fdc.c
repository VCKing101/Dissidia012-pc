typedef struct Vec4 {
    float x;
    float y;
    float z;
    float w;
} Vec4;

typedef struct PosTracker {
    unsigned char active;
    char pad_01[0xF];
    Vec4 pos;
    char pad_20[0x60];
    char field_80[0x4];
} PosTracker;

extern void func_08A86C1C(void *sub);
extern void func_089A3868(Vec4 *dest, Vec4 *src);

int func_08A86FDC(PosTracker *self, Vec4 *pos)
{
    if (!self->active) {
        return -1;
    }
    if (self->pos.x != pos->x || self->pos.y != pos->y || self->pos.z != pos->z) {
        func_08A86C1C(self->field_80);
    }
    func_089A3868(&self->pos, pos);
    return 0;
}
