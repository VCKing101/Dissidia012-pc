/* Opens the loaded map sound pack for a map and registers its battle sound
   bank; returns whether the pack was valid. */
typedef struct PackEntry {
    char pad_00[4];
    void *data;
    int size;
} PackEntry;

typedef struct SoundPack {
    char pad[0x10];
} SoundPack;

extern int D_089D6A90;
extern int D_089D6A94;
extern int D_089D6C80;
extern int D_089D6AF0;
extern int D_089D6AE8;
extern SoundPack D_09B491E0;
extern char D_089CA59C[];

extern void *func_088A0CF8(int bank);
extern int func_08869954(SoundPack *pack, void *data);
extern void func_08896854(int p, int map, SoundPack *pack);
extern void func_08895E3C(int p, int map, SoundPack *pack);
extern PackEntry *func_08869AB4(SoundPack *pack, const char *name);
extern int func_088A0DBC(int bank, const char *name, void *data, int size, int flag);

int func_0888E4D4(int map)
{
    if (D_089D6A90 != 0) {
        void *data = func_088A0CF8(D_089D6A90);
        if (func_08869954(&D_09B491E0, data) == 1) {
            int bank = D_089D6C80;
            const char *name;
            PackEntry *entry;

            func_08896854(D_089D6AF0, map, &D_09B491E0);
            func_08895E3C(D_089D6AE8, map, &D_09B491E0);
            name = D_089CA59C;
            entry = func_08869AB4(&D_09B491E0, name);
            if (entry != 0 && entry->size != 0) {
                D_089D6A94 = func_088A0DBC(bank, name, entry->data, entry->size, 0);
            }
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}
