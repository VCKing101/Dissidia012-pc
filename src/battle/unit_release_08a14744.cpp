extern "C" {
extern void func_089F5F44(void *);
extern void func_089F82C8(void *);
extern void func_08A01844(void *);
extern void func_08A01E88(void *);
extern void func_08A08224(void *);
extern void func_08A10B88(void *);
extern void func_08A1681C(void *);
}

class Unit4744Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class Unit4744 : public Unit4744Base {
public:
    char pad_004[0x18];
    char field_1C[0x28];
    char field_44[0x30];
    char field_74[0x24];
    char field_98[0x2C];
    char field_C4[0x38];
    char field_FC[0x48];
    char field_144[0x4];
};

extern "C" {
void func_08A14744(Unit4744 *self)
{
    func_08A08224(self->field_144);
    func_089F5F44(self->field_FC);
    func_089F82C8(self->field_C4);
    func_08A10B88(self->field_98);
    func_08A01E88(self->field_74);
    func_08A1681C(self->field_44);
    func_08A01844(self->field_1C);
    self->slot_010();
}
}
