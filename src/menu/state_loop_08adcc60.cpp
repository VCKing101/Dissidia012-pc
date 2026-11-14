/* Runs the current step of a 4-entry member-function table (state in field_46AC->field_1B78)
   until a step returns 0, then returns field_1B80. The member call is written out the way SNC
   lowers it: delta and vtable index are read, `this` is adjusted, then the target is read and,
   for a virtual entry, replaced by the vtable entry. */
struct MemberStep {
    short delta;          /* added to the object pointer */
    short index;          /* vtable index; 0 for a non-virtual member */
    unsigned int target;  /* function address, or the vptr offset when index != 0 */
};

class LoopCC60State {
public:
    char pad_000[0x1B78];
    int field_1B78;
    char pad_1B7C[0x4];
    int field_1B80;
};

class LoopCC60Screen {
public:
    char pad_000[0x46AC];
    LoopCC60State *field_46AC;
};

extern "C" void func_08935524(void *dst, const void *src, int size);
extern "C" const MemberStep D_08ADFC10[4];

extern "C" {
int func_08ADCC60(LoopCC60Screen *self)
{
    MemberStep table[4];
    const MemberStep *step;
    unsigned char *object;
    unsigned int target;
    int index;
    int delta;
    int result;
    func_08935524(table, D_08ADFC10, sizeof(table));
    do {
        step = &table[self->field_46AC->field_1B78];
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
    return self->field_46AC->field_1B80;
}
}
