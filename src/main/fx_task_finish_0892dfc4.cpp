extern "C" {
struct FxTaskBase {
    char pad_00[0x24];
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual int done();
};
struct FxTask : FxTaskBase {
    int state;                  /* 0x28 */
};
void func_0892DFC4(FxTask *t)
{
    if (t->done() == 0) {
        return;
    }
    t->state = 3;
}
}
