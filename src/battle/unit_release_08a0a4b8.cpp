extern "C" {
extern void func_089F6490(void *);
extern void func_08A08224(void *);
extern void func_08A10B88(void *);
}

class UnitA4B8Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class UnitA4B8 : public UnitA4B8Base {
public:
    char pad_004[0x10];
    char field_14[0x2C];
    char field_40[0x938];
    char field_978[0x4];
};

extern "C" {
void func_08A0A4B8(UnitA4B8 *self)
{
    func_08A08224(self->field_978);
    func_089F6490(self->field_40);
    func_08A10B88(self->field_14);
    self->slot_010();
}
}
