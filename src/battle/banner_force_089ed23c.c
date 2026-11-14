#include "battle/agent28_types.h"

/* Forces the banner timer to start now. */
extern FrameClock *D_089D69E0;
extern BattleClock *D_089D69F8;
extern float D_089ED884;
extern float D_089ED888;
extern int func_0885ECF4(void *timer);

int func_089ED23C(BannerTimer *banner)
{
    if (!banner->open) {
        return -1;
    }
    {
        int now = func_0885ECF4(D_089D69F8->timer);
        banner->forced = 1;
        banner->started_at = now;
    }
    return 0;
}
