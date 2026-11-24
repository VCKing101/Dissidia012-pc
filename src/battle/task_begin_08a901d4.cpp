// Starts task state 2 with two handles and a float; -1 when unavailable, -2 for a missing handle, -3 when already queued.
extern "C" int func_08A87AA4(void *list, void *self);

struct TaskBase {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual int v5();
};

struct Task : TaskBase {
    int f4;
    void *list;
    int state;
    int f10;
    int f14;
    int f18;
    float f1C;
};

extern "C" int func_08A901D4(Task *t, int a, int b, int c) {
    if (t->v5() == 0) {
        return -1;
    }
    if (a == 0 || b == 0) {
        return -2;
    }
    if (func_08A87AA4(t->list, t) != 0) {
        return -3;
    }
    t->state = 2;
    t->f10 = a;
    t->f18 = b;
    t->f1C = (float)c;
    return 0;
}
