typedef struct Flags { char data_00[0x84]; unsigned int bits[32]; } Flags;

int func_088C54BC(Flags *f, int i)
{
    if (i < 0) {
        return 0;
    }
    if (i < 0x400) {
        int word = i / 32;
        int bit = i % 32;
        return (*(unsigned int *)((char *)f->bits + word * 4) & (1 << bit)) == 0;
    }
    return 0;
}
