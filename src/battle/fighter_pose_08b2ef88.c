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
extern void func_08834934(FighterPose *fighter, int slot, int motion);
extern void func_08B333F0(FighterPose *fighter);
extern void func_08833C98(FighterPose *fighter, float blend);

void func_08B2EF88(FighterPose *fighter, float blend)
{
    PoseEntry *entry = func_08B30898(fighter);

    if (entry != 0 && entry->motion != 0xFFFF && entry->motion != 0) {
        func_08834934(fighter, -1, entry->motion);
        blend = 0.0f;
        fighter->pose = entry->motion;
        fighter->state |= 8;
        fighter->state |= 0x10;
    }
    func_08B333F0(fighter);
    func_08833C98(fighter, blend);
}
