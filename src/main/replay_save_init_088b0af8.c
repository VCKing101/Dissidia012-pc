/* Sets up a save object for REPLAY.BIN: the descriptor at +0x2B84 (enabled, size 0xBEA0, file name, folder stem
   "Replay", the replay save key, the icons, the callback 0x088B0CAC), the buffer cleared, then the savedata
   parameters (0x088B04AC). Used for the save and for the load. */
#include "main/replay_types.h"
extern char D_089CB718[], D_089CB724[], D_089CB72C[], D_089CB740[];
extern unsigned char D_089CB708[];
extern int D_089D7260;
extern void func_088B0CAC(void);
extern void func_08935828(void *, int, unsigned);
extern void func_088B04AC(int, void *, void *, void *, void *);

void func_088B0AF8(D012ReplaySave *s)
{
    unsigned char *buf;
    s->enabled = 1;
    s->size = D012_REPLAY_FILE_SIZE;
    s->file = D_089CB718;
    s->title = D_089CB724;
    s->key = D_089CB708;
    s->icon = D_089CB72C;
    s->icon_new = D_089CB740;
    s->callback = func_088B0CAC;
    buf = s->buffer;
    func_08935828(buf, 0, D012_REPLAY_FILE_SIZE);
    func_088B04AC(D_089D7260, s, buf, buf, s);
}
