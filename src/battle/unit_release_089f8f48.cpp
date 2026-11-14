extern "C" {
extern void func_08A08224(void *member);
extern void func_089F82C8(void *member);
extern void func_089F8788(void *member);
}

class Unit8F48Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

struct Member34 {
    char data[0x34];
};

class Unit8F48 : public Unit8F48Base {
public:
    char pad_004[0x14];
    char field_18[0x90];
    char field_A8[0x38];
    Member34 field_E0[2];
};

extern "C" {

void func_089F8F48(Unit8F48 *self)
{
    for (int i = 0; i < 2; i++) {
        func_08A08224(&self->field_E0[i]);
    }
    func_089F82C8(self->field_A8);
    func_089F8788(self->field_18);
    self->slot_010();
}

}
