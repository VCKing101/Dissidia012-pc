extern "C" {
extern void func_089EF350(void *);
extern void func_089F82C8(void *);
extern void func_08A08224(void *);
extern void func_08A0E4A8(void *);
}

class UnitFDB8Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class UnitFDB8 : public UnitFDB8Base {
public:
    char pad_004[0x18];
    char field_1C[0x28];
    char field_44[0x38];
    char field_7C[0x98];
    char field_114[0x4];
};

extern "C" {
void func_089EFDB8(UnitFDB8 *self)
{
    func_08A08224(self->field_114);
    func_08A0E4A8(self->field_7C);
    func_089F82C8(self->field_44);
    func_089EF350(self->field_1C);
    self->slot_010();
}
}
