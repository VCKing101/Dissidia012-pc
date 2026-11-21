#include "main/agent33_texture.h"

extern D012FileSystem *D_089D5EA0;

extern void func_088630C8(void);
extern const char *func_08867984(const char *path, int flag);   /* path_localize */
extern int func_08863174(const char *path, void *dst, int size, int a3, int t0, unsigned char t1); /* loose-file layer */
extern int func_088715BC(void *package, const char *path, void *dst, int size, int a3, int t0); /* PACKAGE.BIN read */

/* pkg_read_sync: the synchronous read behind pkg_load when the file system's `sync` is set
 * (reached through FUN_0885CBD8). Loose files first, then PACKAGE.BIN. */
int func_088629DC(const char *path, void *dst, int size, int a3, int t0, unsigned char t1)
{
    const char *local;

    func_088630C8();
    local = func_08867984(path, 0);
    if (func_08863174(local, dst, size, a3, t0, t1) != 0) {
        return 1;
    }
    return func_088715BC(D_089D5EA0->package, local, dst, size, a3, t0);
}
