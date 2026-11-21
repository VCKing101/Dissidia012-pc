#include "main/agent33_texture.h"

extern "C" {

D012ArchiveEntry *func_08869A7C(D012Archive *archive, int index);   /* archive_entry (0 if out of range) */
int func_0893585C(const char *a, const char *b);                    /* strcmp */

/* archive_find: the archive entry with this name, or 0. */
D012ArchiveEntry *func_08869AB4(D012Archive *archive, const char *name)
{
    int count = archive->header->count;
    D012ArchiveEntry *entry;

    for (int i = 0; i < count; i++) {
        entry = func_08869A7C(archive, i);
        if (entry->name == 0 || func_0893585C(name, entry->name) != 0) {
            continue;
        }
        goto found;
    }
    return 0;
found:
    return entry;
}

}
