/* Polls the handler: 1 means closed (state 2), otherwise re-opens it (state 1). */
struct ConnHandler {
    virtual int open(int a, int b);
    virtual void close();
    virtual int send(int a, int b, int c);
    virtual int recv(int a, int b, int c);
    virtual int poll();
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
void func_08906AD8(Connection *c)
{
    ConnHandler *h = c->handler;

    if (h != 0) {
        if (h->poll() == 1) {
            c->state = 2;
        } else {
            c->handler->open(c->a, c->b);
            c->state = 1;
        }
    }
}
}
