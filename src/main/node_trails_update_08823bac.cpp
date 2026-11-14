/* Updates an effect node's ground trails: each trail follows its source
   point just above the ground, keeping the previous point when it rises
   more than one unit, for the head and (when looped) the tail. */
struct Vec4_08823BAC {
    float x, y, z, w;
};

struct Trail08823BAC {
    Vec4_08823BAC source;
    char pad_10[0x10];
    Vec4_08823BAC head_last;
    Vec4_08823BAC head;
    char pad_40[0x10];
    Vec4_08823BAC tail_last;
    Vec4_08823BAC tail;
    char pad_70[8];
    float ground;
    char pad_7C[3];
    signed char flags;
};

struct Node08823BAC {
    char pad_00[0x2C];
    Trail08823BAC *trails;
    signed char count;
};

extern "C" {

void func_089A3868(Vec4_08823BAC *dst, const Vec4_08823BAC *src);

void func_08823BAC(Node08823BAC *self)
{
    int count = self->count;
    for (int i = 0; i < count; i++) {
        Trail08823BAC *t = &self->trails[i];
        Vec4_08823BAC p;
        func_089A3868(&p, &t->source);
        p.y = t->ground - 0.01f;
        Vec4_08823BAC *head = &t->head;
        if (!(t->flags & 4)) {
            t->flags |= 4;
            func_089A3868(&t->head_last, &p);
            func_089A3868(head, &p);
            func_089A3868(&t->tail_last, &p);
            func_089A3868(&t->tail, &p);
        } else {
            if (t->head.y - p.y > 1.0f) {
                func_089A3868(&t->head_last, head);
            }
            func_089A3868(head, &p);
        }
        if (t->flags & 1) {
            Vec4_08823BAC *tail = &t->tail;
            if (t->tail.y - p.y > 1.0f) {
                func_089A3868(&t->tail_last, tail);
            }
            func_089A3868(tail, &p);
        }
    }
}

}
