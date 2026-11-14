/* Runs the current step of a 6-entry member-function table copied from .data (state in
   field_1EE4->field_4). The member call is written out the way SNC lowers it: delta, vtable
   index and target are read first, then `this` is adjusted and, for a virtual entry, the vtable
   entry replaces the target. */
struct MemberStep {
    short delta;          /* added to the object pointer */
    short index;          /* vtable index; 0 for a non-virtual member */
    unsigned int target;  /* function address, or the vptr offset when index != 0 */
};

class State53D8 {
public:
    char pad_000[0x4];
    int field_4;
};

class Screen53D8 {
public:
    char pad_000[0x1EE4];
    State53D8 *field_1EE4;
};

extern "C" void func_08935524(void *dst, const void *src, int size);
extern "C" const MemberStep D_08AA8A6C[6];

extern "C" {
void func_08AA53D8(Screen53D8 *self)
{
    MemberStep table[6];
    const MemberStep *step;
    unsigned char *object;
    unsigned int target;
    int index;
    int delta;
    void (*fn)(void *);
    func_08935524(table, D_08AA8A6C, sizeof(table));
    step = &table[self->field_1EE4->field_4];
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
