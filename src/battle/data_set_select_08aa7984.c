typedef struct DataSet {
    unsigned char active;
    char pad_01[0x3];
    char *current;
    char primary[0xD4];
    char alternate[0x10];
} DataSet;

/* Selects the alternate data (1) or the primary data. */
int func_08AA7984(DataSet *self, unsigned char useAlternate)
{
    if (self->active == 0) {
        return -1;
    }
    if (useAlternate == 1) {
        self->current = self->alternate;
    } else {
        self->current = self->primary;
    }
    return 0;
}
