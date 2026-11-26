/* After a load: the buffer's first two words must be the descriptor's "enabled" (1) and size, else flag 4 (broken
   data). Returns whether the flag is set. Written by 0x088AEFA0 before a save. */
#include "main/replay_types.h"
typedef struct SaveManager {
    int unk_00;
    unsigned int flags;
    int *buffer;
    D012ReplaySave *save;
} SaveManager;

int func_088AEF54(SaveManager *manager)
{
    unsigned int flags;
    D012ReplaySave *save = manager->save;
    int *buffer = manager->buffer;
    int enabled = save->enabled;
    if (buffer[0] == enabled && buffer[1] == save->size) {
        flags = manager->flags & ~4u;
    } else {
        flags = manager->flags | 4;
    }
    manager->flags = flags;
    return (flags & 4) != 0;
}
