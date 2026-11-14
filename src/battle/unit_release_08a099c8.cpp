extern "C" {
extern void func_089F82C8(void *);
extern void func_089F8788(void *);
}

class Unit99C8Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class Unit99C8 : public Unit99C8Base {
public:
    char pad_004[0x14];
    char field_18[0x90];
    char field_A8[0x4];
};

extern "C" {
void func_08A099C8(Unit99C8 *self)
{
    func_089F82C8(self->field_A8);
    func_089F8788(self->field_18);
    self->slot_010();
}
}
