extern "C" {
extern void func_08A01844(void *);
extern void func_08A01E88(void *);
extern void func_08A1681C(void *);
extern void func_08A201C8(void *);
extern void func_08A20764(void *);
}

class Unit08A20CC0Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class Unit08A20CC0 : public Unit08A20CC0Base {
public:
    char pad_004[0x10];
    char field_14[0x28];
    char field_3C[0x30];
    char field_6C[0x24];
    char field_90[0x38];
    char field_C8[0x4];
};

extern "C" {
void func_08A20CC0(Unit08A20CC0 *self)
{
    func_08A20764(self->field_C8);
    func_08A201C8(self->field_90);
    func_08A01E88(self->field_6C);
    func_08A1681C(self->field_3C);
    func_08A01844(self->field_14);
    self->slot_010();
}
}
