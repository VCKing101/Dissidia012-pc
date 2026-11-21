#include "main/agent33_texture.h"

struct FepBinder {
    /* Remember `index` as the bound texture; true when it already was. */
    static bool same(D012FepManager *mgr, int index) {
        if (mgr->current_texture != index) {
            mgr->current_texture = index;
            return false;
        }
        return true;
    }
};

extern "C" {

/* fep_tex_bind: set the GE texture state for one FEP texture (filter and wrap modes too). */
void func_08972C14(D012FepTexRef *ref, D012FepCache *cache, int min_filter, int mag_filter,
                   int wrap_u, int wrap_v);

/* fep_tex_bind_idx: bind the manager's index-th texture unless it is bound already.
 * With the cache on, the texture is first copied into the effect VRAM cache. */
void func_08973400(D012FepManager *mgr, int index, int min_filter, int mag_filter, int wrap_u, int wrap_v)
{
    if (FepBinder::same(mgr, index)) {
        return;
    }
    D012FepCache *cache = 0;
    if (mgr->use_tex_cache != 0) {
        cache = &mgr->tex_cache;
    }
    func_08972C14(mgr->textures[index], cache, min_filter, mag_filter, wrap_u, wrap_v);
    mgr->current_texture = index;
}

}
