struct ConnHandler {
    virtual int open(int a, int b);
    virtual void close();
    virtual int send(int a, int b, int c);
    virtual int recv(int a, int b, int c);
    virtual int poll();
    virtual void notify(int arg);
};

struct Connection {
    int state;
    char pad_004[0x2D0 - 4];
    ConnHandler *handler;
    int arg;
};

extern "C" {
void func_08906B60(Connection *c)
{
    if (c->handler != 0) {
        c->handler->notify(c->arg);
    }
}
}
