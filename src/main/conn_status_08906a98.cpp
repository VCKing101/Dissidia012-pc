struct ConnHandler {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual int status();
};

struct Connection {
    int state;
    char pad_004[0x2D0 - 4];
    ConnHandler *handler;
};

extern "C" {
int func_08906A98(Connection *c)
{
    int result = 0;

    if (c->handler != 0) {
        result = c->handler->status();
    }
    return result;
}
}
