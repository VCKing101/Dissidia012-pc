/* Runs the current step of a 4-entry member-function table (state in field_DB30->field_F14)
   until a step returns 0, then returns field_F1C. The member call is written out the way SNC
   lowers it: delta and vtable index are read, `this` is adjusted, then the target is read and,
   for a virtual entry, replaced by the vtable entry. */
struct MemberStep {
    short delta;          /* added to the object pointer */
    short index;          /* vtable index; 0 for a non-virtual member */
    unsigned int target;  /* function address, or the vptr offset when index != 0 */
};

class Loop0F34State {
public:
    char pad_000[0xF14];
    int field_F14;
    char pad_F18[0x4];
    int field_F1C;
};

class Loop0F34Screen {
public:
    char pad_000[0xDB30];
    Loop0F34State *field_DB30;
};

extern "C" void func_08935524(void *dst, const void *src, int size);
extern "C" const MemberStep D_08ADA7FC[4];

extern "C" {
int func_08AD0F34(Loop0F34Screen *self)
{
    MemberStep table[4];
    const MemberStep *step;
    unsigned char *object;
    unsigned int target;
    int index;
    int delta;
    int result;
    func_08935524(table, D_08ADA7FC, sizeof(table));
    do {
        step = &table[self->field_DB30->field_F14];
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
    return self->field_DB30->field_F1C;
}
}
