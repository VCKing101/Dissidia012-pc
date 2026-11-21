#include "main/agent33_texture.h"

extern "C" {

/* pkg_index_find: the PACKAGE_INFO entry of a (localised) path, or 0. */
D012PkgIndexEntry *func_088713E4(D012Package *pkg, const char *path);

/* pkg_read: read a package file from PACKAGE.BIN into dst: seek the device to the entry's
 * sector (low 20 bits, 0x800-byte sectors), then read its size (low 28 bits). Returns what
 * the device read returns, or 0 when the path is not in the index. */
int func_088714F4(D012Package *pkg, const char *path, void *dst)
{
    D012PkgIndexEntry *entry = func_088713E4(pkg, path);

    if (entry == 0) {
        return 0;
    }
    unsigned int mask = 0x0FFFFFFF;
    pkg->device->seek(pkg->file, (entry->sector & 0xFFFFF) * 0x800, 0);
    return pkg->device->read(pkg->file, dst, entry->size & mask, 0);
}

}
