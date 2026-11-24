// Rescales the actor's three basis vectors by the scale at +0x110C (when not 1.0), then refreshes its parts.
extern "C" {
void func_089A3A44(float s, void *in, void *out);
void func_08831994(void *a);
void func_08AC8318(void *p);
void func_0885436C(void *p, void *a);
void func_08851974(void *a, void *v);
void func_08859844(void *a);
void func_0884D658(void *p, void *a);
void func_0884F5D8(void *p);
}

struct ActorBase {
    char d0[0x544];
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
};

struct Actor : ActorBase {
    char d548[0x5A4 - 0x548];
    void *f5A4;
    char d5A8[0x76C - 0x5A8];
    char p76C[0xBA0 - 0x76C];
    char pBA0[0xD08 - 0xBA0];
    char pD08[0xD64 - 0xD08];
    char pD64[0x110C - 0xD64];
    float scale;
};

extern "C" void func_08B2D524(Actor *a) {
    float s = a->scale;
    if (s != 1.0f) {
        float s2 = s;
        float s3 = s;
        func_089A3A44(s, a, a);
        func_089A3A44(s2, (char *)a + 0x10, (char *)a + 0x10);
        func_089A3A44(s3, (char *)a + 0x20, (char *)a + 0x20);
        a->v29();
        func_08831994(a);
    }
    func_08AC8318(a->pBA0);
    func_0885436C(a->pD08, a);
    func_08851974(a, a->f5A4);
    func_08859844(a);
    func_0884D658(a->p76C, a);
    func_0884F5D8(a->pD64);
}
