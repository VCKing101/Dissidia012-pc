#include "battle/agent14_types.h"

class FlagTask : public HandleTask {
public:
    char pad_0C[0x1C];
    unsigned char field_28;
};

extern "C" int func_08A929DC(FlagTask *self, int handle)
{
    self->reset();
    if (handle == 0) {
        return -1;
    }
    self->handle = handle;
    self->bound = 1;
    self->field_28 = 0;
    return 0;
}
