typedef struct ActionState {
    unsigned char active;
    char pad_01[0x3];
    int field_04;
    char pad_08[0x18];
    unsigned char field_20;
    char pad_21[0x4F];
    unsigned char field_70;
} ActionState;

int func_08A98400(ActionState *self)
{
    if (self->active == 0) {
        return -1;
    }
    self->field_04 = 0;
    self->field_70 = 0;
    self->field_20 = 0;
    return 0;
}
