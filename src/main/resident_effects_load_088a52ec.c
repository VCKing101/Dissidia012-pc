/* Loads the resident effect pack from a pack file when it is present. */
typedef struct PackEntry {
    char pad_00[4];
    void *data;
    int size;
} PackEntry;

typedef struct SoundPack SoundPack;

extern char D_089CAF28[];
extern char D_089CAF38[];

extern PackEntry *func_08869AB4(SoundPack *pack, const char *name);
extern void func_088A573C(void *self, const char *name, void *data, int size);

void func_088A52EC(void *self, SoundPack *pack)
{
    PackEntry *entry = func_08869AB4(pack, D_089CAF28);

    if (entry == 0) {
        return;
    }
    func_088A573C(self, D_089CAF38, entry->data, entry->size);
}
