typedef struct ActionState {
    unsigned char active;
    char pad_01[0x9];
    short field_0A;
    int field_0C;
    int field_10;
} ActionState;

extern int func_08899944(void);

int func_08A989BC(ActionState *self)
{
    if (self->active == 0) {
        return 0;
    }
    if (func_08899944() == 1) {
        return 0;
    }
    return self->field_0A;
}
