// Takes a new time sample and adds its difference from the previous sample (obj+0xEA8) to the total at obj+0x1FDF8.
#include "main/agent44_types.h"
extern long long func_089C6010(void);
extern long long func_0899924C(long long, long long);
extern long long D_089CB3D0;
void func_088AB9F8(Clock44 *obj)
{
    int now = (int)func_0899924C(func_089C6010(), D_089CB3D0);
    obj->f1FDF8 += now - obj->fEA8;
    obj->fEA8 = now;
}
