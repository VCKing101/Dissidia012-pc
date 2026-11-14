#ifndef MENU_AGENT3_TYPES_H
#define MENU_AGENT3_TYPES_H

/* Hint marks kept inside the menu page state at +0x38A0. */
typedef struct MenuHintMark {
    signed char field_00;
    signed char field_01;
    short field_02;
    short field_04;
    short field_06;
} MenuHintMark;

typedef struct MenuHintMarks {
    char pad_00000[0x1544];
    MenuHintMark field_1544[1];
    char pad_0154C[0x26D64 - 0x154C];
    int field_26D64;
    char pad_26D68[0x303CC - 0x26D68];
    char field_303CC[4];
} MenuHintMarks;

typedef struct MenuHintState {
    char pad_0000[0x38A0];
    MenuHintMarks field_38A0;
} MenuHintState;

typedef struct MenuHintOwner {
    char pad_000[0x14C];
    MenuHintState *field_14C;
} MenuHintOwner;

/* List state reached through owner +0x2A14 (0x08A3D7xx-0x08A3DExx). */
typedef struct MenuListItem {
    int field_00;
    int field_04;
    int field_08;
    char pad_0C[0x100];
} MenuListItem;

typedef struct MenuListKey {
    int field_00;
    int field_04;
} MenuListKey;

typedef struct MenuListState {
    char pad_0000[0x4B8];
    MenuListItem field_4B8[32];
    char pad_2638[0x2838 - 0x2638];
    MenuListKey field_2838[0x37];
    int field_29F0;
    int field_29F4;
    char pad_29F8[0x8];
    int field_2A00;
} MenuListState;

typedef struct MenuListOwner {
    char pad_0000[0x2A14];
    MenuListState *field_2A14;
} MenuListOwner;

/* Text panel reached through owner +0x418 -> +0x4 (0x08A46xxx). */
typedef struct MenuTextPanel {
    char pad_00[0x6C];
    int field_6C[0x18];
    int field_CC[4];
    int field_DC;
} MenuTextPanel;

typedef struct MenuTextLink {
    int field_00;
    MenuTextPanel *field_04;
    char pad_008[0x3FC - 0x8];
    int field_3FC[4];
} MenuTextLink;

typedef struct MenuTextOwner {
    char pad_000[0x418];
    MenuTextLink *field_418;
} MenuTextOwner;

#endif
