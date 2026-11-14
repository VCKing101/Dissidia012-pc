struct Agent4Actor34894;

/* Controller object whose SNC vtable pointer sits at +0x08. */
class Agent4Controller {
public:
    int field_00;
    int field_04;
    virtual void slot_008();
    virtual void slot_010(Agent4Actor34894 *actor, unsigned short arg);
    virtual void slot_018();
    virtual void slot_020(Agent4Actor34894 *actor);
    virtual void slot_028(Agent4Actor34894 *actor, int arg);
    virtual void slot_030(Agent4Actor34894 *actor);
    virtual void slot_038(Agent4Actor34894 *actor);
};

struct Agent4Pose {
    unsigned char unknown_0000[0xF0];
    short field_F0;
    short field_F2;
};

struct Agent4Actor34894 {
    unsigned char unknown_0000[0x314];
    Agent4Pose *pose;
    unsigned char unknown_0318[0x8C];
    Agent4Controller *controller;
};

extern "C" {
void func_08834894(Agent4Actor34894 *self) {
    self->controller->slot_020(self);
}

void func_088348C8(Agent4Actor34894 *self, int arg) {
    self->controller->slot_028(self, arg);
}

void func_08834900(Agent4Actor34894 *self) {
    self->controller->slot_030(self);
}

void func_08834934(Agent4Actor34894 *self, int a, int b) {
    self->pose->field_F0 = a;
    self->pose->field_F2 = b;
}

void func_08834948(Agent4Actor34894 *self) {
    self->controller->slot_038(self);
}
}
