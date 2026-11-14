#include "battle/agent14_types.h"

typedef struct TimedCommand {
    char pad_00[0x4];
    void *unit;
} TimedCommand;

class CountdownTask : public HandleTaskBase {
public:
    unsigned char bound;
    TimedCommand *command;
    char pad_0C[0x24];
    float remaining;
};

extern "C" {

extern float func_08838300(void *unit);

int func_08A8F9C4(CountdownTask *self)
{
    if (self->isReady() == 0) {
        return -1;
    }
    self->remaining = self->remaining - func_08838300(self->command->unit);
    if (self->remaining < 0.0f) {
        self->remaining = 0.0f;
    }
    return 0;
}

}
