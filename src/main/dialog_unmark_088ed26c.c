typedef struct DialogState {
    char field_00;
    unsigned char open;
    char pad_02[0x140 - 2];
    unsigned int flags;
} DialogState;

extern DialogState *volatile D_089D7F00;

int func_088ED26C(void)
{
    if (D_089D7F00 == 0) {
        return 0;
    }
    if (D_089D7F00->flags & 0x20) {
        unsigned int *flags = &D_089D7F00->flags;

        *flags ^= 0x20;
    }
    return 1;
}
