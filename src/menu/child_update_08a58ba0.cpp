/* Updates the list, then runs virtual step 5 of every child. */
class ChildPartBase {
public:
    char pad_00[0x8];
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
};

struct ChildList {
    char pad_00[0x58];
    int field_58;
    ChildPartBase *field_5C[16];
};

extern "C" void func_08A58830(ChildList *self);

extern "C" {
void func_08A58BA0(ChildList *self)
{
    func_08A58830(self);
    for (int i = 0; i < self->field_58; i++) {
        if (self->field_5C[i] != 0) {
            self->field_5C[i]->v5();
        }
    }
}
}
