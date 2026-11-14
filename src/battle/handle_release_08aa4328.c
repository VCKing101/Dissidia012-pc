typedef struct HandleQuad {
    char pad_00[0x4];
    void *field_04;
    char pad_08[0x4];
    void *field_0C;
    void *field_10;
    char pad_14[0x8];
    void *field_1C;
} HandleQuad;

extern void func_08842138(void *handle);
extern void func_08AA4300(HandleQuad *self);

void func_08AA4328(HandleQuad *self)
{
    if (self->field_1C != 0) {
        func_08842138(self->field_1C);
    }
    if (self->field_10 != 0) {
        func_08842138(self->field_10);
    }
    if (self->field_0C != 0) {
        func_08842138(self->field_0C);
    }
    if (self->field_04 != 0) {
        func_08842138(self->field_04);
    }
    func_08AA4300(self);
}
