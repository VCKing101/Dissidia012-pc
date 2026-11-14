extern "C" {
extern void func_089F2108(void *);
extern void func_089F82C8(void *);
extern void func_08A08224(void *);
extern void func_08A10B88(void *);
}

class Unit2AC0Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class Unit2AC0 : public Unit2AC0Base {
public:
    char pad_004[0x18];
    char field_1C[0x2C];
    char field_48[0x38];
    char field_80[0x170];
    char field_1F0[0x4];
};

extern "C" {
void func_089F2AC0(Unit2AC0 *self)
{
    func_08A08224(self->field_1F0);
    func_089F2108(self->field_80);
    func_089F82C8(self->field_48);
    func_08A10B88(self->field_1C);
    self->slot_010();
}
}
