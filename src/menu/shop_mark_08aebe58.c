typedef struct ShopState {
    char pad_0000[0x49C];
    int field_49C;
    char pad_04A0[0x1924 - 0x4A0];
    unsigned int field_1924;
} ShopState;

typedef struct ShopOwner {
    char pad_0000[0x23F0];
    ShopState *field_23F0;
} ShopOwner;

extern void func_088D9F5C(int target, int kind, int value);
extern void func_08AEBEBC(ShopOwner *self);
extern void func_08AED2E8(ShopOwner *self);

void func_08AEBE58(ShopOwner *self)
{
    if ((self->field_23F0->field_1924 & 4) != 0) {
        func_088D9F5C(self->field_23F0->field_49C, 1, 1);
    } else {
        func_088D9F5C(self->field_23F0->field_49C, 0, 1);
    }
    func_08AEBEBC(self);
    func_08AED2E8(self);
}
