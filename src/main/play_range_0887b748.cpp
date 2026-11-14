#include "main/agent10_types.h"

struct Play0887B748 {
    static bool held(unsigned int flags) {
        return (flags & 0x2000) != 0;
    }
};

extern "C" {
void func_0887B748(Agent10Player *self, int start, int end) {
    self->start = start;
    self->end = end;
    if (!Play0887B748::held(self->flags.word)) {
        self->end = self->end - 1;
    }
}
}
