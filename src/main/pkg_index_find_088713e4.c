#include "main/agent33_texture.h"

extern D012PkgIndex D_09B26BC0;        /* PACKAGE_INFO.BIN */

extern unsigned int func_08871288(const char *path);                 /* pkg_path_hash */
extern int func_08935B50(const char *a, const char *b, int n);       /* strncmp */
extern int func_089359E8(const char *s);                             /* strlen */
extern char *func_08935BB4(char *dst, const char *src, int n);       /* strncpy */

/* pkg_index_find: the PACKAGE_INFO entry of a (localised) path: a binary search on the
 * path's CRC, with the last path looked up cached in the package object. 0 = not there. */
D012PkgIndexEntry *func_088713E4(D012Package *pkg, const char *path)
{
    unsigned int crc;
    int lo;
    int hi;
    unsigned int key;
    D012PkgIndexEntry *entry;
    D012PkgIndexEntry *entries;

    if (pkg->last_entry != 0 && func_08935B50(path, pkg->last_path, 0x50) == 0) {
        return pkg->last_entry;
    }
    crc = func_08871288(path);
    lo = 0;
    hi = D_09B26BC0.count;
    entries = D_09B26BC0.entries;
    do {
        int mid = (hi + lo) / 2;

        entry = &entries[mid];
        key = entry->crc;
        if (key == crc) {
            break;
        }
        if (key < crc) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    } while (hi - lo >= 0);
    if (key != crc) {
        return 0;
    }
    func_089359E8(path);
    func_08935BB4(pkg->last_path, path, 0x50);
    pkg->last_entry = entry;
    return entry;
}
