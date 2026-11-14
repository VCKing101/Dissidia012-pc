/* Forwards one argument to the handler's virtual stop(). */
struct Handler {
    virtual void start(int a, int b, int c);
    virtual void stop(int a);
};

struct HandlerRef {
    Handler *handler;
};

extern "C" {
void func_089052F0(HandlerRef *ref, int a)
{
    ref->handler->stop(a);
}
}
