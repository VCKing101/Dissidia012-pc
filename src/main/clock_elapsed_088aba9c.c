// Returns the total at obj+0x1FDF8 plus the time elapsed since the sample at obj+0xEA8.
#include "main/agent44_types.h"
extern long long func_089C6010(void);
extern long long func_0899924C(long long, long long);
extern long long D_089CB3D0;
int func_088ABA9C(Clock44 *obj)
{
    int now = (int)func_0899924C(func_089C6010(), D_089CB3D0);
    return now - obj->fEA8 + obj->f1FDF8;
}
