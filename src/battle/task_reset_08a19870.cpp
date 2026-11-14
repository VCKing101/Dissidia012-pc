#include "battle/agent11_types.h"

class ResetTask : public BattleTaskBase {
public:
    int field_04;
    int state;
    unsigned char started;
    unsigned char shown;
    char pad_0E[0x2];
    float elapsed;
    char pad_14[0x318];
    int channel;
};

struct ResetMessage {
    int words[3];
};

extern "C" {

extern void func_089F5974(int channel, ResetMessage *message, int flags);

/* Clears the task back to state 0 and tells its channel to stop (only from state -1). */
int func_08A19870(ResetTask *self)
{
    if (!self->isActive()) {
        return -1;
    }
    if (self->state != -1) {
        return -2;
    }
    {
        ResetMessage message;
        for (int i = 0; i < 2; i++) {
            message.words[i] = 1;
        }
        message.words[2] = 2;
        func_089F5974(self->channel, &message, 0);
    }
    self->state = 0;
    self->started = 0;
    self->shown = 0;
    self->elapsed = 0.0f;
    return 0;
}

}
