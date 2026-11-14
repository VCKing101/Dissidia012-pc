/* Screen0B6A0: 1 when every entry is either an unlocked kind-6 entry or a
   free kind-2 entry still above the pull count (and there is at least
   one of the latter); 0 otherwise or when there are no entries. */
typedef struct PendingEntry29 {
    unsigned char kind;
    char pad_01[0x3];
    unsigned int flags;
    char pad_08[0xC];
    short limit;        /* +0x14 */
    char pad_16[0x6E];
} PendingEntry29;

typedef struct PendingWork29 {
    char pad_0000[0x2731];
    signed char count;          /* +0x2731 */
    char pad_2732[0x2];
    unsigned short pulls;       /* +0x2734 */
    char pad_2736[0x296];
    PendingEntry29 entries[9];  /* +0x29CC */
} PendingWork29;

typedef struct PendingScreen29 {
    char pad_000[0x14C];
    PendingWork29 *work;
} PendingScreen29;

int func_089FB024(PendingScreen29 *self)
{
    PendingWork29 *work = self->work;
    int count = work->count;
    int i;
    int found;
    int other;

    if (count == 0) {
        return 0;
    }
    i = 0;
    found = 0;
    other = 0;
    for (; i < count; i++) {
        if (work->entries[i].kind != 6 || (work->entries[i].flags & 0x8000) != 0) {
            if (work->entries[i].kind == 2 && (work->entries[i].flags & 0x3000) == 0 &&
                work->pulls < work->entries[i].limit) {
                found = 1;
            } else {
                other = 1;
            }
        }
    }
    return other ? 0 : found != 0;
}
