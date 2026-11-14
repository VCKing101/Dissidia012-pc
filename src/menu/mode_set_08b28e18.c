/* Switches the state to mode 2 and runs the mode setup. */
struct State08B28E18 {
    char pad_000[0x408];
    int field_408;
};

struct Owner08B28E18 {
    char pad_000[0x14C];
    struct State08B28E18 *field_14C;
};

extern void func_08B28E3C(struct Owner08B28E18 *self);

void func_08B28E18(struct Owner08B28E18 *self)
{
    self->field_14C->field_408 = 2;
    func_08B28E3C(self);
}
