/* Runs the current step of a 4-entry member-function table copied from .data (state in
   field_D80->field_C80). The member call is written out the way SNC lowers it: delta, vtable
   index and target are read first, then `this` is adjusted and, for a virtual entry, the vtable
   entry replaces the target. */
struct MemberStep {
    short delta;          /* added to the object pointer */
    short index;          /* vtable index; 0 for a non-virtual member */
    unsigned int target;  /* function address, or the vptr offset when index != 0 */
};

class State7600 {
public:
    char pad_000[0xC80];
    int field_C80;
    char pad_C84[0x4];
    int field_C88;
};

class Screen7600 {
public:
    char pad_000[0xD80];
    State7600 *field_D80;
};

extern "C" void func_08935524(void *dst, const void *src, int size);
extern "C" const MemberStep D_08A18938[4];

extern "C" {
int func_08A17600(Screen7600 *self)
{
    MemberStep table[4];
    const MemberStep *step;
    unsigned char *object;
    unsigned int target;
    int index;
    int delta;
    void (*fn)(void *);
    func_08935524(table, D_08A18938, sizeof(table));
    step = &table[self->field_D80->field_C80];
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
    return self->field_D80->field_C88;
}
}
