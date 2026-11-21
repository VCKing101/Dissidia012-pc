#include "main/agent33_texture.h"
extern "C" {

typedef struct ModelView0886A298 {
    unsigned char unk_00[0x40];
    float alpha;                       /* +0x40 1.0 = opaque */
    int unk_44;                        /* +0x44 */
    unsigned int flags;                /* +0x48 bit 2: additive (destination kept) */
} ModelView0886A298;

extern void func_089B0D88(void *model, unsigned int state, unsigned int value);
extern void func_089A12A4(int op, int src, int dst, unsigned int srcfix, unsigned int dstfix); /* sceGuBlendFunc */

}
struct Alpha {
    /* The same 8-bit value in all four bytes (a fixed blend colour). */
    static unsigned int gray(unsigned int a) {
        unsigned int c = a << 16 | a << 24;
        c |= a;
        c |= a << 8;
        return c;
    }
    static int clamp(int v) {
        if (v < 0) {
            return 0;
        }
        if (0xFF < v) {
            return 0xFF;
        }
        return v;
    }
};
extern "C" {
/* model_set_alpha: set up blending for a model drawn at the view's alpha (fixed-colour blend,
 * GU_FIX both ways; additive when the view's flag 4 is set). Returns the alpha (0..255); 0xFF
 * when the model is drawn opaque. model_draw only draws when this is not 0. */
int func_0886A298(D012ModelHolder *holder, ModelView0886A298 *view, unsigned char keep_blend)
{
    if (view != 0 && view->alpha < 1.0f) {
        int alpha = (int)(view->alpha * 255.0f);
        unsigned int color;

        alpha = Alpha::clamp(alpha);
        func_089B0D88(holder->current_part_addr->model_addr, 0x200000, 0);
        color = Alpha::gray(alpha);
        unsigned int inverse = -1 - color;
        if ((view->flags & 4) == 0) {
            func_089A12A4(0, 10, 10, color, inverse);   /* GU_ADD, GU_FIX, GU_FIX */
        } else {
            func_089A12A4(0, 10, 10, color, 0xFFFFFFFF);
        }
        return alpha;
    }
    if (keep_blend) {
        func_089B0D88(holder->current_part_addr->model_addr, 0x200000, 0x200000);
    } else {
        func_089B0D88(holder->current_part_addr->model_addr, 0x200000, 0);
        func_089A12A4(0, 10, 10, 0xFFFFFFFF, 0);
    }
    return 0xFF;
}
}
