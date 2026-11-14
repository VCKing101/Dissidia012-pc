extern "C" {
extern void func_08A01844(void *);
extern void func_08A08224(void *);
extern void func_08A10B88(void *);
extern void func_08A1681C(void *);
}

class UnitB020Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class UnitB020 : public UnitB020Base {
public:
    char pad_004[0x10];
    char field_14[0x2C];
    char field_40[0x28];
    char field_68[0x30];
    char field_98[0x4];
};

extern "C" {
void func_08A0B020(UnitB020 *self)
{
    func_08A08224(self->field_98);
    func_08A1681C(self->field_68);
    func_08A01844(self->field_40);
    func_08A10B88(self->field_14);
    self->slot_010();
}
}
