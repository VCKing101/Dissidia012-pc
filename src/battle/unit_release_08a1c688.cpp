extern "C" {
extern void func_089F82C8(void *);
extern void func_08A01844(void *);
extern void func_08A08224(void *);
extern void func_08A10B88(void *);
}

class Unit08A1C688Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class Unit08A1C688 : public Unit08A1C688Base {
public:
    char pad_004[0x10];
    char field_14[0x28];
    char field_3C[0x28];
    char field_64[0x28];
    char field_8C[0x38];
    char field_C4[0x2C];
    char field_F0[0x4];
};

extern "C" {
void func_08A1C688(Unit08A1C688 *self)
{
    func_08A08224(self->field_F0);
    func_08A10B88(self->field_C4);
    func_089F82C8(self->field_8C);
    func_08A01844(self->field_64);
    func_08A01844(self->field_3C);
    func_08A01844(self->field_14);
    self->slot_010();
}
}
