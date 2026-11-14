typedef struct ActionState {
    char pad_00[0x54];
    void *field_54;
    char pad_58[0x1C];
    void *field_74;
} ActionState;

extern void func_08842138(void *object);
extern void func_08A98120(ActionState *self);

void func_08A981C0(ActionState *self)
{
    if (self->field_74 != 0) {
        func_08842138(self->field_74);
    }
    if (self->field_54 != 0) {
        func_08842138(self->field_54);
    }
    func_08A98120(self);
}
