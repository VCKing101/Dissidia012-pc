extern "C" {
#include "main/agent33_texture.h"

typedef struct ReplayPicture {
    unsigned char loaded;              /* +0x00 */
    unsigned char pad_01[3];
    D012ImageObject *image;            /* +0x04 */
    unsigned char visible;             /* +0x08 drawn once, then cleared */
} ReplayPicture;

/* A 2D sprite vertex: texture u, v, then x, y, z (GU_TEXTURE_32BITF | GU_VERTEX_32BITF). */
typedef struct SpriteVertex {
    float u, v;
    float x, y, z;
} SpriteVertex;

extern float D_089CBD68;               /* picture width */
extern float D_089CBD6C;               /* picture height */
extern float D_089CBD70;               /* depth */
extern float D_089D7818;               /* screen x */
extern float D_089D781C;               /* screen y */

extern int func_088CE2A0(ReplayPicture *self);
extern void *func_089A00F4(int size);                                   /* sceGuGetMemory */
extern int func_089A04E8(void);                                         /* save GE state */
extern void func_089A12A4(int op, int src, int dst, int srcfix, int dstfix); /* sceGuBlendFunc */
extern void func_089A0384(int state);                                   /* sceGuEnable */
extern void func_089A03DC(int state);                                   /* sceGuDisable */
extern void func_089A0C8C(int color);
extern void func_089A119C(int on);
extern void func_089A0520(int prim, int vtype, int count, const void *indices, const void *vertices); /* sceGuDrawArray */
extern void func_0899FDC4(int saved);                                    /* restore GE state */

/* replay_picture_draw: draw the replay "CO" picture once as a two-vertex sprite with
 * alpha blending (image bound through img_bind). Returns 0, -1 (not ready), -2 (no list memory). */
int func_088CE0E4(ReplayPicture *self)
{
    SpriteVertex *vertices;
    int saved;

    if (!func_088CE2A0(self)) {
        return -1;
    }
    if (!self->visible) {
        return 0;
    }
    vertices = (SpriteVertex *)func_089A00F4(sizeof(SpriteVertex) * 2);
    if (vertices == 0) {
        return -2;
    }
    for (int i = 0; i < 2; i++) {
        SpriteVertex *v = &vertices[i];
        float f = (float)i;

        v->u = D_089CBD68 * f;
        v->v = D_089CBD6C * f;
        v->x = D_089D7818 + D_089CBD68 * f;
        v->y = D_089D781C + D_089CBD6C * f;
        v->z = D_089CBD70;
    }
    saved = func_089A04E8();
    func_089A12A4(0, 2, 3, 0, 0);      /* GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA */
    func_089A0384(4);                  /* GU_BLEND */
    func_0899D2F8(self->image);
    func_089A0384(9);
    func_089A03DC(0x15);
    func_089A03DC(1);
    func_089A03DC(5);
    func_089A03DC(10);
    func_089A03DC(0);
    func_089A03DC(0x11);
    func_089A03DC(3);
    func_089A03DC(7);
    func_089A03DC(6);
    func_089A0C8C(-1);
    func_089A119C(1);
    func_089A0520(6, 0x800183, 2, 0, vertices);   /* GU_SPRITES, 2D transform */
    func_089A119C(0);
    func_0899FDC4(saved);
    self->visible = 0;
    return 0;
}
}
