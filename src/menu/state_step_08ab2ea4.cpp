/* Runs the current step of a 5-entry member-function table copied from .data (state in
   field_584->field_4). The member call is written out the way SNC lowers it: delta, vtable
   index and target are read first, then `this` is adjusted and, for a virtual entry, the vtable
   entry replaces the target. */
struct MemberStep {
    short delta;          /* added to the object pointer */
    short index;          /* vtable index; 0 for a non-virtual member */
    unsigned int target;  /* function address, or the vptr offset when index != 0 */
};

class State2EA4 {
public:
    char pad_000[0x4];
    int field_4;
};

class Screen2EA4 {
public:
    char pad_000[0x584];
    State2EA4 *field_584;
};

extern "C" void func_08935524(void *dst, const void *src, int size);
extern "C" const MemberStep D_08AB3004[5];

extern "C" {
void func_08AB2EA4(Screen2EA4 *self)
{
    MemberStep table[5];
    const MemberStep *step;
    unsigned char *object;
    unsigned int target;
    int index;
    int delta;
    void (*fn)(void *);
    func_08935524(table, D_08AB3004, sizeof(table));
    step = &table[self->field_584->field_4];
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
}
}
