typedef struct FighterPoseEntry {
    char pad0000[0x101C];
    void *defaultEntry;     /* 0x101C */
    void *entry;            /* 0x1020 */
} FighterPoseEntry;

void *func_08B30898(FighterPoseEntry *fighter)
{
    if (fighter->entry != 0) {
        return fighter->entry;
    } else {
        return fighter->defaultEntry;
    }
}
