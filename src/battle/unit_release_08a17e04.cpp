extern "C" {
extern void func_08A00784(void *);
extern void func_08A01844(void *);
extern void func_08A01E88(void *);
extern void func_08A08224(void *);
extern void func_08A1681C(void *);
extern void func_08A18E18(void *);
}

class Unit7E04Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class Unit7E04 : public Unit7E04Base {
public:
    char pad_004[0x10];
    char field_14[0x28];
    char field_3C[0x24];
    char field_60[0x24];
    char field_84[0x30];
    char field_B4[0x24];
    char field_D8[0x3C];
    char field_114[0x4];
};

extern "C" {
void func_08A17E04(Unit7E04 *self)
{
    func_08A08224(self->field_114);
    func_08A00784(self->field_D8);
    func_08A01E88(self->field_B4);
    func_08A1681C(self->field_84);
    func_08A01E88(self->field_60);
    func_08A18E18(self->field_3C);
    func_08A01844(self->field_14);
    self->slot_010();
}
}
