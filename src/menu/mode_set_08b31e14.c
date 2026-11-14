/* Switches the state to mode 2 and runs the mode setup. */
struct State08B31E14 {
    char pad_000[0x400];
    int field_400;
};

struct Owner08B31E14 {
    char pad_000[0x138];
    struct State08B31E14 *field_138;
};

extern void func_08B31E38(struct Owner08B31E14 *self);

void func_08B31E14(struct Owner08B31E14 *self)
{
    self->field_138->field_400 = 2;
    func_08B31E38(self);
}
