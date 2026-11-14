typedef struct Vec4 {
    float x;
    float y;
    float z;
    float w;
} Vec4;

typedef struct ShotState {
    unsigned char active;
    char pad_01[0x3];
    void *link;
    int field_08;
    char pad_0C[0x4];
    Vec4 from;
    Vec4 to;
    Vec4 start;
    Vec4 pos;
    char pad_50[0x4C];
    unsigned char hit;
    unsigned char missed;
    char pad_9E[0x46];
    unsigned char flags[5];
} ShotState;

extern void func_08A958C0(ShotState *self);
extern void func_089A3868(Vec4 *to, Vec4 *from);
extern void func_08A95CC4(ShotState *self);

/* Restarts the shot from origin; -1 inactive. */
int func_08A95B64(ShotState *self, Vec4 *origin)
{
    if (self->active == 0) {
        return -1;
    }
    if (self->link != 0) {
        func_08A958C0(self);
    }
    self->link = 0;
    self->field_08 = 0;
    func_089A3868(&self->pos, &self->start);
    func_089A3868(&self->from, origin);
    func_089A3868(&self->to, origin);
    self->hit = 0;
    self->missed = 0;
    self->flags[0] = 0;
    self->flags[1] = 0;
    self->flags[2] = 0;
    self->flags[3] = 0;
    self->flags[4] = 0;
    func_08A95CC4(self);
    return 0;
}
