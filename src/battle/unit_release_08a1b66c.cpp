extern "C" {
extern void func_089F82C8(void *);
extern void func_08A01844(void *);
extern void func_08A01E88(void *);
extern void func_08A08224(void *);
extern void func_08A18E18(void *);
extern void func_08A21578(void *);
}

class UnitB66CBase {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class UnitB66C : public UnitB66CBase {
public:
    char pad_004[0x14];
    char field_18[0x28];
    char field_40[0x24];
    char field_64[0x24];
    char field_88[0x78];
    char field_100[0x38];
    char field_138[0x4];
};

extern "C" {
void func_08A1B66C(UnitB66C *self)
{
    func_08A08224(self->field_138);
    func_089F82C8(self->field_100);
    func_08A21578(self->field_88);
    func_08A01E88(self->field_64);
    func_08A18E18(self->field_40);
    func_08A01844(self->field_18);
    self->slot_010();
}
}
