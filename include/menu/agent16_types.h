#ifndef MENU_AGENT16_TYPES_H
#define MENU_AGENT16_TYPES_H

/* Hint mark list kept inside the menu page state at +0x38A0 (0x089FDxxx-0x089FExxx).
   The same block as MenuHintMarks in agent3_types.h, with more fields named. */
typedef struct A16HintMark {
    signed char field_00;
    signed char field_01;
    short field_02;
    short field_04;
    short field_06;
} A16HintMark;

typedef struct A16HintMarks {
    char pad_00000[0x14];
    int field_14;
    char pad_00018[0x4];
    int field_1C;
    char pad_00020[0x8];
    void *field_28;
    char pad_0002C[0x10C - 0x2C];
    short field_10C;
    short field_10E;
    unsigned char field_110;
    unsigned char field_111;
    short field_112;
    char pad_00114[0x1544 - 0x114];
    A16HintMark field_1544[1];
    char pad_0154C[0x26D64 - 0x154C];
    int field_26D64;
    char pad_26D68[0x303A0 - 0x26D68];
    char field_303A0[0x2C];
    char field_303CC[0x1C];
    short field_303E8;
    short field_303EA;
} A16HintMarks;

/* The page state; the totals at +0x33C44 sit inside the hint block's range. */
typedef struct A16HintState {
    union {
        struct {
            char pad_00000[0x38A0];
            A16HintMarks field_38A0;
        } hint;
        struct {
            char pad_00000[0x33C44];
            int field_33C44;
            char pad_33C48[0x8];
            int field_33C50;
        } totals;
    } u;
} A16HintState;

typedef struct A16HintOwner {
    char pad_000[0x14C];
    A16HintState *field_14C;
} A16HintOwner;

#ifdef __cplusplus
/* Menu part whose vtable pointer sits after 0x24 bytes of data (0x08A5xxxx),
   like A12Part24 with the data fields it uses named. Virtual N is at vtable offset 8 * (N + 1). */
class A16Part24 {
public:
    char pad_00[0x18];
    int field_18;
    char pad_1C[0x24 - 0x1C];
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
};
#endif

#endif
