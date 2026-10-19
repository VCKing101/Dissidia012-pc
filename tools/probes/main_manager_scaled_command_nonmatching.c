/* Uncredited probe: register allocation, prologue scheduling, and multiply
 * operand order differ from retail despite an equal function size. */
#include "main/resource_manager_commands.h"

int func_088A14D0(void *manager, int id, float value, int index, unsigned int flags) {
    float scale = func_088A1C38((unsigned char *)manager + 0x7C + index * 16);
    return func_08991660(id, D_089D08F8, (int)(scale * value), flags << 9) != -1;
}
