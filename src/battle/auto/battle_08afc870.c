#include "battle/auto_region5.h"

typedef struct NodeFC870 {
    char pad_00[0xDC];
    unsigned int flags;
} NodeFC870;

/* PartFC870: looks up node `id` of the model and flags it (0x100) as
 * field_0C; field_0C stays 0 when there is no model or node. */
void func_08AFC870(PartFC870 *self, int id)
{
    NodeFC870 *node;

    self->field_0C = 0;
    if (self->model != 0 && (node = func_088A9DE8(self->model, id, 0, 0, 0)) != 0) {
        node->flags = node->flags | 0x100;
        self->field_0C = node;
    }
}
