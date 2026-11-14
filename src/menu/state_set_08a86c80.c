typedef struct A6StInner {
    char pad_000[0x910];
    int field_910;
    char pad_914[0x1BC];
    int field_AD0;
} A6StInner;

typedef struct A6StSelf {
    char pad_00[0x14C];
    A6StInner *field_14C;
} A6StSelf;

void func_08A876DC(A6StSelf *self);

void func_08A86C80(A6StSelf *self) {
    self->field_14C->field_910 = 2;
    self->field_14C->field_AD0 = -1;
    func_08A876DC(self);
}
