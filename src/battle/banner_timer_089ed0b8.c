#include "battle/agent28_types.h"

/* Banner timer: waits for its start time (or a forced start), then runs
 * two timed phases. */
extern FrameClock *D_089D69E0;
extern BattleClock *D_089D69F8;
extern float D_089ED884;
extern float D_089ED888;
extern int func_0885ECF4(void *timer);

int func_089ED0B8(BannerTimer *banner)
{
    if (!banner->open) {
        return -1;
    }
    switch (banner->state) {
    case 0:
        if (banner->start - func_0885ECF4(D_089D69F8->timer) <= 0 || banner->forced == 1) {
            banner->state = 1;
            banner->time = 0.0f;
        }
        break;
    case 1:
        banner->time += D_089D69E0->frame_scale;
        if (banner->time >= D_089ED884) {
            banner->state = 2;
            banner->time = 0.0f;
        }
        break;
    case 2:
        banner->time += D_089D69E0->frame_scale;
        if (banner->time >= D_089ED888) {
            banner->state = 3;
            banner->time = 0.0f;
        }
        break;
    }
    return 0;
}
