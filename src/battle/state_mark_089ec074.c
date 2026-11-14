typedef struct StateMark {
    char pad_00[0x8];
    unsigned char active;
    char pad_09[0x3];
    int state;
    char pad_10[0x1D];
    unsigned char field_2D;
    unsigned char field_2E;
    char pad_2F[0x1D];
    unsigned char field_4C;
} StateMark;

int func_089EC074(StateMark *self)
{
    if (!self->active) {
        return -1;
    }
    switch (self->state) {
    case 2:
        if (!self->field_4C) {
            self->field_2E = 1;
        }
        break;
    case 4:
        self->field_2D = 1;
        break;
    }
    return 0;
}
