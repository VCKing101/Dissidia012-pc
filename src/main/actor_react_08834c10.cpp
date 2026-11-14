/* Queues a reaction for an actor unless it is locked: tells its
   controller first. */
struct Actor08834C10;

struct Controller08834C10 {
    int pad_0;
    int pad_4;
    virtual void v00();
    virtual void v01();
    virtual void notify(Actor08834C10 *actor);
};

struct Actor08834C10 {
    char pad_000[0x380];
    unsigned int flags;
    char pad_384[0x3A4 - 0x384];
    Controller08834C10 *controller;

    static bool locked(Actor08834C10 *a) { return (a->flags & 0x400000) != 0; }
};

extern "C" {

void func_08834BB8(Actor08834C10 *self, int id, unsigned short param);

void func_08834C10(Actor08834C10 *self, int id, unsigned short param)
{
    if (Actor08834C10::locked(self)) {
        return;
    }
    self->controller->notify(self);
    func_08834BB8(self, id, param);
}

}
