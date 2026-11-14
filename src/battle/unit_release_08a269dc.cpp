extern "C" {
extern void func_089F82C8(void *);
extern void func_08A08224(void *);
}

class Unit08A269DCBase {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class Unit08A269DC : public Unit08A269DCBase {
public:
    char pad_004[0x18];
    char field_1C[0x34];
    char field_50[0x4];
};

extern "C" {
void func_08A269DC(Unit08A269DC *self)
{
    func_089F82C8(self->field_50);
    func_08A08224(self->field_1C);
    self->slot_010();
}
}
