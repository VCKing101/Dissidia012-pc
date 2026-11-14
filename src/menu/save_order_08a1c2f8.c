/* MenuRoot: builds the order of the save list: every record in use
   (flag 1; with list flag 2 only those with flag 8), then the "new
   save" place 25, each moved into place by func_08A1C208. Standalone:
   the list is not in the region header's RootWork. */
typedef struct SaveRecord29 {
    unsigned short flags;
    char pad_02[0x12];
    unsigned int time; /* +0x14 */
    char pad_18[0x234];
} SaveRecord29;

typedef struct SaveList29 {
    char pad_0000[0x2BB8];
    unsigned int flags;         /* +0x2BB8 */
    char pad_2BBC[0x4];
    SaveRecord29 records[26];   /* +0x2BC0 */
    int count;                  /* +0x6778 */
    short order[0x1C];          /* +0x677C */
} SaveList29;

typedef struct SaveRoot29 {
    char pad_000[0x154];
    SaveList29 *work;
} SaveRoot29;

extern void func_08A1C208(SaveRoot29 *self, int n);

void func_08A1C2F8(SaveRoot29 *self)
{
    int i;
    unsigned short flags;
    unsigned int only_marked;

    self->work->count = 0;
    for (i = 0; i < 25; i++) {
        flags = self->work->records[i].flags;
        if (flags & 1) {
            only_marked = self->work->flags & 2;
            if ((only_marked != 0 && (flags & 8) != 0) || only_marked == 0) {
                self->work->order[self->work->count] = i;
                self->work->count++;
            }
        }
    }
    self->work->order[self->work->count] = 25;
    self->work->count++;
    for (i = 0; i < self->work->count; i++) {
        func_08A1C208(self, i);
    }
}
