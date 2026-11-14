extern "C" {
extern void func_08A01844(void *);
extern void func_08A10B88(void *);
extern void func_08A1681C(void *);
}

class UnitC730Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class UnitC730 : public UnitC730Base {
public:
    char pad_004[0x10];
    char field_14[0x28];
    char field_3C[0x30];
    char field_6C[0x4];
};

extern "C" {
void func_08A0C730(UnitC730 *self)
{
    func_08A10B88(self->field_6C);
    func_08A1681C(self->field_3C);
    func_08A01844(self->field_14);
    self->slot_010();
}
}
