#include "main/agent33_texture.h"

struct SpriteStock {
    /* Take the next free sprite instance from the manager's stock ring. */
    static D012Sprite *take(D012TexManager *mgr) {
        D012Sprite *sprite = (D012Sprite *)mgr->sprite_take->object_addr;
        mgr->sprite_take->object_addr = 0;
        mgr->sprite_take = mgr->sprite_take->next_addr;
        return sprite;
    }
};

extern "C" {

/* seq_find: the seq (layout) object of (group, id), addref'd. */
void *func_088D0608(D012TexManager *mgr, unsigned char group, int id);
void func_088DA8E8(D012Sprite *sprite, void *seq);
void func_088DA924(D012Sprite *sprite, int value);
/* gim_register_inst: make a texture entry from a GIM in memory and give it to the sprite. */
int func_088DAE0C(D012Sprite *sprite, const void *gim, int size, int unk_06, int id);
void func_088CEEC8(D012TexManager *mgr, D012Sprite *sprite);       /* add to the draw list */

/* sprite_new_mem: like sprite_new_gim, with a GIM that is already in memory. */
D012Sprite *func_088D08C0(D012TexManager *mgr, unsigned char group, int id, const void *gim, int size)
{
    D012Sprite *sprite = SpriteStock::take(mgr);

    func_088DA8E8(sprite, func_088D0608(mgr, group, id));
    func_088DA924(sprite, 0);
    sprite->unk_04 = 0;
    sprite->unk_00 = 0;
    sprite->unk_08 = 0;
    if (gim != 0) {
        func_088DAE0C(sprite, gim, size, -1, -1);
    }
    func_088CEEC8(mgr, sprite);
    sprite->unk_17 = 0;
    sprite->flags |= 1;
    return sprite;
}

}
