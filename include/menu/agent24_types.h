/* Shared menu structures for agent24's functions. */
#ifndef AGENT24_TYPES_H
#define AGENT24_TYPES_H

/* One entry of a screen list (8 bytes, from 0x148A of the list state). */
typedef struct A24ListEntry {
    short id;
    short field_2;
    short field_4;
    unsigned char flags;
    unsigned char mask;
} A24ListEntry;

typedef struct A24List {
    A24ListEntry entries[150];
} A24List;

/* The list state reached through screen + 0xDB30. */
typedef struct A24ListState {
    char pad_0000[0x1214];
    int field_1214;
    char pad_1218[0x1430 - 0x1218];
    int counts[22];
    char pad_1488[2];
    A24List lists[29];
    char pad_9DE2[0xA03A - 0x148A - 29 * 0x4B0];
    unsigned short order[22][150];
} A24ListState;

typedef struct A24ListScreen {
    char pad_0000[0xDB30];
    A24ListState *field_DB30;
} A24ListScreen;


/* A 39 x 64 grid of two-byte cells (0x1380 bytes). */
typedef struct A24Cell {
    unsigned char a;
    unsigned char b;
} A24Cell;

typedef struct A24Grid {
    A24Cell cells[39][64];
} A24Grid;

/* The same screen state seen as its grid pages. */
typedef struct A24GridState {
    char pad_0000[0x1210];
    int field_1210;
    char pad_1214[0x3EBA - 0x1214];
    A24Grid pages[5];
    char pad_A51A[0xAB44 - 0x3EBA - 5 * 0x1380];
    A24Grid field_AB44;
} A24GridState;

typedef struct A24GridScreen {
    char pad_0000[0xDB30];
    A24GridState *field_DB30;
} A24GridScreen;

/* Slot list state reached through owner + 0x67C (0x08B0xxxx functions). */
typedef struct A24SlotMsg {
    int field_0;
    int field_4[5];
    unsigned int field_18[5];
} A24SlotMsg;

typedef struct A24Slot {
    short id;
    unsigned short flags;
} A24Slot;

typedef struct A24SlotState {
    char pad_000[0x400];
    void *field_400[(0x654 - 0x400) / 4];
    int field_654;
    int field_658;
    char pad_65C[4];
    A24Slot field_660[5];
    A24SlotMsg *field_674;
} A24SlotState;

typedef struct A24SlotOwner {
    char pad_000[0x67C];
    A24SlotState *field_67C;
} A24SlotOwner;

/* Cell lists of the 0x08B0xxxx screen state (owner + 0x138). */
typedef struct A24Cell14 {
    unsigned short field_0;
    unsigned short field_2;
    int field_4;
    char pad_8[0x14 - 8];
} A24Cell14;

typedef struct A24CellList {
    A24Cell14 cells[800];
} A24CellList;

typedef struct A24CellState {
    char pad_00000[0x31BC];
    int field_31BC[(0x3264 - 0x31BC) / 4];
    A24CellList field_3264[6];
    char pad_1A9E4[0x1A964 - 0x3264 - 6 * 16000];
    unsigned short field_1A964[12][0x320];
    char pad_1F8E4[0x1F8EC - 0x1A964 - 12 * 0x640];
    int field_1F8EC;
    char pad_1F8F0[0x1F918 - 0x1F8F0];
    int field_1F918;
    char pad_1F91C[0x24820 - 0x1F91C];
    unsigned char field_24820;
} A24CellState;

typedef struct A24CellOwner {
    char pad_000[0x138];
    A24CellState *field_138;
} A24CellOwner;

/* Save-slot screen state (owner + 0x844, 0x08B2Dxxx functions). */
typedef struct A24SaveSlot {
    int field_0;
    int field_4;
} A24SaveSlot;

typedef struct A24SaveState {
    char pad_000[0x1C];
    char field_01C[0x28 - 0x1C];
    void *field_028;
    void *field_02C;
    char pad_030[0x49C - 0x30];
    char field_49C[0x64C - 0x49C];
    A24SaveSlot field_64C[5];
    char field_674[0x678 - 0x674];
    int field_678;
    char pad_67C[0x6A0 - 0x67C];
    int field_6A0;
} A24SaveState;

typedef struct A24SaveOwner {
    char pad_000[0x844];
    A24SaveState *field_844;
} A24SaveOwner;

#endif
