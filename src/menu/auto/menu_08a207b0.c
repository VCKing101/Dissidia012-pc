#include "menu/auto_region1.h"

typedef struct Found207B0 {
    char pad_00[0xC];
    short *field_0C;
} Found207B0;

int func_089012BC(Found207B0 **list, int count, int arg1, int arg2, char arg3);
int func_0890119C(char **list, int count, int arg1, char arg2);

/* Menu root: hands the first value of list 7 and the first byte of name
 * index to func_08900C04 on D_089D8140. */
void func_08A207B0(MenuRoot *self, int index)
{
    Found207B0 *found[32];
    char *names[32];

    func_089012BC(found, 0x20, 7, -1, 0);
    func_0890119C(names, 0x20, 7, 0);
    func_08900C04(D_089D8140, 7, *found[0]->field_0C, *names[index]);
}
