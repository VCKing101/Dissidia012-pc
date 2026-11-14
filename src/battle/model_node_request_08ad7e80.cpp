#include "battle/agent15_types.h"

typedef struct ModelRef {
    char pad_000[0x108];
    ActionHolder *fighter;
} ModelRef;

typedef struct ModelNode {
    char pad_000[0x64];
    ModelRef *model;
} ModelNode;

extern "C" {

extern int func_088A9E24(void *target, void (*callback)(void), void *arg, void *data, int a, int b);
extern void func_08AD81E0(void);

/* Ask the fighter's target to call back func_08AD81E0 for this node. */
int func_08AD7E80(ModelNode *self)
{
    ActionHolder *fighter = self->model->fighter;
    void *target = ActionLinkPart::targetOf(&fighter->link_390->part);
    int result = 0;
    if (target != 0) {
        result = func_088A9E24(target, func_08AD81E0, self, fighter->target_4C8, 0, 0);
    }
    return result;
}

}
