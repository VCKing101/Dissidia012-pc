#include "battle/agent18_types.h"

typedef struct Vec4 {
    float x;
    float y;
    float z;
    float w;
} Vec4;

class LinkMoveTask : public BattleObject {
public:
    int field_04;
    void *link;
    unsigned char mode;
    char pad_0D[0x3];
    int field_10;
    float field_14;
    int field_18;
    char pad_1C[0x4];
    Vec4 field_20;
};

extern "C" {

extern float D_08A90038;
extern int func_08A87AA4(void *link, LinkMoveTask *task);
extern void func_089A3890(Vec4 *v);

int func_08A8F930(LinkMoveTask *self, unsigned char mode)
{
    if (!self->slot_030()) {
        return -1;
    }
    if (func_08A87AA4(self->link, self) != 0) {
        return -3;
    }
    self->mode = mode;
    self->field_10 = 0;
    self->field_14 = D_08A90038;
    self->field_18 = 0;
    func_089A3890(&self->field_20);
    return 0;
}

}
