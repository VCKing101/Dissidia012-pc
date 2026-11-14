/* Shared structures for agent19's main-module matches (0x088D7xxx-0x0890xxxx). */
#ifndef AGENT19_TYPES_H
#define AGENT19_TYPES_H

#ifdef __cplusplus

/* Battle actor: data first, vptr at 0x544 (virtual N at vtable offset 8 * (N + 1)). */
struct Agent19ActorBase {
    char pad_000[0x544];
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
    virtual void sceneUpdate();     /* vtable 0xF8 */
};

/* A value kept in the frame across calls (see build/dc-results/stack-spill.md). */
template <class T> struct Agent19Kept {
    T value;
    Agent19Kept(T v) : value(v) {}
};

#endif

#endif
