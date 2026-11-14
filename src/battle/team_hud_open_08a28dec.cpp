#include "battle/agent31_types.h"

/* Open request for the team HUD: two paired parts and the leader part. */
struct TeamRequest {
    unsigned int side;
    void *name;
    int pad_08;
    A31ResourceSet *set;
    void *icon;
    int key;
};

class HudPartBase {
public:
    virtual void slot_008();
    virtual void slot_010();
    virtual void close();
    virtual void slot_020();
    virtual void slot_028();
    virtual void slot_030();
    virtual void slot_038();
    virtual int is_active();
};

struct HudPairSlot {
    A31HudPair part;
    unsigned char pad[0x11C - sizeof(A31HudPair)];
};

class TeamHud : public HudPartBase {
public:
    unsigned char open;
    unsigned char pad_05[0xB];
    HudPairSlot pairs[2];
    unsigned char leader[0xC8];
    unsigned int side;
    void *name;
    void *icon;
};

extern "C" int func_08A26304(A31HudPair *self, unsigned int side, unsigned int slot, A31ResourceSet *set, int key);
extern "C" int func_08A256C4(void *self, unsigned int side, A31ResourceSet *set, int key);

extern "C" void func_08A29088(TeamHud *self, float dt, float time);
extern "C" void func_08A26484(A31HudPair *self, float dt);
extern "C" void func_08A257E8(void *self, float dt);

extern "C" int func_08A28DEC(TeamHud *self, TeamRequest *request)
{
    self->close();
    if (request == 0 || request->side >= 2 || request->name == 0 || request->set == 0 ||
        request->icon == 0 || request->key < 0) {
        return -1;
    }
    unsigned int side = request->side;
    void *name = request->name;
    A31ResourceSet *set = request->set;
    void *icon = request->icon;
    int key = request->key;
    for (int i = 0; i < 2; i++) {
        func_08A26304(&self->pairs[i].part, side, i, set, key);
    }
    func_08A256C4(self->leader, side, set, key);
    self->side = side;
    self->name = name;
    self->icon = icon;
    self->open = 1;
    return 0;
}

extern "C" int func_08A28F10(TeamHud *self, float dt, float time)
{
    if (!self->is_active()) {
        return -1;
    }
    func_08A29088(self, dt, time);
    for (int i = 0; i < 2; i++) {
        func_08A26484(&self->pairs[i].part, dt);
    }
    func_08A257E8(self->leader, dt);
    return 0;
}
