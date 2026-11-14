typedef struct PendingState {
    char pad_0000[0x8E60];
    unsigned int field_8E60;
} PendingState;

typedef struct PendingOwner {
    char pad_000[0x154];
    PendingState *field_154;
} PendingOwner;

extern void func_08A1DC4C(PendingOwner *self);

int func_08A1DD80(PendingOwner *self)
{
    int result;
    if ((self->field_154->field_8E60 & 2) != 0) {
        if ((self->field_154->field_8E60 & 0x10000) != 0) {
            self->field_154->field_8E60 = self->field_154->field_8E60 & ~0x10000;
            func_08A1DC4C(self);
            return 1;
        }
        result = 0;
        return result;
    }
    return 1;
}
