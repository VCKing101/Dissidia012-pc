/* Detaches the model (virtual 0 with mode 2) and frees its block. */
class Model08B324D8 {
    char data[0xA1C];
public:
    virtual void detach(int mode);
};

struct State08B324D8 {
    char pad_000[0x408];
    void *field_408;
    Model08B324D8 *field_40C;
};

struct Owner08B324D8 {
    char pad_000[0x134];
    void *field_134;
    State08B324D8 *field_138;
};

extern "C" {
void func_0885C538(void *heap, void *block, int flags);

void func_08B324D8(Owner08B324D8 *self)
{
    self->field_138->field_40C->detach(2);
    func_0885C538(self->field_134, self->field_138->field_408, 0);
}
}
