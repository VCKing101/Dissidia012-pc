// Creates a marker effect for the actor: stores it at +0x30, moves it to the actor position and scales it by the actor's size.
extern "C" {
void func_08899300();
void *func_088A9DE8(int a, int b, int c, int d, int e);
void func_088A7284(float x, float y, float z, void *h);
void func_088A72E8(void *h, float x, float y, float z);
float func_08837014(void *actor);
extern struct Pool *D_089D6DD0;
}

struct Pool {
    char d0[0xC];
    int f0C;
};

struct Actor {
    char d0[0x100];
    float x, y, z;
    char d10C[0x544 - 0x10C];
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
};

struct Marker {
    char d0[0x30];
    void *handle;
};

struct Fn {
    static int has(void *h) { return h != 0; }
};

extern "C" void func_08AC4670(Marker *m, Actor *a) {
    func_08899300();
    a->v37();
    void *p = m->handle = func_088A9DE8(D_089D6DD0->f0C, 3, 0, 0, 0);
    if (p != 0) {
        func_088A7284(a->x, a->y, a->z, p);
        void *h = m->handle;
        float s = func_08837014(a);
        func_088A72E8(h, s, s, s);
    }
}
