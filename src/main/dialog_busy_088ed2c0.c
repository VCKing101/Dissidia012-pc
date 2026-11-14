typedef struct DialogState {
    char field_00;
    unsigned char open;
    char pad_02[0x140 - 2];
    unsigned int flags;
} DialogState;

extern DialogState *D_089D7F00;

int func_088ED2C0(void)
{
    if ((D_089D7F00->flags & 1) != 0) {
        return 1;
    }
    return 0;
}
