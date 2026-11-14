typedef struct PoseEntry {
    char pad00[0x10];
    unsigned short motion;  /* 0x10 */
    unsigned short sound;   /* 0x12 */
} PoseEntry;

typedef struct FighterPose {
    char pad000[0x130];
    char voice[0x250];      /* 0x130 */
    unsigned int state;     /* 0x380 */
    char pad384[0x1BC];
    unsigned short pose;    /* 0x540 */
} FighterPose;

extern PoseEntry *func_08B30898(FighterPose *fighter);
extern void func_0882FA10(void *voice, int sound, int flags, float x, float y);

void func_08B2F054(FighterPose *fighter)
{
    PoseEntry *entry = func_08B30898(fighter);

    if (entry != 0 && entry->sound != 0xFFFF && entry->sound != 0) {
        func_0882FA10(fighter->voice, entry->sound, 0, 0.0f, 0.0f);
        fighter->pose = entry->sound;
        fighter->state |= 8;
        fighter->state |= 0x10;
    }
}
