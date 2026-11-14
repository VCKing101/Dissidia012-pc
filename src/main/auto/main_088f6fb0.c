#include "main/auto_region4.h"

/* Hands D_089D8508 a temporary Obj9CCDB8, then calls func_0891B96C on it. */
void func_088F6FB0(void)
{
    void *manager;
    Obj9CCDB8 object;

    manager = D_089D8508;
    object.vtable = &D_089CCDB8;
    func_0891B8C0(D_089D8508, &object);
    func_0891B96C(manager, 0, 0);
}
