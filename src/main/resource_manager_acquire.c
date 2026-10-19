#include "main/resource_manager_commands.h"

int func_088A1380(void *manager, int id, unsigned int a, unsigned int b, int flags) {
    int result = func_0899016C(a, b, id, (unsigned char)flags);
    if (result) func_08991660(id, D_089D08F8, 0, 0);
    return result;
}

int func_088A13E4(void *manager, int id) {
    return func_0899029C(id);
}

int func_088A1400(void *manager, int id) {
    return func_08991660(id, D_089D08E0, 0, 0) != -1;
}

int func_088A1438(void *manager, int id) {
    return func_08991660(id, D_089D08E4, 0, 0) != -1;
}

int func_088A1470(void *manager, int id, unsigned char value) {
    int result;
    if (value == 1) result = func_08991660(id, D_089D08E8, 0, 0);
    else result = func_08991660(id, D_089D08EC, 0, 0);
    return result != -1;
}
