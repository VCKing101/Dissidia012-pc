#include "main/agent33_texture.h"

/* The view a model is drawn in (only the fields used here). */
typedef struct ModelView {
    unsigned char unk_00[0x44];
    int unk_44;                        /* +0x44 libGu value set for the draw, -1 = none */
    unsigned int flags;                /* +0x48 bit 0: ... (model state 2 on) */
} ModelView;

extern void func_0886A6A4(D012ModelHolder *holder);                      /* model_lazy_build */
extern int func_0886A298(D012ModelHolder *holder, ModelView *view, int one);
extern void func_089B0D40(void *model, int state, int value);            /* model state set */
extern void func_089B0D64(void *model, int state, int value);
extern void func_089B0D88(void *model, int state, int value);
extern void func_089B0CC4(void *model, ModelView *view);
extern void func_089B4798(void *model);                                  /* draw the model */
extern void func_089A296C(int mode);                                     /* sceGumMatrixMode */
extern void func_089A250C(void);                                         /* sceGumLoadIdentity */
extern void func_089A0C20(int value);
extern void func_089A0E60(int u, int v);                                 /* sceGuTexWrap */
extern void func_089A0384(int state);                                    /* sceGuEnable */
extern void func_089A03DC(int state);                                    /* sceGuDisable */

/* model_draw: draw a holder's current model (building it first if that is still due). */
void func_0886A3F4(D012ModelHolder *holder, ModelView *view)
{
    holder->flags &= ~D012_MODEL_BUILD_ALT;
    func_0886A6A4(holder);
    func_089B0D40(holder->current_part_addr->model_addr, 1, 0);
    func_089B0D64(holder->current_part_addr->model_addr, 4, 0);
    func_089B0D40(holder->current_part_addr->model_addr, 4, -1);
    if (view != 0 && (view->flags & 1)) {
        func_089B0D40(holder->current_part_addr->model_addr, 2, -1);
    } else {
        func_089B0D40(holder->current_part_addr->model_addr, 2, 0);
    }
    func_089A296C(2);
    func_089A250C();
    func_089B0CC4(holder->current_part_addr->model_addr, view);
    if (func_0886A298(holder, view, 1)) {
        func_089B0D88(holder->current_part_addr->model_addr, 1, 0);
        func_089A0C20(view->unk_44);
        func_089A0E60(0, 0);
        func_089A0384(0x15);           /* GU_FRAGMENT_2X */
        func_089B4798(holder->current_part_addr->model_addr);
        func_089A03DC(0x15);
    }
    if (view->unk_44 != -1) {
        func_089A0C20(-1);
    }
}
