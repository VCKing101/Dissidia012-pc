/* Switches the state to mode 2 and runs the mode setup. */
struct State08B24578 {
    char pad_000[0x90C];
    int field_90C;
};

struct Owner08B24578 {
    char pad_000[0x13C];
    struct State08B24578 *field_13C;
};

extern void func_08B2459C(struct Owner08B24578 *self);

void func_08B24578(struct Owner08B24578 *self)
{
    self->field_13C->field_90C = 2;
    func_08B2459C(self);
}
