#include "battle/agent27_types.h"

extern void func_08B1966C(LoadJob *job);

int func_08B197E0(LoadJob *job, unsigned int kind, int a, int b, void *c)
{
    func_08B1966C(job);
    if (kind >= 9 || a == 0 || b == 0 || c == 0) {
        return -1;
    }
    job->kind = (int)kind;
    job->field_34 = a;
    job->field_38 = b;
    job->field_3C = c;
    job->active = 1;
    return 0;
}
