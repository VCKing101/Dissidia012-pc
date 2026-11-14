extern "C" {
extern void func_08A01844(void *);
extern void func_08A08224(void *);
extern void func_08A1681C(void *);
}

class Unit00FCBase {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class Unit00FC : public Unit00FCBase {
public:
    char pad_004[0x10];
    char field_14[0x28];
    char field_3C[0x30];
    char field_6C[0x4];
};

extern "C" {
void func_08A100FC(Unit00FC *self)
{
    func_08A08224(self->field_6C);
    func_08A1681C(self->field_3C);
    func_08A01844(self->field_14);
    self->slot_010();
}
}
