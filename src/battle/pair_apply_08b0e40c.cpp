// Applies the same setup to two embedded entries (+0x6C and +0x7C), the second with the object returned by a virtual lookup.
extern "C" {
void *func_08889E44();
}

struct Q {
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
    virtual Q *v37();
};

struct Info {
    char d0[8];
    short kind;
};

struct Owner {
    char d0[0x6C];
    char e6C[0x10];
    char e7C[0x10];
    Info *info;
};

extern "C" void func_08B0D7F4(void *e, void *q, int a, int b, unsigned char flag, int c);

extern "C" void func_08B0E40C(Owner *o, int a, int b, int c) {
    if (o->info != 0) {
        Q *q = (Q *)func_08889E44();
        func_08B0D7F4(o->e6C, q, a, b, o->info->kind == 0, c);
        Q *r = q->v37();
        func_08B0D7F4(o->e7C, r, a, b, o->info->kind == 0, c);
    }
}
