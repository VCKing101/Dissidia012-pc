extern "C" {
#include "main/agent33_texture.h"

extern void *func_089B0E78(void *model, int motion);       /* the model's motion */
extern void func_089B0FAC(void *motion, int loop);

/* model_motions_apply: for every used motion slot (id, mode, flag; 3 bytes each, id 0xFF =
 * unused) set whether the motion loops: mode 1 with flag 0xFF loops, anything else plays once. */
void func_0886A180(D012ModelHolder *holder)
{
    for (int i = 0; i < holder->motion_count; i++) {
        unsigned char id = holder->motion_slots[i * 3];

        if (id != 0xFF) {
            void *motion = func_089B0E78(holder->current_part_addr->model_addr, id);
            unsigned char *slot = holder->motion_slots + i * 3;

            if (slot[1] == 1 && slot[2] == 0xFF) {
                func_089B0FAC(motion, 1);
            } else {
                func_089B0FAC(motion, 0);
            }
        }
    }
}
}
