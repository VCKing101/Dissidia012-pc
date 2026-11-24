// Caches two handles on the target object (+0xF8): its own at +0x300 and the one its virtual slot 37 returns at +0x304 (taken through either branch of the func_0883739C test).
extern "C" {
extern int func_08835378(void *);
extern int func_0883739C(void *);
}
struct Obj57 {
    char data[0x544];
    virtual void *v0();
    virtual void *v1();
    virtual void *v2();
    virtual void *v3();
    virtual void *v4();
    virtual void *v5();
    virtual void *v6();
    virtual void *v7();
    virtual void *v8();
    virtual void *v9();
    virtual void *v10();
    virtual void *v11();
    virtual void *v12();
    virtual void *v13();
    virtual void *v14();
    virtual void *v15();
    virtual void *v16();
    virtual void *v17();
    virtual void *v18();
    virtual void *v19();
    virtual void *v20();
    virtual void *v21();
    virtual void *v22();
    virtual void *v23();
    virtual void *v24();
    virtual void *v25();
    virtual void *v26();
    virtual void *v27();
    virtual void *v28();
    virtual void *v29();
    virtual void *v30();
    virtual void *v31();
    virtual void *v32();
    virtual void *v33();
    virtual void *v34();
    virtual void *v35();
    virtual void *v36();
    virtual void *v37();
};
struct Self57 {
    char pad0[0xF8];
    Obj57 *obj;
    char pad1[0x300 - 0xFC];
    int h300;
    int h304;
};
extern "C" void func_08B3B550(Self57 *self)
{
    self->h300 = func_08835378(self->obj);
    if (func_0883739C(self->obj->v37()) != 0) {
        self->h304 = func_08835378(self->obj->v37());
    } else {
        self->h304 = func_08835378(self->obj->v37());
    }
}
