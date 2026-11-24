/* Runs the current step of a 4-entry member-function table (state in field_140->field_984) until a step
   returns 0, then calls the global completion callback if set. */
struct MemberStep {
    short delta;
    short index;
    unsigned int target;
};

class LoopFA6DCState {
public:
    char pad_000[0xB3C];
    int state;
};

class LoopFA6DCScreen {
public:
    char pad_000[0x140];
    LoopFA6DCState *field_140;
};

extern "C" {
void func_08935524(void *dst, const void *src, int size);
extern const MemberStep D_08B01CFC[11];
extern struct BlkFA6DC { char g[0x9C4]; void (*cb)(int); int arg; } D_09B6CDA0;

void func_08AFA6DC(LoopFA6DCScreen *self)
{
    MemberStep table[11];
    const MemberStep *step;
    unsigned char *object;
    unsigned int target;
    int index;
    int delta;
    int result;
    func_08935524(table, D_08B01CFC, sizeof(table));
    do {
        step = &table[self->field_140->state];
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
    if (D_09B6CDA0.cb != 0) {
        D_09B6CDA0.cb(D_09B6CDA0.arg);
    }
}
}
