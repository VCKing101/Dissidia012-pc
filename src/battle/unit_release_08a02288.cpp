extern "C" void func_08A02E38(void *self, void *member);

class Unit2288Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class Unit2288 : public Unit2288Base {
public:
    char pad_004[0x4];
    void *field_08[2];
};

extern "C" {

void func_08A02288(Unit2288 *self)
{
    for (int i = 0; i < 2; i++) {
        if (self->field_08[i] != 0) {
            func_08A02E38(self, self->field_08[i]);
        }
    }
    self->slot_010();
}

}
