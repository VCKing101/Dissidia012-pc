extern "C" {
struct Listener {
    char pad_00[0x24];
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void start();
    virtual void stop();
};
typedef struct ListenerRef { int field_00; Listener *listener; } ListenerRef;
void func_0892AE80(ListenerRef *ref)
{
    if (ref == 0) {
        return;
    }
    Listener *l = ref->listener;
    if (l == 0) {
        return;
    }
    l->start();
    l->stop();
}
}
