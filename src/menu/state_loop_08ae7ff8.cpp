/* Runs the current step of a 4-entry member-function table (state in field_A690->field_2554)
   until a step returns 0, then returns field_255C. The member call is written out the way SNC
   lowers it: delta and vtable index are read, `this` is adjusted, then the target is read and,
   for a virtual entry, replaced by the vtable entry. */
struct MemberStep {
    short delta;          /* added to the object pointer */
    short index;          /* vtable index; 0 for a non-virtual member */
    unsigned int target;  /* function address, or the vptr offset when index != 0 */
};

class Loop7FF8State {
public:
    char pad_000[0x2554];
    int field_2554;
    char pad_2558[0x4];
    int field_255C;
};

class Loop7FF8Screen {
public:
    char pad_000[0xA690];
    Loop7FF8State *field_A690;
};

extern "C" void func_08935524(void *dst, const void *src, int size);
extern "C" const MemberStep D_08AEBD98[4];

extern "C" {
int func_08AE7FF8(Loop7FF8Screen *self)
{
    MemberStep table[4];
    const MemberStep *step;
    unsigned char *object;
    unsigned int target;
    int index;
    int delta;
    int result;
    func_08935524(table, D_08AEBD98, sizeof(table));
    do {
        step = &table[self->field_A690->field_2554];
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
    return self->field_A690->field_255C;
}
}
