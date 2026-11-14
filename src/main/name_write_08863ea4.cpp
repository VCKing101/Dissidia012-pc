#include "main/agent26_types.h"

extern "C" {
void func_08863EA4(A26Device *dev, int id, int a, int b)
{
    char name[256];
    dev->makeName(name, id);
    dev->write(name, a, b, 0);
}
}
