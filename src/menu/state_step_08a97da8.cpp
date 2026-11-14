/* Runs the current step of a 53-entry member-function table copied from .data (state in
   field_154->field_910). The member call is written out the way SNC lowers it: delta, vtable
   index and target are read first, then `this` is adjusted and, for a virtual entry, the vtable
   entry replaces the target. */
struct MemberStep {
    short delta;          /* added to the object pointer */
    short index;          /* vtable index; 0 for a non-virtual member */
    unsigned int target;  /* function address, or the vptr offset when index != 0 */
};

class State7DA8 {
public:
    char pad_000[0x910];
    int field_910;
};

class Screen7DA8 {
public:
    char pad_000[0x154];
    State7DA8 *field_154;
};

extern "C" void func_08935524(void *dst, const void *src, int size);
extern "C" const MemberStep D_08AA3F48[53];
extern "C" void func_08A9C140(Screen7DA8 *self, int mode);

extern "C" {
void func_08A97DA8(Screen7DA8 *self)
{
    MemberStep table[53];
    const MemberStep *step;
    unsigned char *object;
    unsigned int target;
    int index;
    int delta;
    void (*fn)(void *);
    func_08935524(table, D_08AA3F48, sizeof(table));
    step = &table[self->field_154->field_910];
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
    func_08A9C140(self, 0);
}
}
