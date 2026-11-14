/* Shared structures for agent26's main matches. */
#ifndef AGENT26_TYPES_H
#define AGENT26_TYPES_H

#ifdef __cplusplus

/* A pending request kept at offset 0x20 of a stream device; done() is called with the result. */
typedef void (*A26DoneFn)(int result, int arg);
struct A26Device;
struct A26Request {
    int state;      /* 0x00 */
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    A26DoneFn done; /* 0x14 */
    int doneArg;    /* 0x18 */
    int value;      /* 0x1C */
};

/* Stream device: 0x154 bytes of data, then the vptr. Virtual N sits at vtable offset 8 * (N + 1). */
struct A26Device {
    char data_000[0x20];
    A26Request request;                 /* 0x20 */
    char data_040[0x154 - 0x40];
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04();
    virtual void write(char *name, int a, int b, int flags);   /* 0x30 */
    virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16();
    virtual void open(int value, void (*done)(int result, A26Device *dev), A26Device *arg);   /* 0x90 */
    virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28();
    virtual void finish(int value);     /* 0xF0 */
    virtual void v30(); virtual void v31();
    virtual void makeName(char *out, int id);   /* 0x108 */
};

#endif
#endif
