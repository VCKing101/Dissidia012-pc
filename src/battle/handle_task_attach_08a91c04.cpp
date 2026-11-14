#include "battle/agent14_types.h"

class AttachTask : public HandleTask {
public:
    unsigned char started;
    int frames;
};

extern "C" {

extern int func_08A87AA4(int state, AttachTask *task);

int func_08A91C04(AttachTask *self)
{
    if (self->isReady() == 0) {
        return -1;
    }
    if (func_08A87AA4(self->handle, self) != 0) {
        return -2;
    }
    self->started = 1;
    self->frames = 30;
    return 0;
}

}
