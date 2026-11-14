#include "battle/agent18_types.h"

typedef struct LinkInfo {
    char pad_00[0x4];
    void *target;
} LinkInfo;

class LinkTask : public BattleObject {
public:
    int field_04;
    LinkInfo *link;
    int field_0C;
    float field_10;
    unsigned char field_14;
    unsigned char field_15;
};

extern "C" {

extern int func_08A87AA4(LinkInfo *link, LinkTask *task);

int func_08A88CE8(LinkTask *self)
{
    if (!self->slot_030()) {
        return -1;
    }
    if (self->link->target == 0) {
        return -2;
    }
    if (func_08A87AA4(self->link, self) != 0) {
        return -3;
    }
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_14 = 0;
    self->field_15 = 0;
    return 0;
}

}
