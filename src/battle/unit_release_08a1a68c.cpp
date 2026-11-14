extern "C" {
extern void func_089F5F44(void *);
extern void func_089F82C8(void *);
extern void func_08A01844(void *);
extern void func_08A01E88(void *);
extern void func_08A08224(void *);
extern void func_08A10B88(void *);
extern void func_08A18E18(void *);
}

class UnitA68CBase {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class UnitA68C : public UnitA68CBase {
public:
    char pad_004[0x18];
    char field_1C[0x28];
    char field_44[0x24];
    char field_68[0x24];
    char field_8C[0x2C];
    char field_B8[0x38];
    char field_F0[0x48];
    char field_138[0x4];
};

extern "C" {
void func_08A1A68C(UnitA68C *self)
{
    func_08A08224(self->field_138);
    func_089F5F44(self->field_F0);
    func_089F82C8(self->field_B8);
    func_08A10B88(self->field_8C);
    func_08A01E88(self->field_68);
    func_08A18E18(self->field_44);
    func_08A01844(self->field_1C);
    self->slot_010();
}
}
