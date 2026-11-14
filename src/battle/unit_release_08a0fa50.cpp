extern "C" {
extern void func_08A0ED3C(void *);
}

class UnitFA50Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class UnitFA50 : public UnitFA50Base {
public:
    char pad_004[0xC];
    char field_10[0x4];
};

extern "C" {
void func_08A0FA50(UnitFA50 *self)
{
    func_08A0ED3C(self->field_10);
    self->slot_010();
}
}
