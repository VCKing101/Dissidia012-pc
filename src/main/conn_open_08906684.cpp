/* Opens the connection through its handler; state 1 on success. */
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
    int a;
    int b;
};

extern "C" {
bool func_08906684(Connection *c, int a, int b)
{
    ConnHandler *h = c->handler;

    if (h != 0) {
        c->a = a;
        c->b = b;
        if (h->open(a, b) == 1) {
            c->state = 1;
            return true;
        }
    }
    return false;
}
}
