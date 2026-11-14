struct ConnHandler {
    virtual int open(int a, int b);
    virtual void close();
    virtual int send(int a, int b, int c);
    virtual int recv(int a, int b, int c);
};

struct Connection {
    int state;
    char pad_004[0x2D0 - 4];
    ConnHandler *handler;
};

extern "C" {
int func_08906848(Connection *c, int a, int b, int d)
{
    if (c->handler != 0) {
        return c->handler->recv(a, b, d);
    }
    return 0;
}
}
