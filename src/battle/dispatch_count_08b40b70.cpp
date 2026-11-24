// Dispatch on the count at +0x1C: 1 calls func_08B40994, other positive or negative values call func_08B40A50, and 0 runs the target's virtual slot 37.
extern "C" {
extern void func_08B40994(void *, void *);
extern void func_08B40A50(void *, void *);
}
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
};
struct Target57 {
    char pad0[0xF8];
    Obj57 *obj;
};
struct Self57 {
    char pad0[0x1C];
    int count;
};
extern "C" void func_08B40B70(Self57 *self, Target57 *t)
{
    switch (self->count) {
    case 1:
        func_08B40994(self, t);
        return;
    case 0:
        t->obj->v37();
        return;
    default:
        func_08B40A50(self, t);
        return;
    }
}
