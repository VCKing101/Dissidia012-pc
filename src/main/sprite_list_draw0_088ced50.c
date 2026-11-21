#include "main/agent33_texture.h"

/* libGu */
extern void func_089A0384(int state);                    /* sceGuEnable */
extern void func_089A03DC(int state);                    /* sceGuDisable */
extern void func_089A113C(int func, int value, int mask); /* sceGuAlphaFunc */
extern void func_089A0E20(int tfx, int tcc);             /* sceGuTexFunc */
extern void func_089A0AC4(int value);

/* sprite_list_draw: draw every sprite of a list (binds each one's texture). */
extern void func_088D16F4(D012TexManager *mgr, D012SpriteList *list);

/* Draw the manager's sprite list 0 with the 2D render state (texturing, blending,
 * alpha test > 0, colour doubling, no culling). */
void func_088CED50(D012TexManager *mgr)
{
    if (!mgr->draw_enabled) {
        return;
    }
    if (mgr->lists[0].head == 0) {
        return;
    }
    func_089A0384(9);                    /* GU_TEXTURE_2D */
    func_089A0384(4);                    /* GU_BLEND */
    func_089A0384(0x15);                 /* GU_FRAGMENT_2X */
    func_089A113C(6, 0, 0xFF);           /* GU_GREATER */
    func_089A0384(0);                    /* GU_ALPHA_TEST */
    func_089A0384(1);                    /* GU_DEPTH_TEST */
    func_089A03DC(5);                    /* GU_CULL_FACE */
    func_089A0E20(0, 1);                 /* GU_TFX_MODULATE, GU_TCC_RGBA */
    func_089A0AC4(0x14);
    func_088D16F4(mgr, &mgr->lists[0]);
    func_089A03DC(0x15);                 /* GU_FRAGMENT_2X */
    func_089A113C(6, 4, 0xFF);
}
