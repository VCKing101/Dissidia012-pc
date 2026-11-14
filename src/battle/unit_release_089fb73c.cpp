extern "C" {
extern void func_089F82C8(void *);
extern void func_08A08224(void *);
extern void func_08A21578(void *);
}

class UnitB73CBase {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class UnitB73C : public UnitB73CBase {
public:
    char pad_004[0x14];
    char field_18[0x78];
    char field_90[0x38];
    char field_C8[0x4];
};

extern "C" {
void func_089FB73C(UnitB73C *self)
{
    func_08A08224(self->field_C8);
    func_089F82C8(self->field_90);
    func_08A21578(self->field_18);
    self->slot_010();
}
}
