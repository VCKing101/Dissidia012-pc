#include "battle/agent14_types.h"

class TrackTask : public HandleTask {
public:
    char pad_0C[0x28];
    char tracker[0x4];
};

extern "C" {

extern void func_08A82220(void *tracker);

void func_08A907EC(TrackTask *self)
{
    func_08A82220(self->tracker);
    self->release();
}

}
