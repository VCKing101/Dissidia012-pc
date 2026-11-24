// Computes a value from the owner's actor level and two virtual stats and the target's level, scaled by the percentage at +0x20.
extern "C" int func_08AE3C80(int a, int b, int c, int d);

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
    virtual ActorBase *v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual int v49();
    virtual int v50();
};

struct Actor : ActorBase {
    char d548[0x10B4 - 0x548];
    int level;
};

struct Self {
    char d0[0x10];
    Actor *actor;
    char d14[0xC];
    int percent;
};

extern "C" int func_08AE0ADC(Self *s) {
    Actor *target = (Actor *)s->actor->v37();
    Actor *a = s->actor;
    int lvl = a->level;
    int x = a->v50();
    int y = s->actor->v49();
    int v = func_08AE3C80(lvl, x, y, target->level);
    return v * s->percent / 100;
}
