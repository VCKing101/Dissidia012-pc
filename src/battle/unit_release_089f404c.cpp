extern "C" {
extern void func_08A08224(void *member);
extern void func_089F34D4(void *member);
extern void func_089F82C8(void *member);
extern void func_08A10B88(void *member);
extern void func_089F3908(void *member);
}

class Unit404CBase {
public:
    virtual void slot_008();
    virtual void slot_010();
};

struct Member38 {
    char data[0x38];
};

struct Member140 {
    char data[0x140];
};

struct Member34 {
    char data[0x34];
};

class Unit404C : public Unit404CBase {
public:
    char pad_004[0x1C];
    char field_20[0x1C];
    char field_3C[0x2C];
    Member38 field_68[2];
    char pad_0D8[0x8];
    Member140 field_E0[2];
    Member34 field_360[2];
};

extern "C" {

void func_089F404C(Unit404C *self)
{
    for (int i = 0; i < 2; i++) {
        func_08A08224(&self->field_360[i]);
        func_089F34D4(&self->field_E0[i]);
        func_089F82C8(&self->field_68[i]);
    }
    func_08A10B88(self->field_3C);
    func_089F3908(self->field_20);
    self->slot_010();
}

}
