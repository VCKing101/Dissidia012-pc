typedef struct FlagRow {
    unsigned char active;
    unsigned char flags[8];
} FlagRow;

/* True when at least six of the eight flags are set. */
extern "C" int func_08A82380(FlagRow *self)
{
    if (self->active == 0) {
        return 0;
    }
    int count = 0;
    for (int i = 0; i < 8; i++) {
        unsigned char *flags = self->flags;
        if (flags[i] != 0) {
            count++;
        }
    }
    return count >= 6;
}
