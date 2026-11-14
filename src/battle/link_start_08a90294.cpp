#include "battle/agent18_types.h"

class LinkValueTask : public BattleObject {
public:
    int field_04;
    void *link;
    int state;
    void *value;
};

extern "C" {

extern int func_08A87AA4(void *link, LinkValueTask *task);

int func_08A90294(LinkValueTask *self, void *value)
{
    if (!self->slot_030()) {
        return -1;
    }
    if (value == 0) {
        return -2;
    }
    if (func_08A87AA4(self->link, self) != 0) {
        return -3;
    }
    self->state = 3;
    self->value = value;
    return 0;
}

}
