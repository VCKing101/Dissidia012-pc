extern "C" {
extern void func_08A2568C(void *);
extern void func_08A262C4(void *);
}

struct Part28D7C {
    char data[0x11C];
};

class Unit08A28D7CBase {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class Unit08A28D7C : public Unit08A28D7CBase {
public:
    char pad_004[0xC];
    Part28D7C field_10[2];
    char field_248[0x4];
};

extern "C" {
void func_08A28D7C(Unit08A28D7C *self)
{
    func_08A2568C(self->field_248);
    for (int i = 0; i < 2; i++) {
        func_08A262C4(&self->field_10[i]);
    }
    self->slot_010();
}
}
