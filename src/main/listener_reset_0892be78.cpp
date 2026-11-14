extern "C" {
struct Listener {
    char pad_00[0x1C];
    float time;
    char pad_20[4];
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void start();
    virtual void stop();
    virtual void setMode(int mode);
};
void func_0892BE78(Listener *l)
{
    l->time = 0.0f;
    l->setMode(0);
    l->start();
}
}
