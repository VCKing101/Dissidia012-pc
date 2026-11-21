/* GMO texture library: bit 4 of the texture's flags (+0x02), 0 for no texture. */
#include "main/agent34_sdk.h"

typedef struct D012GmoTextureHead {
    s16 refcount; /* 0x00 */
    u16 flags;    /* 0x02 */
} D012GmoTextureHead;

int func_089BCD58(D012GmoTextureHead *tex)
{
    if (tex == 0)
        return 0;
    return __builtin_allegrex_ext(tex->flags, 4, 1);
}
