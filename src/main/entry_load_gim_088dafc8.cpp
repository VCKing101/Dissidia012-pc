#include "main/agent33_texture.h"

extern "C" {

extern D012TexManager *D_089D7830;

int func_088FE7C8(const char *path, unsigned char alt);                       /* file_cache_size */
int func_08862C40(const char *path, int flag);                                /* pkg_size */
int func_088FE708(const char *path, void *dst, int size, unsigned char alt);  /* file_cache_read */
void func_088FE944(const char *path, void *data, int size);                  /* post-process the file */
void func_08862854(const char *path, void *dst, int size, unsigned char flag); /* pkg_load */
void *func_0885C6CC(void *heap, int size, int align, int tag, int one, int from_end); /* heap_alloc */
void func_0885C538(void *heap, void *block, int align);                       /* heap_free */
void func_088DB10C(D012Sprite *sprite);                                       /* drop the sprite's texture */
int func_088DAE0C(D012Sprite *sprite, const void *gim, int size, int unk_06, int id); /* gim_register_inst */

/* entry_load_gim: load one whole package entry as one GIM and make it the sprite's texture.
 * The file goes through a staging block from the texture manager's heap, freed after the
 * image library has copied the pixels. Returns true when the sprite has a texture. */
bool func_088DAFC8(D012Sprite *sprite, const char *path, unsigned char use_cache)
{
    int size;

    if (use_cache) {
        size = func_088FE7C8(path, 0);
    } else {
        size = func_08862C40(path, 1);
    }
    if (size == 0) {
        func_088DB10C(sprite);
        return false;
    }
    void *heap = D_089D7830->heap_b == 0 ? D_089D7830->heap_a : D_089D7830->heap_b;
    int one = 1;
    void *data = func_0885C6CC(heap, size, 0x40, 0, one, 1);
    if (use_cache) {
        func_088FE708(path, data, size, 0);
        func_088FE944(path, data, size);
    } else {
        func_08862854(path, data, size, 1);
    }
    func_088DAE0C(sprite, data, size, -1, -1);
    func_0885C538(heap, data, 0x40);
    return sprite->tex != 0;
}

}
