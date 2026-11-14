/* Shared structures for agent17's main-module matches. */
#ifndef AGENT17_TYPES_H
#define AGENT17_TYPES_H

/* Effect parameter block: id at 4, size at 0xC, payload from 0x10. */
typedef struct FxBlob {
    int field_00;
    int magic;
    int field_08;
    int size;
    char data[1];
} FxBlob;

/* Parameter slots filled by func_088AA434/088AA440/088AA47C. */
typedef struct FxSlots {
    char pad_00[0xC];
    FxBlob *first;
    FxBlob *second;
    char *second_data;
    int second_size;
    char pad_1C[0x24 - 0x1C];
    FxBlob *third;
    char *third_data;
    int third_size;
} FxSlots;

/* Battle round controller (0x088AF134 family): flags at 4, context at 0xC. */
typedef struct RoundCtx {
    char pad_00[0x814];
    int mode;
    char pad_818[0xD65 - 0x818];
    unsigned char field_D65;
    char pad_D66[0xDAC - 0xD66];
    int stage;
    char pad_DB0[0xDD4 - 0xDB0];
    int field_DD4;
    char pad_DD8[0x2B58 - 0xDD8];
    unsigned long long saved;
    char pad_2B60[0x2B80 - 0x2B60];
    unsigned char field_2B80;
    char pad_2B81[0x2B84 - 0x2B81];
    int saved_x;
    int saved_y;
} RoundCtx;

typedef struct Round {
    int field_00;
    union {
        unsigned int word;
        struct {
            unsigned int b0 : 1;
            unsigned int b1 : 1;
            unsigned int b2 : 1;
            unsigned int b3 : 1;
            unsigned int b4 : 1;
            unsigned int b5 : 1;
            unsigned int b6 : 1;
            unsigned int b7 : 1;
        } bits;
    } flags;
    int field_08;
    RoundCtx *ctx;
} Round;

/* Three 30-byte rows (12 single cells, 6 triples), then 64 cells at 0x5A. */
typedef struct GridRow {
    unsigned char cells[12];
    unsigned char triples[6][3];
} GridRow;

typedef struct GridPair {
    unsigned char first;
    unsigned char second;
} GridPair;

typedef struct Grid {
    GridRow rows[3];
    unsigned char extra[64];
    unsigned short marks[4];
    short values[10];
    signed char current;
    char pad_B7[3];
    unsigned char mode;
    GridPair pairs[5];
} Grid;

/* Six-byte slot of the board's id list (0x3DE): id, a value and flags. */
typedef struct BoardSlot {
    short id;
    short value;
    unsigned short flags;
} BoardSlot;

/* Five grids, then 150 slots, then the current index. */
typedef struct Board {
    Grid grids[5];
    BoardSlot slots[150];
    signed char current;
} Board;

#ifdef __cplusplus
/* Comparison order passed by value to the binary searches; 0 = ascending. */
struct SearchOrder {
    signed char descending;

    SearchOrder() : descending(0) {}
};
#endif

/* Sorted id records kept in one block (0x088B8FD0 family). */
typedef struct CatalogWord {
    short id;
    short value;
} __attribute__((aligned(4))) CatalogWord;

typedef struct CatalogByte4 {
    signed char id;
    char value[3];
} __attribute__((aligned(4))) CatalogByte4;

typedef struct CatalogByte2 {
    signed char id;
    char value;
} CatalogByte2;

typedef struct Catalog {
    CatalogWord first[683];
    CatalogWord second[630];
    CatalogByte4 third[64];
    CatalogByte2 fourth[56];
    int first_count;
    int second_count;
    int third_count;
    int fourth_count;
} Catalog;

/* Five optional pointers and two flag bytes (0x088BC6DC family). */
typedef struct LinkSet {
    void *link_00;
    void *link_04;
    void *link_08;
    void *link_0C;
    void *link_10;
    unsigned char flag_14;
    unsigned char flag_15;
} LinkSet;

#ifdef __cplusplus
/* Network session (0x088C99E4 family): state at 0xBDB8, flags at 0xBDE8 (bit 1 = online). */
struct Session {
    char pad_00[0xBDB8];
    int state;
    char pad_BDBC[0xBDE8 - 0xBDBC];
    union {
        unsigned int word;
        struct {
            unsigned int b0 : 1;
            unsigned int online : 1;
        } bits;
    } flags;

    static bool online(Session *s)
    {
        return s->flags.bits.online != 0;
    }
};
#endif

#endif
