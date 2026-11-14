extern "C" {
extern void func_089F5F44(void *);
extern void func_089F82C8(void *);
extern void func_08A01844(void *);
extern void func_08A1681C(void *);
}

class UnitFDB0Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class UnitFDB0 : public UnitFDB0Base {
public:
    char pad_004[0x1C];
    char field_20[0x28];
    char field_48[0x30];
    char field_78[0x38];
    char field_B0[0x4];
};

extern "C" {
void func_089FFDB0(UnitFDB0 *self)
{
    func_089F5F44(self->field_B0);
    func_089F82C8(self->field_78);
    func_08A1681C(self->field_48);
    func_08A01844(self->field_20);
    self->slot_010();
}
}
