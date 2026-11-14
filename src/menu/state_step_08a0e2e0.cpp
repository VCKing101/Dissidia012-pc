/* Runs the current step of a 16-entry member-function table copied from .data (state in
   field_1320->field_FD0). The member call is written out the way SNC lowers it: delta, vtable
   index and target are read first, then `this` is adjusted and, for a virtual entry, the vtable
   entry replaces the target. */
struct MemberStep {
    short delta;          /* added to the object pointer */
    short index;          /* vtable index; 0 for a non-virtual member */
    unsigned int target;  /* function address, or the vptr offset when index != 0 */
};

class StateE2E0 {
public:
    char pad_000[0xFD0];
    int field_FD0;
    char pad_FD4[0x8];
    int field_FDC;
};

class ScreenE2E0 {
public:
    char pad_000[0x1320];
    StateE2E0 *field_1320;
};

extern "C" void func_08935524(void *dst, const void *src, int size);
extern "C" const MemberStep D_08A11E58[16];

extern "C" {
int func_08A0E2E0(ScreenE2E0 *self)
{
    MemberStep table[16];
    const MemberStep *step;
    unsigned char *object;
    unsigned int target;
    int index;
    int delta;
    void (*fn)(void *);
    func_08935524(table, D_08A11E58, sizeof(table));
    step = &table[self->field_1320->field_FD0];
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
    return self->field_1320->field_FDC;
}
}
