/* Runs the current step of a 4-entry member-function table (state in field_138->field_984) until a step
   returns 0, then calls the global completion callback if set. */
struct MemberStep {
    short delta;
    short index;
    unsigned int target;
};

class Loop23C00State {
public:
    char pad_000[0x984];
    int state;
};

class Loop23C00Screen {
public:
    char pad_000[0x138];
    Loop23C00State *field_138;
};

extern "C" {
void func_08935524(void *dst, const void *src, int size);
extern const MemberStep D_08B243C4[4];
extern struct Blk23C00 { char g[4]; void (*cb)(int); int arg; } D_09B6DA58;

void func_08B23C00(Loop23C00Screen *self)
{
    MemberStep table[4];
    const MemberStep *step;
    unsigned char *object;
    unsigned int target;
    int index;
    int delta;
    int result;
    func_08935524(table, D_08B243C4, sizeof(table));
    do {
        step = &table[self->field_138->state];
        delta = step->delta;
        index = step->index;
        object = (unsigned char *)self + delta;
        target = step->target;
        if (index) {
            step = (const MemberStep *)*(void **)(object + target) + index;
            object += step->delta;
            target = step->target;
        } else {
            target = target;
        }
        result = ((int (*)(void *))target)(object);
    } while (result != 0);
    if (D_09B6DA58.cb != 0) {
        D_09B6DA58.cb(D_09B6DA58.arg);
    }
}
}
