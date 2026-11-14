extern "C" {
extern void func_08A01844(void *);
extern void func_08A01E88(void *);
extern void func_08A0D768(void *);
extern void func_08A1681C(void *);
}

class UnitDA84Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class UnitDA84 : public UnitDA84Base {
public:
    char pad_004[0x14];
    char field_18[0x28];
    char field_40[0x30];
    char field_70[0x24];
    char field_94[0x4];
};

extern "C" {
void func_08A0DA84(UnitDA84 *self)
{
    func_08A0D768(self->field_94);
    func_08A01E88(self->field_70);
    func_08A1681C(self->field_40);
    func_08A01844(self->field_18);
    self->slot_010();
}
}
