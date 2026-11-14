#include "menu/auto_region3.h"

/* List76158 constructor: the node and ten rows. */
List76158 *func_08A76158(List76158 *self)
{
    func_088D2E70(&self->node);
    func_08993560(self->rows, 10, sizeof(Row75B78), func_08A75B78);
    return self;
}
