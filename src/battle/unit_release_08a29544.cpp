extern "C" {
extern void func_089F5F44(void *);
extern void func_089F82C8(void *);
extern void func_08A01844(void *);
extern void func_08A1681C(void *);
}

class Unit08A29544Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class Unit08A29544 : public Unit08A29544Base {
public:
    char pad_004[0x18];
    char field_1C[0x28];
    char field_44[0x30];
    char field_74[0x38];
    char field_AC[0x4];
};

extern "C" {
void func_08A29544(Unit08A29544 *self)
{
    func_089F5F44(self->field_AC);
    func_089F82C8(self->field_74);
    func_08A1681C(self->field_44);
    func_08A01844(self->field_1C);
    self->slot_010();
}
}
