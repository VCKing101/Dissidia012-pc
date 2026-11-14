#ifndef AGENT7_TYPES_H
#define AGENT7_TYPES_H

/* Object reached through D_089D5EA0->manager; vptr at 0x154 after its data. */
struct Agent7Manager {
    char data[0x154];
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void load04(const char *name, void *buf, int size);
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void request08(int handle, int a, int b, void (*done)(void *), void *ctx, int flags);
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void setEntry14(int entry, unsigned char on);
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual int open22(const char *name);
    virtual void setEntry23(int entry, unsigned char on);
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void call29(int a);
    virtual void call30(int a, int b);
    virtual void call31(int a, int b);
};

struct Agent7Root {
    int field_00;
    Agent7Manager *manager;
};

#endif
