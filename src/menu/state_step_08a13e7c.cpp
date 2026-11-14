/* Runs the current step of a 16-entry member-function table copied from .data (state in
   field_F60->field_C8C). The member call is written out the way SNC lowers it: delta, vtable
   index and target are read first, then `this` is adjusted and, for a virtual entry, the vtable
   entry replaces the target. */
struct MemberStep {
    short delta;          /* added to the object pointer */
    short index;          /* vtable index; 0 for a non-virtual member */
    unsigned int target;  /* function address, or the vptr offset when index != 0 */
};

class State3E7C {
public:
    char pad_000[0xC8C];
    int field_C8C;
    char pad_C90[0x4];
    int field_C94;
};

class Screen3E7C {
public:
    char pad_000[0xF60];
    State3E7C *field_F60;
};

extern "C" void func_08935524(void *dst, const void *src, int size);
extern "C" const MemberStep D_08A15F90[16];
extern "C" void func_08A12E08(Screen3E7C *self);

extern "C" {
int func_08A13E7C(Screen3E7C *self)
{
    MemberStep table[16];
    const MemberStep *step;
    unsigned char *object;
    unsigned int target;
    int index;
    int delta;
    void (*fn)(void *);
    func_08935524(table, D_08A15F90, sizeof(table));
    step = &table[self->field_F60->field_C8C];
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
    func_08A12E08(self);
    return self->field_F60->field_C94;
}
}
