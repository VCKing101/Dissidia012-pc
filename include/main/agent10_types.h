#ifndef AGENT10_TYPES_H
#define AGENT10_TYPES_H

/* Animation player at 0x0887B748..0x0887B9E0: clip pointer, frame range,
 * play flags at 0x10, two float pairs at 0x18. */
typedef struct Agent10Clip {
    unsigned char unknown_00[0xC];
    short frame_count;
} Agent10Clip;

typedef union Agent10PlayFlags {
    unsigned int word;
    struct {
        unsigned int bit00 : 1;
        unsigned int started : 1;
        unsigned int bit02 : 1;
        unsigned int bit03 : 1;
        unsigned int bit04 : 1;
        unsigned int bit05 : 1;
        unsigned int bit06 : 1;
        unsigned int bit07 : 1;
        unsigned int bit08 : 1;
        unsigned int unknown_09 : 4;
        unsigned int hold : 1;
    } bits;
} Agent10PlayFlags;

typedef struct Agent10Player {
    Agent10Clip *clip;
    short frame;
    unsigned char field_06;
    unsigned char field_07;
    short field_08;
    short field_0A;
    unsigned char unknown_0C[4];
    Agent10PlayFlags flags;
    short start;
    short end;
    float field_18;
    float field_1C;
    float field_20;
    float field_24;
} Agent10Player;

#ifdef __cplusplus

/* Object reached through D_089D5EA0->manager; vptr at 0x154 after its data.
 * Same object as Agent7Manager, with the signatures the agent10 callers use. */
struct Agent10Manager {
    char data[0x154];
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void set03(int entry, int a, int b, unsigned char on);
    virtual void call04(int a, int b, int c);
    virtual void set05(int entry, int a, int b, unsigned char on);
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void call15(int a);
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
    virtual void set26(int entry, int a, int b, unsigned char on);
    virtual void call27(int a, int b, int c);
};

struct Agent10Root {
    int field_00;
    Agent10Manager *manager;
};

#endif /* __cplusplus */

#endif
