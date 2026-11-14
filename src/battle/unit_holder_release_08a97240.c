typedef struct UnitHolder {
    char pad_000[0x158];
    void *field_158;
} UnitHolder;

extern void func_08842138(void *object);
extern void func_08A971A8(UnitHolder *self);

void func_08A97240(UnitHolder *self)
{
    if (self->field_158 != 0) {
        func_08842138(self->field_158);
    }
    func_08A971A8(self);
}
