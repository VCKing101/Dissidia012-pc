// Resolves the target's object (virtual slot 37): without one, or one that is not type 0 with a link at +0x1108, falls back to func_08B3B668 and returns 0; otherwise returns func_08B3B81C(self, arg, link, handle(link)).
extern "C" {
extern int func_08835378(void *);
extern void func_08B3B668(void *, int);
}
struct Typed57 {
    char pad0[0x542];
    unsigned char type;
    char pad1[0x1108 - 0x543];
    Typed57 *link;
    static bool isZero(Typed57 *t) { return t->type == 0; }
};
extern "C" int func_08B3B81C(void *, int, void *, int);
struct Obj57 {
    char data[0x544];
    virtual Typed57 *v0();
    virtual Typed57 *v1();
    virtual Typed57 *v2();
    virtual Typed57 *v3();
    virtual Typed57 *v4();
    virtual Typed57 *v5();
    virtual Typed57 *v6();
    virtual Typed57 *v7();
    virtual Typed57 *v8();
    virtual Typed57 *v9();
    virtual Typed57 *v10();
    virtual Typed57 *v11();
    virtual Typed57 *v12();
    virtual Typed57 *v13();
    virtual Typed57 *v14();
    virtual Typed57 *v15();
    virtual Typed57 *v16();
    virtual Typed57 *v17();
    virtual Typed57 *v18();
    virtual Typed57 *v19();
    virtual Typed57 *v20();
    virtual Typed57 *v21();
    virtual Typed57 *v22();
    virtual Typed57 *v23();
    virtual Typed57 *v24();
    virtual Typed57 *v25();
    virtual Typed57 *v26();
    virtual Typed57 *v27();
    virtual Typed57 *v28();
    virtual Typed57 *v29();
    virtual Typed57 *v30();
    virtual Typed57 *v31();
    virtual Typed57 *v32();
    virtual Typed57 *v33();
    virtual Typed57 *v34();
    virtual Typed57 *v35();
    virtual Typed57 *v36();
    virtual Typed57 *v37();
};
struct Self57 {
    char pad0[0xF8];
    Obj57 *obj;
};
extern "C" int func_08B3B748(Self57 *self, int arg)
{
    Typed57 *t = self->obj->v37();
    if (t == 0) {
        func_08B3B668(self, arg);
        return 0;
    }
    if (!Typed57::isZero(t)) {
        func_08B3B668(self, arg);
        return 0;
    }
    t = t->link;
    if (t == 0) {
        func_08B3B668(self, arg);
        return 0;
    }
    return func_08B3B81C(self, arg, t, func_08835378(t));
}
