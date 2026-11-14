#include "battle/auto_region3.h"

/* Part80208 release: calls func_08AA56C8 on D_08AA5828, then resets. */
void func_08A80154(Part80208 *self)
{
    func_08AA56C8(D_08AA5828);
    func_08A8014C(self);
}
