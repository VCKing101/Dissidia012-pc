#include "battle/oct13_activation_views.h"

int func_089E0BE8(void *object, void *buffer, int count)
{
    func_089E0C6C(object);
    if (!buffer || count <= 0) return -1;
    if (!func_0887AC70((unsigned char *)object + 8, buffer, count, func_088908E0())) return -2;
    return 0;
}
