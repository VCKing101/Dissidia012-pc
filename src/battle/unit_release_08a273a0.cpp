extern "C" {
extern void func_08A08224(void *);
}

class Unit08A273A0Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class Unit08A273A0 : public Unit08A273A0Base {
public:
    char pad_004[0x10];
    char field_14[0x4];
};

extern "C" {
void func_08A273A0(Unit08A273A0 *self)
{
    func_08A08224(self->field_14);
    self->slot_010();
}
}
