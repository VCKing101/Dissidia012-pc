#ifndef MAIN_AGENT2_TYPES_H
#define MAIN_AGENT2_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/* Virtual table entry of the SNC C++ ABI: a this-adjustment followed by the
 * function address. The object's vtable pointer lives at +0x154. */
typedef struct Agent2VtblEntry {
    short delta;
    short index;
    void *func;
} Agent2VtblEntry;

typedef struct Agent2Pair {
    int field_0000;
    int field_0004;
} Agent2Pair;

/* Stream-like object reached through 0x08864xxx wrappers. */
typedef struct Agent2Stream {
    unsigned char unknown_0000[0x24];
    int field_0024;
    int field_0028;
    int field_002C;
    int field_0030;
    int field_0034;
    int field_0038;
    int field_003C;
    Agent2Pair field_0040;
    unsigned char unknown_0048[0x101];
    unsigned char field_0149;
    unsigned char unknown_014A[0xA];
    Agent2VtblEntry *field_0154;
} Agent2Stream;

/* Two floats and a packed colour, set through the 0x08864C1C accessors. */
typedef struct Agent2Point {
    float field_0000;
    float field_0004;
    unsigned int field_0008;
} Agent2Point;

/* Table object released by 0x08865710: two allocations and a count. */
typedef struct Agent2Table {
    unsigned char field_0000;
    unsigned char unknown_0001[0xF];
    void *field_0010;
    void **field_0014;
    short field_0018;
    unsigned char field_001A;
} Agent2Table;

typedef struct Agent2NamedValue {
    char name[0x20];
    int value;
} Agent2NamedValue;

/* Object held at D_089D5EC0. Constructed by 0x08865840. */
typedef struct Agent2GlobalState {
    int field_0000[0x1C4];
    void *field_0710;
    void *field_0714;
    unsigned char unknown_0718[0x8];
    unsigned char field_0720[0xC0];
    unsigned char field_07E0[0xD0];
    Agent2Point field_08B0;
    unsigned int field_08BC;
    int field_08C0;
    Agent2NamedValue field_08C4[0x80];
    int field_1AC4;
    Agent2NamedValue field_1AC8[0x80];
    int field_2CC8;
    unsigned char field_2CCC;
    unsigned char unknown_2CCD;
    unsigned char field_2CCE;
    unsigned char field_2CCF;
    signed char field_2CD0;
    unsigned char field_2CD1;
} Agent2GlobalState;

#ifdef __cplusplus
}
#endif
#endif
