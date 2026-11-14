#ifndef MENU_AGENT20_TYPES_H
#define MENU_AGENT20_TYPES_H

/* Entry table of the 0x089Fxxxx menu page, reached through owner->field_14C.
   Entries are 0x84 bytes from +0x29CC; the count is the signed byte at +0x2731. */
typedef struct A20Entry {
    unsigned char field_00;
    unsigned char field_01;
    unsigned char field_02;
    char pad_03;
    unsigned int field_04;
    char pad_08[0x7];
    signed char field_0F;
    short field_10;
    char pad_12[0x84 - 0x12];
} A20Entry;

typedef struct A20EntryTable {
    char pad_000[0x2724];
    unsigned int field_2724;
    short field_2728;
    char pad_272A[0x4];
    unsigned short field_272E;
    char pad_2730[0x1];
    signed char field_2731;
    char pad_2732[0x4];
    unsigned short field_2736;
    unsigned short field_2738;
    char pad_273A[0xA];
    short field_2744;
    char pad_2746[0x56];
    unsigned char field_279C;
    char pad_279D[0x5];
    short field_27A2;
    char pad_27A4[0x228];
    A20Entry field_29CC[9];
    char pad_2E70[0x3B4];
    int field_3224;
    int field_3228;
    short field_322C;
    short field_322E;
    char pad_3230[0x50];
    unsigned int field_3280;
    char pad_3284[0x74];
    unsigned int field_32F8;
} A20EntryTable;

typedef struct A20EntryOwner {
    char pad_000[0x14C];
    A20EntryTable *field_14C;
} A20EntryOwner;

#endif
