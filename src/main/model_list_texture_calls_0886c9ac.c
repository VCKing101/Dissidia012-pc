#include "main/agent33_texture.h"

/* model_list_texture_calls: walk one of the holder's precompiled GE lists and switch its
 * CALLs (command 0x0A) on or off. A CALL becomes a NOP (command 0x00, address kept) unless
 * the word two before it is 0x00030000; a NOP right after a BASE (0x10) becomes a CALL again.
 * GMO textures are emitted into such lists as BASE + CALL to their own precompiled list. */
void func_0886C9AC(D012ModelHolder *holder, int which)
{
    unsigned int *cmd;
    unsigned int count;
    int i;

    cmd = holder->draw_lists[which];
    i = 0;
    count = holder->draw_list_sizes[which] >> 2;

    for (; i < count; i++, cmd++) {
        unsigned int word = *cmd;

        if ((word & 0xFF000000) == 0x0A000000) {
            if (i >= 2 && cmd[-2] != 0x30000) {
                *cmd = word & 0xFFFFFF;
            }
        } else if (i > 0 && (word & 0xFF000000) == 0 && (cmd[-1] & 0xFF000000) == 0x10000000) {
            *cmd = word | 0x0A000000;
        }
    }
}
