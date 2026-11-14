extern unsigned char D_089D5F1F;
extern int func_089E9C64(void);

typedef struct ModeState {
    int field_00;
} ModeState;

int func_089E8EF0(ModeState *self)
{
    if (D_089D5F1F) {
        return 0;
    }
    if (func_089E9C64() == 1 && self->field_00 == 1) {
        return 0;
    }
    return self->field_00;
}
