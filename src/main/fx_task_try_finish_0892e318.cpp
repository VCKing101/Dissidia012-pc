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
extern int func_0892E5DC(FxTask *t);
extern void func_0892E5B4(FxTask *t);
bool func_0892E318(FxTask *t)
{
    if (func_0892E5DC(t) == 1 && t->done() != 0) {
        func_0892E5B4(t);
        return true;
    } else {
        return false;
    }
}
}
