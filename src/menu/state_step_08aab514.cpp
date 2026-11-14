/* Runs the current step of an 8-entry member-function table copied from .data (state in
   field_7CFC->field_4). The member call is written out the way SNC lowers it: delta, vtable
   index and target are read first, then `this` is adjusted and, for a virtual entry, the vtable
   entry replaces the target. */
struct MemberStep {
    short delta;          /* added to the object pointer */
    short index;          /* vtable index; 0 for a non-virtual member */
    unsigned int target;  /* function address, or the vptr offset when index != 0 */
};

class StateB514 {
public:
    char pad_000[0x4];
    int field_4;
};

class ScreenB514 {
public:
    char pad_000[0x7CFC];
    StateB514 *field_7CFC;
};

extern "C" void func_08935524(void *dst, const void *src, int size);
extern "C" const MemberStep D_08AB09A0[8];
extern "C" void func_08AAE340(ScreenB514 *self);
extern "C" void func_08AAE8FC(ScreenB514 *self);

extern "C" {
int func_08AAB514(ScreenB514 *self)
{
    MemberStep table[8];
    const MemberStep *step;
    unsigned char *object;
    unsigned int target;
    int index;
    int delta;
    int (*fn)(void *);
    int result;
    func_08935524(table, D_08AB09A0, sizeof(table));
    step = &table[self->field_7CFC->field_4];
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
    fn = (int (*)(void *))target;
    result = fn(object);
    func_08AAE340(self);
    func_08AAE8FC(self);
    return result;
}
}
