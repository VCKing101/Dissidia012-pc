/* Clears the flag in the scene block, then calls virtual 30 on every actor the iterator returns, and finishes. */
struct Actor {
    char data[0x544];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8(); virtual void v9(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void target();
};
extern char *D_089D6AC8;
extern "C" Actor *func_0881CBB8(Actor *prev, int mask);
extern "C" void func_0881CB5C(void);
extern "C" void func_08915CBC(void)
{
    Actor *a;
    int zero = 0;
    *(char *)(D_089D6AC8 + 0x6B2) = zero;
    a = (Actor *)zero;
    while ((a = func_0881CBB8(a, -1)) != 0) {
        a->target();
    }
    func_0881CB5C();
}
