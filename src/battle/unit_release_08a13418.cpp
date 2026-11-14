extern "C" {
extern void func_08A08224(void *);
extern void func_089F5F44(void *);
extern void func_08A12CBC(void *);
extern void func_089F82C8(void *);
extern void func_08A01E88(void *);
extern void func_08A1681C(void *);
extern void func_08A01844(void *);
}

class Unit3418Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

struct Entry38 { char data[0x38]; };

class Unit3418 : public Unit3418Base {
public:
    char pad_004[0x20];
    char field_24[0x28];
    char field_4C[0x30];
    char field_7C[0x24];
    Entry38 field_A0[2];
    char field_110[0x68];
    char field_178[0x48];
    char field_1C0[4];
};

extern "C" {
void func_08A13418(Unit3418 *self)
{
    func_08A08224(self->field_1C0);
    func_089F5F44(self->field_178);
    func_08A12CBC(self->field_110);
    for (int i = 0; i < 2; i++) {
        func_089F82C8(&self->field_A0[i]);
    }
    func_08A01E88(self->field_7C);
    func_08A1681C(self->field_4C);
    func_08A01844(self->field_24);
    self->slot_010();
}
}
