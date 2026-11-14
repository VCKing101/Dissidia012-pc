#include "main/auto_region2.h"

/* Releases the world's field_00 and clears it. */
void func_088417C8(void)
{
    World9D4958 *world;

    world = D_089D4958;
    func_08882704(D_089D4958->field_00);
    world->field_00 = 0;
}
