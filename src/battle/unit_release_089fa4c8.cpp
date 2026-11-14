extern "C" void func_08A175E8(void *member);

class UnitA4C8Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

struct Member118 {
    char data[0x118];
};

class UnitA4C8 : public UnitA4C8Base {
public:
    char pad_004[0xC];
    Member118 field_10[2];
};

extern "C" {

void func_089FA4C8(UnitA4C8 *self)
{
    for (int i = 0; i < 2; i++) {
        func_08A175E8(&self->field_10[i]);
    }
    self->slot_010();
}

}
