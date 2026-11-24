// Releases the effects owned by `self`: clears flag 1 at +0xD64, runs func_08837CAC, then for each object func_08842088 returns whose owner (+0x550) is `self`, calls its virtual slot 43.
struct Obj57 {
    char data[0x544];
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
};
extern "C" {
extern void func_0884F960(void *, int);
extern void func_08837CAC(void *);
extern Obj57 *func_08842088(Obj57 *);
}
struct Self57 {
    char pad0[0xD64];
};
struct Cmp57 { static bool same(void *a, void *b) { return a == b; } };
struct Obj2_57 {
    char pad0[0x550];
    void *owner;
};
extern "C" void func_08B35118(Self57 *self)
{
    Obj57 *o;
    func_0884F960((char *)self + 0xD64, 1);
    func_08837CAC(self);
    o = 0;
    while ((o = func_08842088(o)) != 0) {
        if (((Obj2_57 *)o)->owner != (void *)self) {
            continue;
        }
        o->v43();
    }
}
