extern "C" {
extern void func_08A08224(void *);
extern void func_08A10B88(void *);
}

class UnitBCD0Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class UnitBCD0 : public UnitBCD0Base {
public:
    char pad_004[0x10];
    char field_14[0x2C];
    char field_40[0x4];
};

extern "C" {
void func_08A0BCD0(UnitBCD0 *self)
{
    func_08A08224(self->field_40);
    func_08A10B88(self->field_14);
    self->slot_010();
}
}
