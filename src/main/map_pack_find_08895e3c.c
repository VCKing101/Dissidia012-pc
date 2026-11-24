/* Finds the map's message file and its two battle sound files in a pack and
   stores their data pointers and sizes. */
typedef struct PackEntry {
    char pad_00[4];
    void *data;
    int size;
} PackEntry;

typedef struct MapFiles {
    void *messages;
    void *ex_sound;
    int ex_size;
    void *finish_sound;
    int finish_size;
} MapFiles;

typedef struct SoundPack SoundPack;

extern char D_089CA938[];
extern char D_089CA940[];
extern char D_089CA954[];

extern void func_0893476C(char *buf, const char *fmt, const char *name);
extern PackEntry *func_08869AB4(SoundPack *pack, const char *name);

void func_08895E3C(MapFiles *self, const char *map, SoundPack *pack)
{
    char path[256];
    PackEntry *entry;

    func_0893476C(path, D_089CA938, map);
    entry = func_08869AB4(pack, path);
    if (entry != 0 && entry->size != 0) {
        self->messages = entry->data;
    }
    entry = func_08869AB4(pack, D_089CA940);
    if (entry != 0 && entry->size != 0) {
        self->ex_sound = entry->data;
        self->ex_size = entry->size;
    }
    entry = func_08869AB4(pack, D_089CA954);
    if (entry != 0 && entry->size != 0) {
        self->finish_sound = entry->data;
        self->finish_size = entry->size;
    }
}
