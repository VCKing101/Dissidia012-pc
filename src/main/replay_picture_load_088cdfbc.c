#include "main/agent33_texture.h"

/* The replay screen's "CO" picture. */
typedef struct ReplayPicture {
    unsigned char loaded;              /* +0x00 */
    unsigned char pad_01[3];
    D012ImageObject *image;            /* +0x04 */
} ReplayPicture;

extern int D_089D69A8;
extern char D_089CBD74[];              /* "replay_CO.gim" */
extern char D_089CBD84[];              /* task name */
extern void func_088CDF80(ReplayPicture *self);   /* release the old picture */
extern D012ArchiveEntry *func_08869AB4(D012Archive *archive, const char *name); /* archive_find */
/* task_new_in(group, priority, order, function, name) */
extern void *func_0888227C(int group, int priority, int order, void (*fn)(void), const char *name);
extern void func_088CE0C0(void);       /* the draw task */

/* replay_picture_load: load "replay_CO.gim" from an archive into an image object and
 * start its draw task. Returns 0, or -1 (no archive), -2 (no such entry), -3 (no memory). */
int func_088CDFBC(ReplayPicture *self, D012Archive *archive)
{
    D012ArchiveEntry *entry;

    func_088CDF80(self);
    if (archive == 0) {
        return -1;
    }
    entry = func_08869AB4(archive, D_089CBD74);
    if (entry == 0) {
        return -2;
    }
    self->image = func_0899B9F0(0);
    if (self->image == 0) {
        return -3;
    }
    func_0899BB68(self->image, entry->data, entry->size, 0);
    func_0888227C(D_089D69A8, -1, 0x151E4, func_088CE0C0, D_089CBD84);
    self->loaded = 1;
    return 0;
}
