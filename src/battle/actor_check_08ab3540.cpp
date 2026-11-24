// True when the actor passes its virtual check and the matching 0x112/0x113 flag test succeeds; 0 for a null actor.
extern "C" {
int func_08835778(void *a);
int func_088374B0(void *a);
int func_088BF4CC(int v, int id);
}

struct Actor {
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
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual int v42();
};

struct Fn {
    static unsigned char has(int v) { return v != 0; }
};

extern "C" int func_08AB3540(Actor *a) {
    if (a == 0) {
        return 0;
    }
    if (a->v42() == 0) {
        return 0;
    }
    Actor *b = a;
    int v = func_08835778(b);
    if (func_088374B0(b) == 0) {
        if (!Fn::has(func_088BF4CC(v, 0x112))) {
            return 0;
        }
    } else {
        if (!Fn::has(func_088BF4CC(v, 0x113))) {
            return 0;
        }
    }
    return 1;
}
