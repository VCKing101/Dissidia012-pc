/* Deselects all seven cards, then selects the current one with its highlight mode. */
class SelectCard {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3(int mode);
};

struct SelectCards {
    char pad_000[0xD14];
    SelectCard *field_D14[7];
    char pad_D30[0xD90 - 0xD30];
    int field_D90;
};

extern "C" int func_08A62F88(SelectCards *self);

extern "C" {
void func_08A62EF0(SelectCards *self)
{
    int i;
    int current;
    int mode;
    for (i = 0; i < 7; i++) {
        self->field_D14[i]->v2();
    }
    current = self->field_D90;
    mode = func_08A62F88(self);
    self->field_D14[current]->v3(mode);
}
}
