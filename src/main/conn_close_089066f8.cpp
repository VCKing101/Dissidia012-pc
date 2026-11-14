struct ConnHandler {
    virtual int open(int a, int b);
    virtual void close();
};

struct Connection {
    int state;
    char pad_004[0x2D0 - 4];
    ConnHandler *handler;
};

extern "C" {
void func_089066F8(Connection *c)
{
    if (c->handler != 0) {
        c->handler->close();
        c->state = 0;
    }
}
}
