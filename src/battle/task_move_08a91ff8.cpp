// Starts the move task toward the given vector: -1 when unavailable, -2 for a zero vector, -3 when already queued.
extern "C" {
float func_089A3BF8(void *v);
int func_08A87AA4(void *list, void *self);
void func_08A97098(void *p, int id);
void func_089A3868(void *dst, void *src);
}

struct Slot {
    char d0[8];
    void *obj;
};

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
    Slot *list;
    char dC[4];
    char vec[0x10];
    int f20;
    int f24;
};

extern "C" int func_08A91FF8(Task *t, void *v) {
    if (t->v5() == 0) {
        return -1;
    }
    if (func_089A3BF8(v) == 0.0f) {
        return -2;
    }
    if (func_08A87AA4(t->list, t) != 0) {
        return -3;
    }
    func_08A97098(t->list->obj, 0xD);
    func_089A3868(t->vec, v);
    t->f20 = 0;
    t->f24 = 30;
    return 0;
}
