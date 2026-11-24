/* Returns whether a sound may start: unique sounds (flag 4) are refused
   while the same sound and owner are already in the playing table. */
typedef struct SoundInfo {
    char pad_00[8];
    unsigned int flags;
} SoundInfo;

typedef struct PlayingEntry {
    int sound;
    int voice;
    int owner;
} PlayingEntry;

extern PlayingEntry D_09B4CFA0[64];

extern SoundInfo *func_088A0BC0(int sound);

int func_088A290C(int sound, int owner)
{
    SoundInfo *info = func_088A0BC0(sound);

    if (info != 0 && (info->flags & 4)) {
        int i;
        for (i = 0; i < 64; i++) {
            PlayingEntry *e = &D_09B4CFA0[i];
            if (e->sound == sound && e->owner == owner) {
                return 0;
            }
        }
    }
    return 1;
}
