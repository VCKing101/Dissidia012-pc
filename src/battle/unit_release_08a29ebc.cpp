extern "C" {
extern void func_08A01844(void *);
extern void func_08A1681C(void *);
}

class Unit08A29EBCBase {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class Unit08A29EBC : public Unit08A29EBCBase {
public:
    char pad_004[0x10];
    char field_14[0x28];
    char field_3C[0x4];
};

extern "C" {
void func_08A29EBC(Unit08A29EBC *self)
{
    func_08A1681C(self->field_3C);
    func_08A01844(self->field_14);
    self->slot_010();
}
}
