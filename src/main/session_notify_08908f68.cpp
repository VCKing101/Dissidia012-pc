struct SessionHandler {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void poll();
    virtual void notify(int arg);
};

struct Session {
    char pad_00[0x20];
    SessionHandler *handler;
};

extern "C" {
void func_08908F68(Session *s, int arg)
{
    if (s->handler != 0) {
        s->handler->notify(arg);
    }
}
}
