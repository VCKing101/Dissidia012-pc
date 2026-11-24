/* Releases every loaded menu sound entry, frees the entry list and clears
   the count. */
extern "C" {
struct SoundEntry {
    char pad_00[0x24];
    int handle;
};

struct Menu {
    int heap;
    char pad_004[0x72C - 4];
    int count;
    SoundEntry *entries;
};

struct SoundBank {
    int loaded;
};

SoundBank *func_08899300(void);
void func_088989F4(SoundBank *bank, SoundEntry *entry, int handle);
void func_0885C538(int heap, void *block, int flag);

void func_08892DCC(Menu *self)
{
    for (int i = 0; i < self->count; i++) {
        if ((unsigned char)(func_08899300()->loaded != 0) == 1) {
            func_088989F4(func_08899300(), &self->entries[i], self->entries[i].handle);
        }
    }
    func_0885C538(self->heap, self->entries, 0);
    self->entries = 0;
    self->count = 0;
}

}
