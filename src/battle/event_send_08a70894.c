typedef struct EventOwner70894 {
    char pad_000[0x68];
    int field_68;
} EventOwner70894;

extern void func_08A969BC(int id, int value);

int func_08A70894(EventOwner70894 *self, void *a, void *b, void *c)
{
    if (!a || !b || !c) {
        return -1;
    }
    func_08A969BC(self->field_68, 0);
    return 0;
}
