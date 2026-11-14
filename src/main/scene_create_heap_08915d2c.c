#include "main/agent21_types.h"

extern void *func_088D0700(int heap, int size, int a, int b);
extern int D_089D7830;

void func_08915D2C(Scene *scene)
{
    int size = 0x24;
    scene->heap = func_088D0700(D_089D7830, size, 0x8A02, 0x8801);
}
