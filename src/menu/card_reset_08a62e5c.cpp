/* Resets card `index`, or all seven cards when the index is out of range. */
class ResetCard {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
};

struct ResetCards {
    char pad_000[0xD14];
    ResetCard *field_D14[7];
};

extern "C" {
void func_08A62E5C(ResetCards *self, int index)
{
    if (index >= 0 && index < 7) {
        self->field_D14[index]->v13();
    } else {
        int i;
        for (i = 0; i < 7; i++) {
            self->field_D14[i]->v13();
        }
    }
}
}
