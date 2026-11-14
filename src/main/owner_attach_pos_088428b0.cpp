/* Gets the attachment point of an object's owner. */
struct Vec4_088428B0 {
    float x, y, z, w;
};

struct Point088428B0 {
    float x, y, z, w;

    Point088428B0(const Vec4_088428B0 &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
};

struct Vec3_088428B0 {
    float x, y, z;
};

struct Actor088428B0;

struct Object088428B0 {
    char pad_00[0x10];
    Actor088428B0 *owner;
};

extern "C" {

Vec4_088428B0 func_08838D14(Actor088428B0 *actor);

void func_088428B0(Object088428B0 *self, Vec3_088428B0 *out)
{
    Vec4_088428B0 v;
    v = func_08838D14(self->owner);
    Point088428B0 p(v);
    out->x = p.x;
    out->y = p.y;
    out->z = p.z;
}

}
