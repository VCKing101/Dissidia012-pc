typedef struct IdleStep29088 {
    char pad_000[0x8];
    int field_08;
} IdleStep29088;

extern int func_08A290BC(IdleStep29088 *self);

int func_08A29088(IdleStep29088 *self)
{
    if (self->field_08 == 0) {
        return func_08A290BC(self);
    }
    return 0;
}
