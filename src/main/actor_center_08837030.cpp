/* The centre of an actor's collision box in world space (a direction
   vector, w = 0); without a box, the origin as a point. */
struct Vec4_08837030 {
    float x, y, z, w;
};

struct Box08837030 {
    char pad_00[8];
    float x;
    float y;
    float z;
    float height;
};

struct Actor08837030 {
    char pad_000[0x80];
    Vec4_08837030 pos;
    char pad_090[0x3A0 - 0x90];
    Box08837030 *box;
};

extern "C" {

void func_089A3868(Vec4_08837030 *dst, Vec4_08837030 *src);
void func_089A3890(Vec4_08837030 *v);

Vec4_08837030 func_08837030(Actor08837030 *self)
{
    Vec4_08837030 v;
    if (self->box != 0) {
        func_089A3868(&v, &self->pos);
        Box08837030 *b = self->box;
        v.x += b->x;
        v.y = b->y + b->height + v.y;
        v.z += b->z;
        v.w = 0.0f;
    } else {
        func_089A3890(&v);
        v.w = 1.0f;
    }
    return v;
}

}
