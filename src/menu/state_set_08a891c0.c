typedef struct A6StInner2 {
    char pad_000[0x418];
    int field_418;
} A6StInner2;

typedef struct A6StSelf2 {
    char pad_00[0x30C];
    A6StInner2 *field_30C;
} A6StSelf2;

void func_08A891E4(A6StSelf2 *self);

void func_08A891C0(A6StSelf2 *self) {
    self->field_30C->field_418 = 2;
    func_08A891E4(self);
}
