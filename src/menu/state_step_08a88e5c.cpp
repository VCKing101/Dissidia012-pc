/* Runs the current step of a 5-entry member-function table copied from .data (state in
   field_30C->field_418). The member call is written out the way SNC lowers it: delta, vtable
   index and target are read first, then `this` is adjusted and, for a virtual entry, the vtable
   entry replaces the target. */
struct MemberStep {
    short delta;          /* added to the object pointer */
    short index;          /* vtable index; 0 for a non-virtual member */
    unsigned int target;  /* function address, or the vptr offset when index != 0 */
};

class State8E5C {
public:
    char pad_000[0x418];
    int field_418;
};

class Screen8E5C {
public:
    char pad_000[0x30C];
    State8E5C *field_30C;
};

extern "C" void func_08935524(void *dst, const void *src, int size);
extern "C" const MemberStep D_08A8B7AC[5];

extern "C" {
void func_08A88E5C(Screen8E5C *self)
{
    MemberStep table[5];
    const MemberStep *step;
    unsigned char *object;
    unsigned int target;
    int index;
    int delta;
    void (*fn)(void *);
    func_08935524(table, D_08A8B7AC, sizeof(table));
    step = &table[self->field_30C->field_418];
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
