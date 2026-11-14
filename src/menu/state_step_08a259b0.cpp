/* Runs the current step of a 4-entry member-function table copied from .data (state in
   field_28F0->field_1E6C). The member call is written out the way SNC lowers it: delta, vtable
   index and target are read first, then `this` is adjusted and, for a virtual entry, the vtable
   entry replaces the target. */
struct MemberStep {
    short delta;          /* added to the object pointer */
    short index;          /* vtable index; 0 for a non-virtual member */
    unsigned int target;  /* function address, or the vptr offset when index != 0 */
};

class State59B0 {
public:
    char pad_000[0x1E6C];
    int field_1E6C;
    char pad_1E70[0x4];
    int field_1E74;
};

class Screen59B0 {
public:
    char pad_000[0x28F0];
    State59B0 *field_28F0;
};

extern "C" void func_08935524(void *dst, const void *src, int size);
extern "C" const MemberStep D_08A28B24[4];

extern "C" {
int func_08A259B0(Screen59B0 *self)
{
    MemberStep table[4];
    const MemberStep *step;
    unsigned char *object;
    unsigned int target;
    int index;
    int delta;
    void (*fn)(void *);
    func_08935524(table, D_08A28B24, sizeof(table));
    step = &table[self->field_28F0->field_1E6C];
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
    return self->field_28F0->field_1E74;
}
}
