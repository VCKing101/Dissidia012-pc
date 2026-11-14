typedef struct EventOwner71C44 {
    unsigned char active;
    unsigned char field_01;
    char pad_002[0x66];
    int field_68;
} EventOwner71C44;

extern void func_08A97060(int id, int command);

int func_08A71C44(EventOwner71C44 *self, void *a, void *b, void *c)
{
    if (!a || !b || !c) {
        return -1;
    }
    func_08A97060(self->field_68, 0x15);
    self->field_01 = 1;
    return 0;
}
