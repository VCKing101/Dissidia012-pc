extern "C" {
extern void func_089F5F44(void *);
extern void func_089F82C8(void *);
extern void func_08A08224(void *);
extern void func_08A10B88(void *);
}

class Unit08A1ED38Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class Unit08A1ED38 : public Unit08A1ED38Base {
public:
    char pad_004[0x18];
    char field_1C[0x2C];
    char field_48[0x2C];
    char field_74[0x38];
    char field_AC[0x48];
    char field_F4[0x4];
};

extern "C" {
void func_08A1ED38(Unit08A1ED38 *self)
{
    func_08A08224(self->field_F4);
    func_089F5F44(self->field_AC);
    func_089F82C8(self->field_74);
    func_08A10B88(self->field_48);
    func_08A10B88(self->field_1C);
    self->slot_010();
}
}
