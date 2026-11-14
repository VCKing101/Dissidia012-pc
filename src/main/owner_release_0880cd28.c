typedef struct Owner {
    char pad0[0x30];
    void *first;
    void *second;
    void *third;
} Owner;
extern void func_08842138(void *object);
void func_0880CD28(Owner *owner)
{
    if (owner->first != 0) {
        func_08842138(owner->first);
    }
    owner->first = 0;
    if (owner->second != 0) {
        func_08842138(owner->second);
    }
    owner->second = 0;
    if (owner->third != 0) {
        func_08842138(owner->third);
    }
    owner->third = 0;
}
