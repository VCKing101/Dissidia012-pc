/* Moves each attached part of an effect node: works out its step since
   the last frame (flat when the node is grounded), keeps the new position
   and carries the step through the given matrix. */
struct Vec4_08823954 {
    float x, y, z, w;
};

struct NodePart08823954 {
    char pad_000[0x30];
    Vec4_08823954 pos;
    char pad_040[0x80 - 0x40];
    Vec4_08823954 last_pos;
    char pad_090[0xB0 - 0x90];
    Vec4_08823954 step;
    char pad_0C0[0x12B - 0xC0];
    signed char flags;
    char pad_12C[4];
};

struct NodeFlags08823954 {
    unsigned int bits : 5;
    signed int parts : 8;
};

struct Node08823954 {
    char pad_00[0x20];
    NodeFlags08823954 flags;
    void *owner;
    NodePart08823954 *parts;
};

extern "C" {

void func_089A38FC(Vec4_08823954 *dst, const Vec4_08823954 *a, const Vec4_08823954 *b);
void func_089A3868(Vec4_08823954 *dst, const Vec4_08823954 *src);
int func_08822D30(Node08823954 *self);
void func_089A3F68(Vec4_08823954 *dst, const void *m, const Vec4_08823954 *v);

void func_08823954(Node08823954 *self, const void *m)
{
    int count = self->flags.parts;
    for (int i = 0; i < count; i++) {
        NodePart08823954 *p = &self->parts[i];
        if (p->flags & 2) {
            continue;
        }
        Vec4_08823954 step;
        func_089A38FC(&step, &p->pos, &p->last_pos);
        func_089A3868(&p->last_pos, &p->pos);
        if (func_08822D30(self) == 1) {
            step.y = 0.0f;
        }
        func_089A3F68(&p->step, m, &step);
    }
}

}
