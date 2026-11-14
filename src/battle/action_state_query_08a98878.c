typedef struct ActionState {
    unsigned char active;
    char pad_01[0x7];
    signed char field_08;
    signed char field_09;
    char pad_0A[0x66];
    unsigned char field_70;
} ActionState;

extern int func_08899944(void);

int func_08A98878(ActionState *self)
{
    if (self->active == 0) {
        return 0;
    }
    if (func_08899944() == 1) {
        return 0;
    }
    return self->field_08 != 0;
}
