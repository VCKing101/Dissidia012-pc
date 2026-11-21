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
int func_088DAFC8(D012Sprite *sprite, const char *path, unsigned char use_file_cache); /* entry_load_gim */
void func_088CEEC8(D012TexManager *mgr, D012Sprite *sprite);       /* add to the draw list */

/* sprite_new_gim: a new sprite instance of the layout object (group, id); when `path` is
 * given, that whole package entry is loaded as one GIM and becomes the sprite's texture. */
D012Sprite *func_088D0810(D012TexManager *mgr, unsigned char group, int id, const char *path)
{
    D012Sprite *sprite = SpriteStock::take(mgr);

    func_088DA8E8(sprite, func_088D0608(mgr, group, id));
    func_088DA924(sprite, 0);
    sprite->unk_04 = 0;
    sprite->unk_00 = 0;
    sprite->unk_08 = 0;
    if (path != 0) {
        func_088DAFC8(sprite, path, mgr->use_file_cache);
    }
    func_088CEEC8(mgr, sprite);
    sprite->unk_17 = 0;
    sprite->flags |= 1;
    return sprite;
}

}
