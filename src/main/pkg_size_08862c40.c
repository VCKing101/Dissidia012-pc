#include "main/agent33_texture.h"

extern D012FileSystem *D_089D5EA0;

/* path_localize: copy the path to 0x08B63600 with "ZQ"/"JP" replaced by the language code. */
extern const char *func_08867984(const char *path, int flag);
extern int func_088631AC(const char *path);                      /* size from the loose-file layer, 0 = none */
extern int func_0885CC24(const char *path);                      /* size, synchronous file system */
extern int func_08871644(void *package, const char *path);       /* size from the PACKAGE.BIN index */

/* pkg_size: size in bytes of a package file. */
int func_08862C40(const char *path)
{
    const char *local = func_08867984(path, 0);
    int size = func_088631AC(local);

    if (size != 0) {
        return size;
    }
    return D_089D5EA0->sync == 1 ? func_0885CC24(local) : func_08871644(D_089D5EA0->package, local);
}
