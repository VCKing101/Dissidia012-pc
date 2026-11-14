/* Forwards three arguments to the handler's virtual start(). */
struct Handler {
    virtual void start(int a, int b, int c);
    virtual void stop(int a);
};

struct HandlerRef {
    Handler *handler;
};

extern "C" {
void func_089051A8(HandlerRef *ref, int a, int b, int c)
{
    ref->handler->start(a, b, c);
}
}
