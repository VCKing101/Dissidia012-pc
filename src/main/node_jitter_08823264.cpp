/* Gives every part of a node a random jitter in [-1, 1]. */
struct Rng08823264 {
    int seed;

    static int next(Rng08823264 *r)
    {
        r->seed = r->seed * 0x343FD + 0x269EC3;
        return (r->seed >> 16) & 0x7FFF;
    }
};

struct Part08823264 {
    char pad_000[0x11C];
    float jitter;
    char pad_120[0x10];
};

struct NodeFlags08823264 {
    unsigned int bits : 5;
    signed int parts : 8;
};

struct Node08823264 {
    char pad_00[0x20];
    NodeFlags08823264 flags;
    char pad_24[4];
    Part08823264 *parts;
};

extern "C" {

void *func_0881BAA8(void);
Rng08823264 *func_0881BA98(void *mgr);

void func_08823264(Node08823264 *self)
{
    for (int i = 0; i < self->flags.parts; i++) {
        Rng08823264 *r = func_0881BA98(func_0881BAA8());
        self->parts[i].jitter = (Rng08823264::next(r) % 10001 - 5000) / 5000.0f;
    }
}

}
