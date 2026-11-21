#include "main/agent33_texture.h"

/* A file held in memory by the preloaded-file cache (only the fields read here). */
typedef struct FileCacheEntry {
    char name[0x50];                   /* +0x00 */
    void *data;                        /* +0x50 */
    int size;                          /* +0x54 */
    char unk_58[0x10];
    int id;                            /* +0x68 */
} FileCacheEntry;


extern unsigned char D_089D69DF;       /* cache switched off */
extern unsigned char D_089D69DC;       /* cache filled */
extern char D_09B49130[];              /* the cache's table */
extern FileCacheEntry *func_08883D0C(void *table, const char *path);

/* file_cache_find: look a (localised) path up in the preloaded-file cache.
 * Returns 1 and fills `hit` when the file is held in memory. */
int func_08883538(D012FileCacheHit *hit, const char *path)
{
    FileCacheEntry *entry;

    if (D_089D69DF != 0) {
        return 0;
    } else if (D_089D69DC == 0) {
        return 0;
    } else {
        entry = func_08883D0C(D_09B49130, path);
        if (entry != 0) {
            hit->data = entry->data;
            hit->size = entry->size;
            hit->id = entry->id;
            return 1;
        } else {
            return 0;
        }
    }
}
