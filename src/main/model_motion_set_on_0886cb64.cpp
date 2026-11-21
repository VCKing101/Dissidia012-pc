extern "C" {
#include "main/agent33_texture.h"

/* model_motion_set_on: give motion `id` mode `on` and flag 0xFF when on (0 when off); a motion
 * not in the table takes the first free slot (id 0xFF). */
void func_0886CB64(D012ModelHolder *holder, int id, unsigned char on)
{
    int count = holder->motion_count;

    for (int i = 0; i < count; i++) {
        if (holder->motion_slots[i * 3] == id) {
            holder->motion_slots[i * 3 + 1] = on;
            holder->motion_slots[i * 3 + 2] = on != 0 ? 0xFF : 0;
            return;
        }
    }
    for (int i = 0; i < count; i++) {
        if (holder->motion_slots[i * 3] == 0xFF) {
            holder->motion_slots[i * 3] = id;
            holder->motion_slots[i * 3 + 1] = on;
            holder->motion_slots[i * 3 + 2] = on != 0 ? 0xFF : 0;
            return;
        }
    }
}
}
