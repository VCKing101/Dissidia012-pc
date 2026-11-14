/* Copies each attached part's bone matrix (identity when the bone is
   missing); detached parts keep their own. */
struct NodePart08823870 {
    float matrix[16];
    char pad_040[0x128 - 0x40];
    unsigned char bone;
    char pad_129[2];
    signed char flags;
    char pad_12C[4];
};

struct Body08823870 {
    char pad_0[0xC];
    void *skeleton;
};

struct Actor08823870 {
    char pad_000[0x394];
    Body08823870 *body;
};

struct NodeFlags08823870 {
    unsigned int bits : 5;
    signed int parts : 8;
};

struct Node08823870 {
    char pad_00[0x20];
    NodeFlags08823870 flags;
    Actor08823870 *owner;
    NodePart08823870 *parts;
};

extern "C" {

void *func_089B0E78(void *skeleton, int index);
float *func_089B1224(void *bone);
void func_089A3EBC(NodePart08823870 *m);
void func_089A3EFC(NodePart08823870 *m, float *src);

void func_08823870(Node08823870 *self)
{
    void *skeleton = self->owner->body->skeleton;
    int count = self->flags.parts;
    for (int i = 0; i < count; i++) {
        NodePart08823870 *p = &self->parts[i];
        if (p->flags & 2) {
            continue;
        }
        void *bone = func_089B0E78(skeleton, p->bone);
        if (bone == 0) {
            func_089A3EBC(p);
        } else {
            func_089A3EFC(p, func_089B1224(bone));
        }
    }
}

}
