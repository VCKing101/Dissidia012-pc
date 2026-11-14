extern "C" {
extern void func_089F5F44(void *);
extern void func_089F82C8(void *);
extern void func_089FC0D8(void *);
extern void func_089FDEF8(void *);
extern void func_08A01844(void *);
extern void func_08A10B88(void *);
extern void func_08A1681C(void *);
}

class Unit08A1D64CBase {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class Unit08A1D64C : public Unit08A1D64CBase {
public:
    char pad_004[0x1C];
    char field_20[0x2C];
    char field_4C[0x38];
    char field_84[0x48];
    char field_CC[0x28];
    char field_F4[0x30];
    char field_124[0x34];
    char field_158[0x4];
};

extern "C" {
void func_08A1D64C(Unit08A1D64C *self)
{
    func_089FC0D8(self->field_158);
    func_089FDEF8(self->field_124);
    func_08A1681C(self->field_F4);
    func_08A01844(self->field_CC);
    func_089F5F44(self->field_84);
    func_089F82C8(self->field_4C);
    func_08A10B88(self->field_20);
    self->slot_010();
}
}
