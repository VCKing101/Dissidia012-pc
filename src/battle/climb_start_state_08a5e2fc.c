/* Fighter state callback, climb start: clears the climb record, marks the
 * fighter climbing, plays the climb motion and its motion pair, sets the
 * climb flags and the record's first step. */
typedef struct ClimbRecord {
    short step;
    short count;
    int active;
    float x;
    float y;
    float z;
    unsigned char pad_14[0x3C];
} ClimbRecord;

typedef struct ClimbBits {
    unsigned char b0;
    unsigned char b1;
} ClimbBits;

typedef struct ClimbHold {
    unsigned char pad_00[0xF4];
    int stage;
} ClimbHold;

typedef struct ClimbStartFighter {
    unsigned char pad_000[0x130];
    unsigned char motion[0x1E4];
    ClimbHold *holder;
    unsigned char pad_318[0x68];
    unsigned int state_flags;
    unsigned int flags;
    unsigned char pad_388[0x20];
    ClimbRecord *record;
    unsigned char pad_3ac[0x204];
    ClimbBits bits;
} ClimbStartFighter;

extern void func_08935828(void *dst, int value, int size);
extern void func_0882FA10(float x, float y, void *motion, int id, int arg);
extern void func_08834934(ClimbStartFighter *fighter, int from, int to);

void func_08A5E2FC(void *ctx, ClimbStartFighter *fighter)
{
    ClimbRecord *record = fighter->record;
    ClimbBits *bits;
    func_08935828(record, 0, sizeof(ClimbRecord));
    bits = &fighter->bits;
    bits->b1 |= 0x40;
    fighter->holder->stage = 0;
    func_0882FA10(0.0f, 0.0f, fighter->motion, 0xCA, 0);
    func_08834934(fighter, 0x65, 0x66);
    fighter->flags |= 0x1000;
    fighter->state_flags |= 2;
    record->step = 1;
    record->count = 0;
    record->active = 1;
    record->y = 0.0f;
    record->x = 0.0f;
    record->z = 0.0f;
    fighter->flags &= ~0x200;
}
