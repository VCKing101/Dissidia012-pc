extern "C" {
struct FxListenerBase {
    char pad_00[0x24];
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void start();
};
struct FxListener : FxListenerBase {
    char pad_28[8];
    void *model;                /* 0x30 */
};
extern void func_088D9B28(void *p, unsigned char on);
void func_0892CE60(FxListener *l)
{
    if (l->model == 0) {
        return;
    }
    func_088D9B28(l->model, 0);
    l->start();
}
}
