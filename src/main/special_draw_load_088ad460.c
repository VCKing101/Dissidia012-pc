#include "main/agent33_texture.h"

/* The menu object that draws the "SpecialDraw" picture (only the fields used here). */
typedef struct SpecialDrawOwner {
    unsigned char unk_000[0xED8];
    int gim_size;                      /* +0xED8 */
    void *gim;                         /* +0xEDC GIM file in memory */
    unsigned char unk_EE0[4];
    D012ImageObject *image;            /* +0xEE4 */
    int unk_EE8;                       /* +0xEE8 */
    unsigned char unk_EEC[0xC];
    unsigned int flags;                /* +0xEF8 */
    unsigned char unk_EFC[8];
    void *draw_task;                   /* +0xF04 */
} SpecialDrawOwner;

extern int D_089D69A8;
extern char D_089CB410[];              /* "SpecialDraw" */
extern void *func_08882274(int id);    /* task scheduler of a group */
/* task_new(scheduler, priority, order, function, name) */
extern void *func_088823C4(void *scheduler, int priority, int order, void (*fn)(void), const char *name);
extern void func_088AD1E8(void);       /* the draw task */

/* special_draw_load: make an image object from the owner's GIM and start the draw task. */
void func_088AD460(SpecialDrawOwner *self)
{
    D012ImageObject *image;

    self->image = func_0899B9F0(0);
    if (func_0899BB68(self->image, self->gim, self->gim_size, 0) == 0) {
        func_0899BB1C(self->image);
        self->image = 0;
    }
    image = self->image;
    if (image != 0) {
        self->unk_EE8 = 0;
        self->flags |= 0x100;
        self->draw_task = func_088823C4(func_08882274(D_089D69A8), -1, 86000, func_088AD1E8, D_089CB410);
    }
}
