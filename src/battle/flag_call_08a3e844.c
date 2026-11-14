typedef struct FlagCall3E844 {
    char pad_000[0x278];
    int field_278;
} FlagCall3E844;

extern void func_08A3F448(FlagCall3E844 *self);

void func_08A3E844(FlagCall3E844 *self)
{
    if (!self->field_278) {
        return;
    }
    func_08A3F448(self);
}
