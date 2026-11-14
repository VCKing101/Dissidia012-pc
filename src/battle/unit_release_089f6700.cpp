extern "C" {
extern void func_089F82C8(void *);
extern void func_08A08224(void *);
extern void func_08A10B88(void *);
}

class Unit6700Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class Unit6700 : public Unit6700Base {
public:
    char pad_004[0x18];
    char field_1C[0x2C];
    char field_48[0x34];
    char field_7C[0x4];
};

extern "C" {
void func_089F6700(Unit6700 *self)
{
    func_089F82C8(self->field_7C);
    func_08A08224(self->field_48);
    func_08A10B88(self->field_1C);
    self->slot_010();
}
}
