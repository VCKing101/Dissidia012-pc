/* Builds each attached part's world matrix: its spin about its own axis,
   then its bone matrix, moved to the part position and placed in the
   node's frame. */
struct Vec4_088242EC {
    float x, y, z, w;
};

struct Matrix088242EC {
    Vec4_088242EC x, y, z, pos;
} __attribute__((aligned(16)));

struct NodePart088242EC {
    Matrix088242EC bone;
    Matrix088242EC world;
    char pad_080[0x10];
    Vec4_088242EC pos;
    char pad_0A0[0x118 - 0xA0];
    float spin;
    char pad_11C[0x12B - 0x11C];
    signed char flags;
    char pad_12C[4];
};

struct NodeFlags088242EC {
    unsigned int bits : 5;
    signed int parts : 8;
};

struct Node088242EC {
    char pad_00[0x20];
    NodeFlags088242EC flags;
    Matrix088242EC *frame;
    NodePart088242EC *parts;
};

extern "C" {

void func_089A3EBC(Matrix088242EC *m);
void func_089A4124(Matrix088242EC *dst, const Matrix088242EC *src, float angle);
void func_089A3FB8(Matrix088242EC *dst, const Matrix088242EC *a, const Matrix088242EC *b);
void func_089A3868(Vec4_088242EC *dst, const Vec4_088242EC *src);

void func_088242EC(Node088242EC *self)
{
    int count = self->flags.parts;
    for (int i = 0; i < count; i++) {
        NodePart088242EC *p = &self->parts[i];
        if (p->flags & 2) {
            continue;
        }
        Matrix088242EC m;
        func_089A3EBC(&m);
        func_089A4124(&m, &m, p->spin);
        func_089A3FB8(&m, &m, &p->bone);
        func_089A3868(&m.pos, &p->pos);
        func_089A3FB8(&p->world, self->frame, &m);
    }
}

}
