extern "C" {
extern void func_089FC0D8(void *);
extern void func_089FDEF8(void *);
extern void func_08A01844(void *);
extern void func_08A1681C(void *);
}

class UnitD54CBase {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class UnitD54C : public UnitD54CBase {
public:
    char pad_004[0x10];
    char field_14[0x28];
    char field_3C[0x30];
    char field_6C[0x34];
    char field_A0[0x4];
};

extern "C" {
void func_089FD54C(UnitD54C *self)
{
    func_089FC0D8(self->field_A0);
    func_089FDEF8(self->field_6C);
    func_08A1681C(self->field_3C);
    func_08A01844(self->field_14);
    self->slot_010();
}
}
