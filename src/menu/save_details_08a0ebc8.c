/* Screen11510: loads the details page of the save in place +0x1140 of
   the root's save list (place 25 is the empty one): its progress pairs
   as done/left, its two totals and its completion in percent; then
   refreshes (refresh set) or waits (step 2). Standalone: the save list
   is not in the region header's RootWork, and retail reads `refresh`
   as unsigned. */
typedef struct SaveRecord29 {
    unsigned short flags;
    char pad_02[0x12];
    unsigned int time;      /* +0x14 */
    char pad_18[0x220];
    unsigned short stage_max;  /* +0x238 */
    unsigned short stage;      /* +0x23A */
    unsigned short story_max;  /* +0x23C */
    unsigned short story;      /* +0x23E */
    unsigned short total;      /* +0x240 */
    unsigned short done;       /* +0x242 */
    int field_244;
    int field_248;
} SaveRecord29;

typedef struct SaveList29 {
    unsigned int flags;
    char pad_04[0x4];
    SaveRecord29 records[26];
} SaveList29;

typedef struct SaveRootWork29 {
    char pad_0000[0x2BB8];
    SaveList29 list; /* +0x2BB8 */
} SaveRootWork29;

typedef struct SaveRoot29 {
    char pad_000[0x154];
    SaveRootWork29 *work;
} SaveRoot29;

typedef struct DetailWork29 {
    char pad_0000[0x110C];
    int step;               /* +0x110C */
    unsigned char empty;    /* +0x1110 */
    char pad_1111[0x3];
    int stage;              /* +0x1114 */
    int stage_left;
    int story;
    int story_left;
    int field_1124;
    int field_1128;
    int total;              /* +0x112C */
    int done;
    int left;
    int percent;            /* +0x1138 */
    char pad_113C[0x4];
    int place;              /* +0x1140 */
} DetailWork29;

typedef struct DetailScreen29 {
    void *vtable;
    char pad_04[0x131C];
    DetailWork29 *work; /* +0x1320 */
} DetailScreen29;

extern SaveRoot29 *D_08A23FF0;
extern void func_08A0ECF0(DetailScreen29 *self);

void func_08A0EBC8(DetailScreen29 *self, unsigned char refresh)
{
    int place = self->work->place;
    SaveList29 *list = &D_08A23FF0->work->list;
    SaveRecord29 *record = &list->records[place];
    int percent;

    self->work->empty = place == 25;
    self->work->stage = record->stage;
    self->work->stage_left = record->stage_max - record->stage;
    self->work->story = record->story;
    self->work->story_left = record->story_max - record->story;
    self->work->field_1124 = record->field_244;
    self->work->field_1128 = record->field_248;
    self->work->total = record->total;
    self->work->done = record->done;
    self->work->left = record->total - record->done;
    percent = 0;
    if (record->total != 0) {
        percent = record->done * 100 / record->total;
    }
    self->work->percent = percent;
    if (refresh) {
        func_08A0ECF0(self);
        self->work->step = 0;
    } else {
        self->work->step = 2;
    }
}
