#include "main/agent33_texture.h"

extern "C" {

bool func_088DE0E8(D012TexEntry *entry, const void *gim, int size, int unk_06, int id, unsigned char group); /* tex_entry_load */

/* tex_register: make a texture entry of (group, id) from a GIM already in memory and add it
 * to the manager's list; a GIM that does not load gives its entry back. Returns id + 1. */
int func_088D01E4(D012TexManager *mgr, unsigned char group, const void *gim, int size, int id)
{
    D012TexEntry *entry;

    entry = D012TexEntries::alloc(mgr);
    if (func_088DE0E8(entry, gim, size, -1, id, group)) {
        D012TexEntries::push_back(mgr, entry);
    } else {
        D012TexEntries::release(mgr, entry);
    }
    return id + 1;
}

}
