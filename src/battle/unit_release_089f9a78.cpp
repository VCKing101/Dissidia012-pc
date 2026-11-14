extern "C" {
extern void func_08A00784(void *);
extern void func_08A01844(void *);
extern void func_08A01E88(void *);
extern void func_08A08224(void *);
extern void func_08A1681C(void *);
}

class Unit9A78Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class Unit9A78 : public Unit9A78Base {
public:
    char pad_004[0x10];
    char field_14[0x28];
    char field_3C[0x30];
    char field_6C[0x24];
    char field_90[0x3C];
    char field_CC[0x4];
};

extern "C" {
void func_089F9A78(Unit9A78 *self)
{
    func_08A08224(self->field_CC);
    func_08A00784(self->field_90);
    func_08A01E88(self->field_6C);
    func_08A1681C(self->field_3C);
    func_08A01844(self->field_14);
    self->slot_010();
}
}
