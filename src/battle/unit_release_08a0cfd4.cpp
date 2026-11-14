extern "C" {
extern void func_08A01844(void *);
extern void func_08A1681C(void *);
}

class UnitCFD4Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class UnitCFD4 : public UnitCFD4Base {
public:
    char pad_004[0x10];
    char field_14[0x28];
    char field_3C[0x4];
};

extern "C" {
void func_08A0CFD4(UnitCFD4 *self)
{
    func_08A1681C(self->field_3C);
    func_08A01844(self->field_14);
    self->slot_010();
}
}
