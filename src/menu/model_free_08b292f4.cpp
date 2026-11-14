/* Detaches the model (virtual 0 with mode 2) and frees its block. */
class Model08B292F4 {
    char data[0xA1C];
public:
    virtual void detach(int mode);
};

struct State08B292F4 {
    char pad_000[0x410];
    void *field_410;
    Model08B292F4 *field_414;
};

struct Owner08B292F4 {
    char pad_000[0x138];
    void *field_138;
    char pad_13C[0x14C - 0x13C];
    State08B292F4 *field_14C;
};

extern "C" {
void func_0885C538(void *heap, void *block, int flags);

void func_08B292F4(Owner08B292F4 *self)
{
    self->field_14C->field_414->detach(2);
    func_0885C538(self->field_138, self->field_14C->field_410, 0);
}
}
