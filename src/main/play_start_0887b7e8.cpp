#include "main/agent10_types.h"

struct Play0887B7E8 {
    static bool held(unsigned int flags) {
        return (flags & 0x2000) != 0;
    }
};

extern "C" {
void func_0887B7E8(Agent10Player *self) {
    unsigned int flags = self->flags.word;
    if (!Play0887B7E8::held(flags)) {
        self->frame = self->clip->frame_count;
    }
    self->field_06 = 0;
    self->field_07 = 0;
    self->flags.word = flags | 2;
}
}
