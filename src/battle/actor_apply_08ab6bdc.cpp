// Applies the V4 to the active actor (-1 when inactive), calls its virtual update, returns -2 when the follow-up fails.
struct V4 { float x, y, z, w; };

extern "C" {
void func_088359A8(void *a, V4 *v, int flag);
int func_08AB6F98(void *a, int arg);
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
    virtual void v42();
    virtual void v43();
    virtual void v44();
};

struct Actor : ActorBase {
    char d548[8];
    unsigned char active;
};

extern "C" int func_08AB6BDC(Actor *a, V4 *q, int arg) {
    if (a->active == 0) {
        return -1;
    }
    V4 v = *q;
    func_088359A8(a, &v, 1);
    a->v44();
    if (func_08AB6F98(a, arg) != 0) {
        return -2;
    }
    return 0;
}
