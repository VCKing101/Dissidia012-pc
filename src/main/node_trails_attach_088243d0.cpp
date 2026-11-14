/* Moves the source point of every trail attached to an effect node's
   parts into world space through the part's world matrix. */
struct Vec4_088243D0 {
    float x, y, z, w;

    Vec4_088243D0() {}
    Vec4_088243D0(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
};

struct Matrix088243D0 {
    Vec4_088243D0 x, y, z, pos;
} __attribute__((aligned(16)));

struct Anchor088243D0 {
    char pad_00[8];
    float x, y, z;
};

struct Trail088243D0 {
    Vec4_088243D0 source;
    char pad_10[0x70 - 0x10];
    Anchor088243D0 *anchor;
    char pad_74[0x80 - 0x74];
};

struct NodePart088243D0 {
    Matrix088243D0 bone;
    Matrix088243D0 world;
    char pad_080[0x129 - 0x80];
    signed char first_trail;
    signed char trails;
    signed char flags;
    char pad_12C[4];
};

struct NodeFlags088243D0 {
    unsigned int bits : 5;
    signed int parts : 8;
};

struct Node088243D0 {
    char pad_00[0x20];
    NodeFlags088243D0 flags;
    void *frame;
    NodePart088243D0 *parts;
    Trail088243D0 *trails;
};

extern "C" {

void func_089A3F40(Vec4_088243D0 *dst, const Matrix088243D0 *m, const Vec4_088243D0 *v);

void func_088243D0(Node088243D0 *self)
{
    int count = self->flags.parts;
    for (int i = 0; i < count; i++) {
        NodePart088243D0 *p = &self->parts[i];
        if (p->flags & 2) {
            continue;
        }
        int trails = p->trails & 0x3F;
        int first = p->first_trail & 0x7F;
        for (int j = 0; j < trails; j++) {
            Trail088243D0 *t = &self->trails[first + j];
            Anchor088243D0 *a = t->anchor;
            Vec4_088243D0 v(a->x, a->y, a->z, 1.0f);
            func_089A3F40(&t->source, &p->world, &v);
        }
    }
}

}
