#include "battle/agent14_types.h"

class AttachTask : public HandleTask {
public:
    int field_0C;
    int field_10;
    unsigned char field_14;
};

extern "C" {

extern int func_08A87AA4(int state, AttachTask *task);

int func_08A8A6A0(AttachTask *self)
{
    if (self->isReady() == 0) {
        return -1;
    }
    if (func_08A87AA4(self->handle, self) != 0) {
        return -2;
    }
    self->field_0C = 0;
    self->field_10 = -1;
    self->field_14 = 0;
    return 0;
}

}
