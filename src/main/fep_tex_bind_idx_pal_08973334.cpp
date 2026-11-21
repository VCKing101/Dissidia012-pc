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

/* fep_tex_bind_pal: fep_tex_bind, then the TEX block's own palette (fep_clut_bind). */
void func_08972BC8(D012FepTexRef *ref, D012FepCache *tex_cache, D012FepCache *clut_cache,
                   int min_filter, int mag_filter, int wrap_u, int wrap_v);

/* fep_tex_bind_idx_pal: like fep_tex_bind_idx, for a paletted texture; each cache
 * (texture, palette) is used only when it is switched on. */
void func_08973334(D012FepManager *mgr, int index, int min_filter, int mag_filter, int wrap_u, int wrap_v)
{
    if (FepBinder::same(mgr, index)) {
        return;
    }
    D012FepCache *tex_cache = 0;
    D012FepCache *clut_cache = 0;
    if (mgr->use_tex_cache != 0) {
        tex_cache = &mgr->tex_cache;
    }
    if (mgr->use_clut_cache != 0) {
        clut_cache = &mgr->clut_cache;
    }
    func_08972BC8(mgr->textures[index], tex_cache, clut_cache, min_filter, mag_filter, wrap_u, wrap_v);
    mgr->current_texture = index;
}

}
