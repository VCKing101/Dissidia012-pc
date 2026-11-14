extern "C" {
typedef struct SoundEntry { int sound; int id; } SoundEntry;
typedef struct SoundBank { char pad_00[0xC]; int count; SoundEntry *entries; } SoundBank;
extern int func_08828E8C(SoundBank *b);
extern void *func_08899300(void);
extern void func_088981B4(void *table, int sound, int a, int b);
int func_08828DD8(SoundBank *b)
{
    if (func_08828E8C(b) == 0) {
        return -1;
    }
    void *table = func_08899300();
    for (int i = 0; i < b->count; i++) {
        if (b->entries[i].id == 0) {
            continue;
        }
        func_088981B4(table, b->entries[i].sound, 0, 1);
    }
    return 0;
}
}
