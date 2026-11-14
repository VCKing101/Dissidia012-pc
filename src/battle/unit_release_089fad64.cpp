extern "C" {
extern void func_089F5F44(void *);
extern void func_089F82C8(void *);
extern void func_08A08224(void *);
extern void func_08A10B88(void *);
}

class UnitAD64Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class UnitAD64 : public UnitAD64Base {
public:
    char pad_004[0x18];
    char field_1C[0x2C];
    char field_48[0x38];
    char field_80[0x48];
    char field_C8[0x4];
};

extern "C" {
void func_089FAD64(UnitAD64 *self)
{
    func_08A08224(self->field_C8);
    func_089F5F44(self->field_80);
    func_089F82C8(self->field_48);
    func_08A10B88(self->field_1C);
    self->slot_010();
}
}
