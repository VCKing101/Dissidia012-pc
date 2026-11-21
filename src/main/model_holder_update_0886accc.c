#include "main/agent33_texture.h"

extern void func_0886A908(D012ModelHolder *holder);
extern void func_0886A54C(D012ModelHolder *holder);
extern void func_0886A180(D012ModelHolder *holder);
extern void func_0886D618(D012ModelHolder *holder);
extern void func_0886E86C(D012ModelHolder *holder, void *view, int mode);  /* model_build: precompile the draw list */
extern void func_0886A24C(D012ModelHolder *holder, void *view);            /* draw through the precompiled list */
extern void func_0886A3F4(D012ModelHolder *holder, void *view);            /* model_draw */
extern void func_0886CAA0(D012ModelHolder *holder);                        /* model_holder_release */

/* model_holder_update: per-frame work of a model holder: animate, then draw it, or (on the
 * first frames, before it is built) build it; counts down a pending release. */
void func_0886ACCC(D012ModelHolder *holder, void *view, int mode)
{
    func_0886A908(holder);
    func_0886A54C(holder);
    func_0886A180(holder);
    if (holder->current_part_addr->unk_00 != 0) {
        func_0886D618(holder);
    }
    if (((holder->flags & 1) || mode == 0) && holder->release_delay == 0) {
        if (holder->draw_lists[0] == 0) {
            func_0886E86C(holder, view, mode);
        }
        if (holder->draw_lists[0] != 0) {
            func_0886A24C(holder, view);
        }
    } else {
        func_0886A3F4(holder, view);
    }
    if (holder->release_delay != 0) {
        holder->release_delay--;
        if (holder->release_delay == 0) {
            func_0886CAA0(holder);
        }
    }
}
