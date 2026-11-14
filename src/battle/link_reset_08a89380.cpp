#include "battle/agent18_types.h"

class LinkTimerTask : public BattleObject {
public:
    int field_04;
    void *link;
    float field_0C;
    int field_10;
};

extern "C" {

extern int func_08A87AA4(void *link, LinkTimerTask *task);
extern float func_08A896BC(LinkTimerTask *task);

int func_08A89380(LinkTimerTask *self)
{
    if (!self->slot_030()) {
        return -1;
    }
    if (func_08A87AA4(self->link, self) != 0) {
        return -2;
    }
    self->field_0C = func_08A896BC(self);
    self->field_10 = 0;
    return 0;
}

}
