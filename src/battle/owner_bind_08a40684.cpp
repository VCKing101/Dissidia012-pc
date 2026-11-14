struct BindRecord {
    char pad_000[0xD0];
    int field_D0;
};

struct BindFighter {
    char pad_000[0x390];
    BindRecord *record;
};

class OwnerBind40684 {
public:
    BindFighter *fighter;
    int field_04;
    int field_08;
    int field_0C;
    virtual void slot_008(int arg);
};

extern "C" {

void func_08A40684(OwnerBind40684 *self, int arg, BindFighter *fighter, int a, int b)
{
    self->fighter = fighter;
    self->field_04 = a;
    self->field_08 = b;
    self->field_0C = fighter->record->field_D0;
    self->slot_008(arg);
}

}
