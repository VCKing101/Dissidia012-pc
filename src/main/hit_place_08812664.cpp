/* Fills a hit entry from one of an actor's hit parts: the part's sizes,
   and the part's offset carried through its bone into world space. */
struct Vec3_08812664 {
    float x, y, z;
};

struct Vec4_08812664 {
    float x, y, z, w;

    Vec4_08812664(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
    Vec3_08812664 xyz() const { return *(const Vec3_08812664 *)this; }
};

struct Matrix08812664 {
    float m[16] __attribute__((aligned(16)));
};

struct Part08812664 {
    char pad_00[5];
    unsigned char id;
    unsigned short bone;
    float ox, oy, oz;
    float drop;
    float radius;
    float half_height;
};

struct Place08812664 {
    int id;
    float radius;
    float half_height;
    Part08812664 *part;
    Vec3_08812664 pos;
};

struct Actor08812664;

extern "C" {

void func_088369A4(Actor08812664 *actor, Matrix08812664 *m, int bone);
void func_089A3F40(Vec4_08812664 *dst, const Matrix08812664 *m, const Vec4_08812664 *src);
void func_089A34C0(Vec3_08812664 *dst, const Vec3_08812664 *src);

void func_08812664(Place08812664 *out, Actor08812664 *actor, Part08812664 *part)
{
    Vec3_08812664 at;
    Matrix08812664 m;
    out->id = part->id;
    out->part = part;
    out->radius = part->radius;
    out->half_height = part->half_height;
    Vec4_08812664 p(part->ox, part->oy, part->oz, 1.0f);
    func_088369A4(actor, &m, part->bone);
    func_089A3F40(&p, &m, &p);
    at = p.xyz();
    func_089A34C0(&out->pos, &at);
    out->pos.y -= part->drop;
}

}
