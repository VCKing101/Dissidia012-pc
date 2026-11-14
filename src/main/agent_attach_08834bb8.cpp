struct Agent4Actor34BB8;

/* Controller object whose SNC vtable pointer sits at +0x08. */
class Agent4Controller {
public:
    int field_00;
    int field_04;
    virtual void slot_008();
    virtual void slot_010(Agent4Actor34BB8 *actor, unsigned short arg);
};

struct Agent4Actor34BB8 {
    unsigned char unknown_0000[0x3A4];
    Agent4Controller *controller;
};

extern "C" {
Agent4Controller *func_08A5D2B8(int id);

void func_08834BB8(Agent4Actor34BB8 *self, int id, unsigned short arg) {
    self->controller = func_08A5D2B8(id);
    self->controller->slot_010(self, arg);
}
}
