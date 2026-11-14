extern "C" {
extern void func_089F1C80(void *);
}

class Unit1564Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class Unit1564 : public Unit1564Base {
public:
    char pad_004[0x10];
    char field_14[0x4];
};

extern "C" {
void func_089F1564(Unit1564 *self)
{
    func_089F1C80(self->field_14);
    self->slot_010();
}
}
