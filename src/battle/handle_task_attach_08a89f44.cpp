#include "battle/agent14_types.h"

class AttachTask : public HandleTask {
public:
    int field_0C;
};

extern "C" {

extern int func_08A87AA4(int state, AttachTask *task);

int func_08A89F44(AttachTask *self)
{
    if (self->isReady() == 0) {
        return -1;
    }
    if (func_08A87AA4(self->handle, self) != 0) {
        return -2;
    }
    self->field_0C = 0;
    return 0;
}

}
