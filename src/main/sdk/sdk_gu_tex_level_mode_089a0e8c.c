/* libGu: sceGuTexLevelMode - TEXLEVEL (0xC8): mip-level mode and a bias in 1/16 levels, -128..127. */
#include "main/agent34_sdk.h"

extern const float D_089D18BC; /* 16.0f */

void func_089A0E8C(float bias, u32 mode)
{
    int offset = (int)(bias * D_089D18BC);

    if (offset > 127)
        offset = 127;
    else if (offset < -128)
        offset = -128;
    GU_SEND(0xC8, ((offset & 0xFF) << 16) | mode);
}
