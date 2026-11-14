#include "battle/agent14_types.h"

class TrackTask : public HandleTask {
public:
    char pad_0C[0x28];
    char tracker[0x4];
};

extern "C" {

extern int func_08A8223C(void *tracker);

int func_08A9082C(TrackTask *self, int handle)
{
    self->reset();
    if (handle == 0) {
        return -1;
    }
    if (func_08A8223C(self->tracker) != 0) {
        return -2;
    }
    self->handle = handle;
    self->bound = 1;
    return 0;
}

}
