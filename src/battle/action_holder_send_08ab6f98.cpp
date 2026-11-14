#include "battle/agent15_types.h"

extern "C" {

extern void func_088A9DE8(void *target, int message, void *data, int a, int b);

int func_08AB6F98(ActionHolder *self, int message)
{
    void *target;
    if (self->active == 0) {
        return -1;
    }
    target = ActionLinkPart::targetOf(&self->link_390->part);
    if (target == 0) {
        return -2;
    }
    func_088A9DE8(target, message, self->target_4C8, 0, 0);
    return 0;
}

}
