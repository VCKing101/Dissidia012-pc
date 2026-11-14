/* Tests whether two hit cylinders overlap as ellipsoids: their offset is
   scaled by the summed radii and heights and must be shorter than one. */
struct Vec3_08812054 {
    float x, y, z;
};

struct Vec4_08812054 {
    float x, y, z, w;

    Vec4_08812054() {}
    Vec4_08812054(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
};

struct Cylinder08812054 {
    int kind;
    float radius;
    float half_height;
    int pad_0C;
    Vec3_08812054 pos;
};

extern "C" {

void func_089A3528(Vec3_08812054 *d, const Vec3_08812054 *a, const Vec3_08812054 *b);
void func_089A355C(Vec3_08812054 *d, const Vec3_08812054 *a, const Vec3_08812054 *b);
void func_089A34C0(Vec3_08812054 *d, const Vec3_08812054 *src);
float func_089A3B10(const Vec4_08812054 *a, const Vec4_08812054 *b);

}

struct Vec08812054 {
    static Vec3_08812054 sub(const Vec3_08812054 &a, const Vec3_08812054 &b)
    {
        Vec3_08812054 r;
        func_089A3528(&r, &a, &b);
        return r;
    }
    static Vec3_08812054 mul(const Vec3_08812054 &a, const Vec3_08812054 &b)
    {
        Vec3_08812054 r;
        func_089A355C(&r, &a, &b);
        return r;
    }
};

extern "C" {

unsigned char func_08812054(Cylinder08812054 *self, Cylinder08812054 *other)
{
    Vec3_08812054 scaled;
    Vec3_08812054 d;
    Vec4_08812054 p __attribute__((aligned(16)));
    Vec4_08812054 q __attribute__((aligned(16)));
    Vec3_08812054 v;
    Vec3_08812054 scale;
    float one = 1.0f;
    float r_inv = one / (self->radius + other->radius);
    float h_inv = one / (self->half_height + other->half_height);
    scale.z = r_inv;
    scale.x = r_inv;
    scale.y = h_inv;
    scaled = Vec08812054::mul(d = Vec08812054::sub(other->pos, self->pos), scale);
    func_089A34C0(&v, &scaled);
    p.x = v.x;
    p.y = v.y;
    p.z = v.z;
    p.w = one;
    q.x = v.x;
    q.y = v.y;
    q.z = v.z;
    q.w = one;
    float dist = func_089A3B10(&p, &q);
    int r = dist < one;
    return r;
}

}
