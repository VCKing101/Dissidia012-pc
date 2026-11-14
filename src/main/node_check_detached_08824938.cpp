/* Runs the node's release step once it no longer has attached parts (or,
   for an actor in mode 1, when the actor asks for it). */
struct NodePart08824938 {
    char pad_000[0x12B];
    signed char flags;
    char pad_12C[4];
};

struct Mode08824938 {
    char pad_00[0x14];
    int kind;
};

struct Actor08824938 {
    char pad_000[0x184];
    unsigned int flags;
    char pad_188[0x568 - 0x188];
    Mode08824938 *mode;

    static bool releasing(Actor08824938 *a) { return (a->flags & 2) != 0; }
};

struct NodeFlags08824938 {
    unsigned int bits : 5;
    signed int parts : 8;
};

struct Node08824938 {
    char pad_00[0x20];
    NodeFlags08824938 flags;
    Actor08824938 *owner;
    NodePart08824938 *parts;
};

extern "C" {

void func_08823384(Node08824938 *self);

void func_08824938(Node08824938 *self)
{
    Actor08824938 *a = self->owner;
    if (a->mode->kind == 1) {
        if (Actor08824938::releasing(a) == true) {
            func_08823384(self);
        }
    } else {
        bool detached = true;
        int count = self->flags.parts;
        for (int i = 0; i < count; i++) {
            if (!(self->parts[i].flags & 2)) {
                detached = false;
                break;
            }
        }
        if (detached == true) {
            func_08823384(self);
        }
    }
}

}
