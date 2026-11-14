typedef struct MotionEntry {
    char data[0x28];
} MotionEntry;

typedef struct MotionBank {
    char pad_00[0x10];
    char table_10[0x70];
    char table_80[0x70];
    MotionEntry *entries;
} MotionBank;

extern int func_08A23454(MotionBank *self);
extern int func_08891684(MotionEntry *entry);
extern int func_08A2409C(MotionBank *self, unsigned int side, void *table, int index, unsigned char loop);

/* Plays side's current motion from the first table; 0 when there is nothing to play. */
int func_08A23488(MotionBank *self, unsigned int side, unsigned char loop)
{
    int index;
    if (func_08A23454(self) == 0) {
        return 0;
    }
    if (side >= 2) {
        return 0;
    }
    index = func_08891684(&self->entries[side]);
    if (index < 0) {
        return 0;
    }
    return func_08A2409C(self, side, self->table_10, index, loop);
}

/* Plays motion index from the second table; 0 for a bad side or index. */
int func_08A2352C(MotionBank *self, unsigned int side, int index, unsigned char loop)
{
    if (func_08A23454(self) == 0) {
        return 0;
    }
    if (side >= 2 || index < 0) {
        return 0;
    }
    return func_08A2409C(self, side, self->table_80, index, loop);
}
