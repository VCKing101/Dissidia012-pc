typedef struct StateObj {
    char pad_00[0x10F4];
    int field_10F4;
} StateObj;

extern void func_08A57734(StateObj *self);
extern void func_08A57FC0(StateObj *self);

unsigned char func_08A58068(StateObj *self)
{
    if (self->field_10F4 == 6) {
        func_08A57FC0(self);
        func_08A57734(self);
        return 1;
    }
    return 0;
}
