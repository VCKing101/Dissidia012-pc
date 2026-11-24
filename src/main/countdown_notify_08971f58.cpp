/* Counts a slot's counter down by one (if positive) and calls virtual 9 of the object when it reads zero. */
struct Counter {
    char data0[0x10];
    int *counts;
    char data1[0x10];
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void target(int idx);
};
extern "C" void func_08971F58(Counter *self, int idx)
{
    int *p = &self->counts[idx];
    int v = *p;
    if (v > 0) {
        *p = v - 1;
        v = self->counts[idx];
    }
    if (v == 0) {
        self->target(idx);
    }
}
