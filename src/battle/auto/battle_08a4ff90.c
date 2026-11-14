#include "battle/auto_region3.h"

typedef struct State4FF90 State4FF90;

void func_08A500D0(unsigned char flag);
void func_0889991C(int value);
int func_08AA7984(void *object, char flag);
void func_08A4FFD8(State4FF90 *self);

/* Calls func_08A500D0(0), func_0889991C(-1) and func_08AA7984, then
 * func_08A4FFD8. */
void func_08A4FF90(State4FF90 *self)
{
    func_08A500D0(0);
    func_0889991C(-1);
    func_08AA7984(D_08AAAD80, 0);
    func_08A4FFD8(self);
}
