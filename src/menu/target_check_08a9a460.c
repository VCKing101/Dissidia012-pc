typedef struct TargetState {
    char pad_0000[0x92C];
    int field_92C;
    char pad_0930[0x81B4 - 0x930];
    unsigned int field_81B4;
} TargetState;

typedef struct TargetOwner {
    char pad_000[0x154];
    TargetState *field_154;
} TargetOwner;

extern int func_08A80E34(int target);

int func_08A9A460(TargetOwner *self)
{
    TargetState *state = self->field_154;
    unsigned int flags = state->field_81B4;
    if (((flags & 0x20) != 0 || (flags & 0x80) != 0) && (flags & 0x1000) != 0) {
        return 0;
    }
    if (state->field_92C != 0) {
        return func_08A80E34(state->field_92C);
    }
    return 0;
}
