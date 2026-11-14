extern "C" {
extern void func_089F82C8(void *);
extern void func_08A08224(void *);
extern void func_08A0E4A8(void *);
extern void func_08A10B88(void *);
}

class Unit5C88Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class Unit5C88 : public Unit5C88Base {
public:
    char pad_004[0x14];
    char field_18[0x2C];
    char field_44[0x38];
    char field_7C[0x98];
    char field_114[0x4];
};

extern "C" {
void func_08A15C88(Unit5C88 *self)
{
    func_08A08224(self->field_114);
    func_08A0E4A8(self->field_7C);
    func_089F82C8(self->field_44);
    func_08A10B88(self->field_18);
    self->slot_010();
}
}
