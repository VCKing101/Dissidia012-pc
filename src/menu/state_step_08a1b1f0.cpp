/* Runs the current step of a 10-entry member-function table copied from .data (state in
   field_154->field_D7C). The member call is written out the way SNC lowers it: delta, vtable
   index and target are read first, then `this` is adjusted and, for a virtual entry, the vtable
   entry replaces the target. */
struct MemberStep {
    short delta;          /* added to the object pointer */
    short index;          /* vtable index; 0 for a non-virtual member */
    unsigned int target;  /* function address, or the vptr offset when index != 0 */
};

class StateB1F0 {
public:
    char pad_000[0xD7C];
    int field_D7C;
};

class ScreenB1F0 {
public:
    char pad_000[0x154];
    StateB1F0 *field_154;
};

extern "C" void func_08935524(void *dst, const void *src, int size);
extern "C" const MemberStep D_08A23FF4[10];
extern "C" void func_08A1D0C4(ScreenB1F0 *self);

extern "C" {
void func_08A1B1F0(ScreenB1F0 *self)
{
    MemberStep table[10];
    const MemberStep *step;
    unsigned char *object;
    unsigned int target;
    int index;
    int delta;
    void (*fn)(void *);
    func_08935524(table, D_08A23FF4, sizeof(table));
    step = &table[self->field_154->field_D7C];
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
    func_08A1D0C4(self);
}
}
