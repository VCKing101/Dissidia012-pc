/* Runs the current step of a 4-entry member-function table copied from .data (state in
   field_17B0->field_10E8). The member call is written out the way SNC lowers it: delta, vtable
   index and target are read first, then `this` is adjusted and, for a virtual entry, the vtable
   entry replaces the target. */
struct MemberStep {
    short delta;          /* added to the object pointer */
    short index;          /* vtable index; 0 for a non-virtual member */
    unsigned int target;  /* function address, or the vptr offset when index != 0 */
};

class State96EC {
public:
    char pad_000[0x10E8];
    int field_10E8;
    char pad_10EC[0x4];
    int field_10F0;
};

class Screen96EC {
public:
    char pad_000[0x17B0];
    State96EC *field_17B0;
};

extern "C" void func_08935524(void *dst, const void *src, int size);
extern "C" const MemberStep D_08A1B1A8[4];

extern "C" {
int func_08A196EC(Screen96EC *self)
{
    MemberStep table[4];
    const MemberStep *step;
    unsigned char *object;
    unsigned int target;
    int index;
    int delta;
    void (*fn)(void *);
    func_08935524(table, D_08A1B1A8, sizeof(table));
    step = &table[self->field_17B0->field_10E8];
    delta = step->delta;
    index = step->index;
    target = step->target;
    object = (unsigned char *)self + delta;
    if (index) {
        step = (const MemberStep *)*(void **)(object + target) + index;
        object += step->delta;
        target = step->target;
    } else {
        target = target;
    }
    fn = (void (*)(void *))target;
    fn(object);
    return self->field_17B0->field_10F0;
}
}
