/* Stops a unit: clears its state and player, runs virtual 43 and releases
   its model effect's filter. */
struct Effect08842284 {
    char pad_00[0x28];
    void *handle;
};

struct Model08842284 {
    char pad_00[0x34];
    Effect08842284 effect;

    static Effect08842284 *effectOf(Model08842284 *m) { return &m->effect; }
};

struct Unit08842284 {
    char pad_000[0x130];
    char player[0x260];
    Model08842284 *model;
    char pad_394[0x134];
    char filter[4];
    char pad_4CC[0x78];
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
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
    virtual void stop();
};

extern "C" {

void func_08834C10(Unit08842284 *u, int a, int b);
void func_0882FA10(void *player, int one, int zero, float a, float b);
void func_088AA1A0(void *handle, void *filter, int id);

void func_08842284(Unit08842284 *u)
{
    if (u != 0) {
        func_08834C10(u, 0, 0);
        func_0882FA10(u->player, 1, 0, 0.0f, 0.0f);
        u->stop();
        void *handle = Model08842284::effectOf(u->model)->handle;
        if (handle != 0) {
            func_088AA1A0(handle, u->filter, -1);
        }
    }
}

}
