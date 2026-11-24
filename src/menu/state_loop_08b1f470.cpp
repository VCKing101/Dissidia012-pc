/* Runs the current step of a 4-entry member-function table (state in field_138->field_984) until a step
   returns 0, then calls the global completion callback if set. */
struct MemberStep {
    short delta;
    short index;
    unsigned int target;
};

class Loop1F470State {
public:
    char pad_000[0x80EC];
    int state;
};

class Loop1F470Screen {
public:
    char pad_000[0x138];
    Loop1F470State *field_138;
};

extern "C" {
void func_08935524(void *dst, const void *src, int size);
extern const MemberStep D_08B22E9C[4];
extern struct Blk1F470 { char g[8]; void (*cb)(int); int arg; } D_09B6DA30;

void func_08B1F470(Loop1F470Screen *self)
{
    MemberStep table[4];
    const MemberStep *step;
    unsigned char *object;
    unsigned int target;
    int index;
    int delta;
    int result;
    func_08935524(table, D_08B22E9C, sizeof(table));
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
    if (D_09B6DA30.cb != 0) {
        D_09B6DA30.cb(D_09B6DA30.arg);
    }
}
}
