#ifndef DISSIDIA_MENU_AUTO_REGION1_H
#define DISSIDIA_MENU_AUTO_REGION1_H

/* Classes behind the automatically matched functions in menu
 * 0x089E0AC0..0x08A3BF9C (readability pass, menu region 1).
 *
 * Most of this range is menu screens. A screen object either keeps a task
 * (MenuTask, built by func_08864FF0) at +0x100 and is reached through a
 * singleton pointer, or derives from the screen base with its vtable at 0
 * (constructor func_08A24048, destructor func_08A2405C, freed from the menu
 * root's heap by func_08A240C8). Every screen keeps its state in a separate
 * "work" block reached through one pointer field (+0x140, +0x14C, +0xF60 ...).
 * Structs are named after the singleton pointer or the vtable of their class
 * (ScreenE75C4 lives in D_089E75C4, Screen11510 has the vtable D_08A11510);
 * without either, after their first function here.
 *
 * Inside a work block the same pieces recur:
 *   layout    0x400 bytes (MenuLayout) handed to the script players
 *             func_088D3F00 / func_088D3CF0 / func_088D3C24 ... as their
 *             first argument;
 *   handles   the model handles those scripts drive (0x120 of them, 0x180
 *             in Screen0B6A0: the count passed to func_088D3C24 /
 *             func_088D3B5C); func_088D9F5C plays a motion on one,
 *             func_088D9CD8 / func_088D9CFC show and hide it;
 *   messages  the message source that func_0887B3EC / func_0887B500 read;
 *   texts     the objects func_0887B3EC makes (freed with func_0887BA5C),
 *             handed to func_088D44EC / func_088D428C after the handles.
 * The length of a texts array is not known; each is declared up to the
 * highest index used here.
 *
 * Fields whose use is not plain keep their offset as their name. */

#ifdef __cplusplus
extern "C" {
#endif

#define MENU1_OFFSET(type, field) ((unsigned int)&((type *)0)->field)
#define MENU1_ASSERT(name, cond) typedef char menu1_assert_##name[(cond) ? 1 : -1]
#define MENU1_SIZE(type, size) MENU1_ASSERT(size_##type, sizeof(type) == (size))
#define MENU1_AT(type, field, offset) \
    MENU1_ASSERT(at_##type##_##field, MENU1_OFFSET(type, field) == (offset))

typedef void (*MenuCallback)(void);
typedef void (*MenuCallback2)(int arg1, int arg2);

/* ------------------------------------------------------------ shared pieces */

/* Task object kept by the singleton screens: func_08864FF0 builds it,
 * func_0886503C sets its three callbacks, func_0886504C hands it a block
 * and a size, func_088650A4 starts it and func_08865158 stops it.
 * Size from Screen35A00, where the next field follows at +0x38. */
typedef struct MenuTask {
    char data[0x34];
} MenuTask;
MENU1_SIZE(MenuTask, 0x34);

/* Layout block played by the script functions (func_088D3F00 ...). */
typedef struct MenuLayout {
    void *entries[0x100];
} MenuLayout;
MENU1_SIZE(MenuLayout, 0x400);

/* A model handle as far as these functions read it (size not known). */
typedef struct MenuModel {
    char pad_00[0xC];
    short field_0C; /* -1 while it has nothing to show */
    char pad_0E[0x12];
    unsigned short field_20; /* bit 0x20 tested by the EE228/EE850 waits */
    char pad_22[0xE];
    int field_30;
    int field_34;
    int field_38;
} MenuModel;
MENU1_AT(MenuModel, field_0C, 0xC);
MENU1_AT(MenuModel, field_20, 0x20);
MENU1_AT(MenuModel, field_30, 0x30);
MENU1_AT(MenuModel, field_34, 0x34);
MENU1_AT(MenuModel, field_38, 0x38);

/* 0x2C-byte block that four screens fill the same way and hand to
 * func_088D4E00 (open, with a value first), func_088D4EC4, func_088D4F9C
 * and func_088D4FBC. */
typedef struct MenuPrompt {
    int field_00;
    int field_04;
    int field_08;
    void *field_0C; /* a layout entry */
    void *field_10; /* a model handle */
    void **field_14; /* texts */
    int field_18;
    void *field_1C; /* the screen's messages */
    int field_20;
    unsigned char *field_24; /* a table in the screen's data */
    unsigned short field_28;
    unsigned short field_2A;
} MenuPrompt;
MENU1_SIZE(MenuPrompt, 0x2C);
MENU1_AT(MenuPrompt, field_00, 0x0);
MENU1_AT(MenuPrompt, field_04, 0x4);
MENU1_AT(MenuPrompt, field_08, 0x8);
MENU1_AT(MenuPrompt, field_0C, 0xC);
MENU1_AT(MenuPrompt, field_10, 0x10);
MENU1_AT(MenuPrompt, field_14, 0x14);
MENU1_AT(MenuPrompt, field_18, 0x18);
MENU1_AT(MenuPrompt, field_1C, 0x1C);
MENU1_AT(MenuPrompt, field_20, 0x20);
MENU1_AT(MenuPrompt, field_24, 0x24);
MENU1_AT(MenuPrompt, field_28, 0x28);
MENU1_AT(MenuPrompt, field_2A, 0x2A);

/* Block set up by func_088D46E4 (first, last, ...) and read by
 * func_088D484C / func_088D4928 / func_088D4B74. Size from the spacing
 * of the two in Work0B6A0 (+0x3370, +0x339C). */
typedef struct MenuCursor {
    unsigned int flags;
    char pad_04[0x28];
} MenuCursor;
MENU1_SIZE(MenuCursor, 0x2C);

typedef struct Pair8 {
    int field_00;
    int field_04;
} Pair8;
MENU1_SIZE(Pair8, 0x8);
MENU1_AT(Pair8, field_00, 0x0);
MENU1_AT(Pair8, field_04, 0x4);

/* Record returned by func_088CB164 from the table at D_089D712C +0xF58. */
typedef struct Record8CB164 {
    char pad_00[0x1];
    unsigned char field_01;
    char pad_02[0x4];
    short field_06;
    char pad_08[0x4];
    int field_0C;
} Record8CB164;
MENU1_AT(Record8CB164, field_01, 0x1);
MENU1_AT(Record8CB164, field_06, 0x6);
MENU1_AT(Record8CB164, field_0C, 0xC);

/* ------------------------------------------------------------ globals */

/* D_089D712C as far as this region reads it. */
typedef struct Global9D712C {
    char pad_00[0xF58];
    unsigned char field_F58[4]; /* record table read by func_088CB164 (length not known) */
    char pad_F5C[0x1CEA4];
    int field_1DE00;
} Global9D712C;
MENU1_AT(Global9D712C, field_F58, 0xF58);
MENU1_AT(Global9D712C, field_1DE00, 0x1DE00);

typedef struct Global9D8140 {
    char pad_00[0x1C];
    unsigned int field_1C;
} Global9D8140;
MENU1_AT(Global9D8140, field_1C, 0x1C);

typedef struct Global9D5EC0 {
    char pad_00[0x710];
    void *field_710; /* a heap (func_0885C538) */
} Global9D5EC0;
MENU1_AT(Global9D5EC0, field_710, 0x710);

/* What D_08A29BF0 points to: five counts summed by func_08A29660. */
typedef struct Counts29BF0 {
    char pad_00[0x2];
    unsigned short field_02;
    unsigned short field_04;
    unsigned short field_06;
    unsigned short field_08;
    unsigned short field_0A;
} Counts29BF0;
MENU1_AT(Counts29BF0, field_02, 0x2);
MENU1_AT(Counts29BF0, field_04, 0x4);
MENU1_AT(Counts29BF0, field_06, 0x6);
MENU1_AT(Counts29BF0, field_08, 0x8);
MENU1_AT(Counts29BF0, field_0A, 0xA);

/* Entry of the 0x25-entry table D_08A0B1A8. */
typedef struct IdPair {
    unsigned short field_00;
    unsigned short field_02;
} IdPair;
MENU1_SIZE(IdPair, 0x4);
MENU1_AT(IdPair, field_00, 0x0);
MENU1_AT(IdPair, field_02, 0x2);

extern Global9D712C *D_089D712C;
extern Global9D8140 *D_089D8140;
extern Global9D5EC0 *D_089D5EC0;
extern Counts29BF0 *D_08A29BF0;
extern IdPair D_08A0B1A8[];
extern void *D_089D5FB4;
extern void *D_089D6998;
extern void *D_089D7F90;
extern void *D_089D727C;
extern void *D_089D6C30;
extern void *D_089D69E8;
extern void *D_089D7260;
extern void *D_089D6C80;
extern void *D_089D6D80;
extern void *D_089D6DC8;
extern void *D_089D4DC8;
extern void *D_08B01CF8;
extern void *D_08AC7500;
extern void *D_08ABB9E4;
extern void *D_08B59230;
extern void *D_08A457EC;     /* a second menu root (func_08A41454, func_08A412A8) */
extern unsigned char D_089EC1E0[];
extern unsigned char D_089EC230[];
extern unsigned char D_089EC280[];
extern unsigned char D_08A0B920[];
extern unsigned char D_08A11510[];
extern unsigned char D_08A15A88[];
extern unsigned char D_08A181E0[];
extern unsigned char D_08A1AB08[];
extern unsigned char D_08A240F8[];
extern unsigned char D_08A28560[];
extern unsigned char D_08A35550[];
extern unsigned char D_08A38EA8[];

/* ------------------------------------------------------------ ScreenE75C4 (D_089E75C4) */

/* Work block of ScreenE75C4 (+0x140). */
typedef struct WorkE75C4 {
    int state;
    unsigned int flags;
    MenuLayout layout;
    MenuModel *handles[0x120];
    MenuPrompt prompt;
    void *messages;
    void *texts[0x5A];
    void *field_A20; /* freed from the screen's heap */
    void *field_A24; /* released with func_0887E77C */
    char pad_A28[0x64];
    unsigned int field_A8C; /* 16-bit position handed to func_088D3AE8 */
    unsigned int field_A90;
    char pad_A94[0x8C];
    int field_B20;
    char pad_B24[0x30];
    int field_B54;
    int field_B58;
    int field_B5C;
    char pad_B60[0x404];
    char field_F64;
    char pad_F65[0x3];
    int field_F68;
    int field_F6C;
    int field_F70;
    int field_F74;
    int field_F78;
    void *field_F7C;
    void *field_F80;
    void *field_F84;
} WorkE75C4;
MENU1_AT(WorkE75C4, state, 0x0);
MENU1_AT(WorkE75C4, flags, 0x4);
MENU1_AT(WorkE75C4, layout, 0x8);
MENU1_AT(WorkE75C4, handles, 0x408);
MENU1_AT(WorkE75C4, prompt, 0x888);
MENU1_AT(WorkE75C4, messages, 0x8B4);
MENU1_AT(WorkE75C4, texts, 0x8B8);
MENU1_AT(WorkE75C4, field_A20, 0xA20);
MENU1_AT(WorkE75C4, field_A24, 0xA24);
MENU1_AT(WorkE75C4, field_A8C, 0xA8C);
MENU1_AT(WorkE75C4, field_A90, 0xA90);
MENU1_AT(WorkE75C4, field_B20, 0xB20);
MENU1_AT(WorkE75C4, field_B54, 0xB54);
MENU1_AT(WorkE75C4, field_B58, 0xB58);
MENU1_AT(WorkE75C4, field_B5C, 0xB5C);
MENU1_AT(WorkE75C4, field_F64, 0xF64);
MENU1_AT(WorkE75C4, field_F68, 0xF68);
MENU1_AT(WorkE75C4, field_F6C, 0xF6C);
MENU1_AT(WorkE75C4, field_F70, 0xF70);
MENU1_AT(WorkE75C4, field_F74, 0xF74);
MENU1_AT(WorkE75C4, field_F78, 0xF78);
MENU1_AT(WorkE75C4, field_F7C, 0xF7C);
MENU1_AT(WorkE75C4, field_F80, 0xF80);
MENU1_AT(WorkE75C4, field_F84, 0xF84);

/* Screen in D_089E75C4 (0x089E0AC0..0x089E3D80); constructor func_089E0B0C. */
typedef struct ScreenE75C4 {
    char field_000[0x100];
    MenuTask task;
    void *field_134; /* released with func_0888208C */
    void *heap;
    char pad_13C[0x4];
    WorkE75C4 *work;
} ScreenE75C4;
MENU1_AT(ScreenE75C4, field_000, 0x0);
MENU1_AT(ScreenE75C4, task, 0x100);
MENU1_AT(ScreenE75C4, field_134, 0x134);
MENU1_AT(ScreenE75C4, heap, 0x138);
MENU1_AT(ScreenE75C4, work, 0x140);

extern ScreenE75C4 *D_089E75C4;

/* ------------------------------------------------------------ ScreenE9D38 (D_089E9D38) */

typedef struct PartE8E78 {
    char pad_00[0x15C];
    char field_15C;
} PartE8E78;
MENU1_AT(PartE8E78, field_15C, 0x15C);

/* Screen in D_089E9D38 (0x089E7DF0..0x089E94E8). */
typedef struct ScreenE9D38 {
    char pad_00[0x100];
    MenuTask task;
    char pad_134[0x8];
    PartE8E78 *field_13C;
    void *field_140;
    void *field_144;
    unsigned int field_148;
    char pad_14C[0x8];
    int field_154;
    int field_158;
    char pad_15C[0x10];
    int field_16C;
    int field_170;
    char pad_174[0x4];
    unsigned int field_178;
} ScreenE9D38;
MENU1_AT(ScreenE9D38, task, 0x100);
MENU1_AT(ScreenE9D38, field_13C, 0x13C);
MENU1_AT(ScreenE9D38, field_140, 0x140);
MENU1_AT(ScreenE9D38, field_144, 0x144);
MENU1_AT(ScreenE9D38, field_148, 0x148);
MENU1_AT(ScreenE9D38, field_154, 0x154);
MENU1_AT(ScreenE9D38, field_158, 0x158);
MENU1_AT(ScreenE9D38, field_16C, 0x16C);
MENU1_AT(ScreenE9D38, field_170, 0x170);
MENU1_AT(ScreenE9D38, field_178, 0x178);

extern ScreenE9D38 *D_089E9D38;

/* ------------------------------------------------------------ small objects 0x089E9E70..0x089EEC30 */

/* Cleared by func_089E9E70. */
typedef struct Block9E70 {
    char field_00[8];
    char field_08[8];
    char pad_10[0x4];
    int field_14;
} Block9E70;
MENU1_AT(Block9E70, field_00, 0x0);
MENU1_AT(Block9E70, field_08, 0x8);
MENU1_AT(Block9E70, field_14, 0x14);

/* Any object with its vtable at 0. */
typedef struct MenuObject {
    void *vtable;
} MenuObject;

/* vtable D_089EC1E0; constructor func_089EAD30, base destructor func_089EACFC. */
typedef struct ObjC1E0 {
    void *vtable;
    MenuModel *model;
    int field_08;
} ObjC1E0;
MENU1_AT(ObjC1E0, vtable, 0x0);
MENU1_AT(ObjC1E0, model, 0x4);
MENU1_AT(ObjC1E0, field_08, 0x8);

/* vtable D_089EC230 (constructor func_089EAFA0, hand-written). */
typedef struct StepCounter {
    void *vtable;
    MenuModel *field_04[14]; /* cleared together; [0] is a model */
    int field_3C;
    int field_40;
    int field_44;
    int field_48;
    unsigned char field_4C;
    char pad_4D[0x3];
    int field_50;
} StepCounter;
MENU1_SIZE(StepCounter, 0x54);
MENU1_AT(StepCounter, vtable, 0x0);
MENU1_AT(StepCounter, field_04, 0x4);
MENU1_AT(StepCounter, field_3C, 0x3C);
MENU1_AT(StepCounter, field_40, 0x40);
MENU1_AT(StepCounter, field_44, 0x44);
MENU1_AT(StepCounter, field_48, 0x48);
MENU1_AT(StepCounter, field_4C, 0x4C);
MENU1_AT(StepCounter, field_50, 0x50);

typedef struct ObjECB40 {
    char pad_00[0x140];
    unsigned char field_140;
} ObjECB40;
MENU1_AT(ObjECB40, field_140, 0x140);

/* Element built by func_089EDDD8. */
typedef struct ElemEDDD8 {
    char data[0x18];
} ElemEDDD8;
MENU1_SIZE(ElemEDDD8, 0x18);

/* Constructor func_089EE228. */
typedef struct ObjEE228 {
    MenuModel *models[3];
    int field_0C;
    ElemEDDD8 field_10[3];
    int field_58;
    int field_5C;
    int state;
} ObjEE228;
MENU1_SIZE(ObjEE228, 0x64);
MENU1_AT(ObjEE228, models, 0x0);
MENU1_AT(ObjEE228, field_0C, 0xC);
MENU1_AT(ObjEE228, field_10, 0x10);
MENU1_AT(ObjEE228, field_58, 0x58);
MENU1_AT(ObjEE228, field_5C, 0x5C);
MENU1_AT(ObjEE228, state, 0x60);

/* Constructor func_089EE850. */
typedef struct ObjEE850 {
    MenuModel *models[6];
    int field_18;
    int field_1C;
    int state;
} ObjEE850;
MENU1_SIZE(ObjEE850, 0x24);
MENU1_AT(ObjEE850, models, 0x0);
MENU1_AT(ObjEE850, field_18, 0x18);
MENU1_AT(ObjEE850, field_1C, 0x1C);
MENU1_AT(ObjEE850, state, 0x20);

/* ------------------------------------------------------------ Screen0B6A0 (D_08A0B6A0) */

/* Header of the data block at Work0B6A0.field_04: the counts of the
 * tables that follow it (func_08A05B0C). */
typedef struct MenuDataHeader {
    char pad_00[0x10];
    short field_10;
    short field_12;
    short field_14;
    short field_16;
    short field_18;
    short field_1A;
    short field_1C;
    short field_1E;
    short field_20;
    short field_22;
    short field_24;
    char pad_26[0x2];
    short field_28;
    char pad_2A[0x2];
} MenuDataHeader;
MENU1_SIZE(MenuDataHeader, 0x2C);
MENU1_AT(MenuDataHeader, field_10, 0x10);
MENU1_AT(MenuDataHeader, field_12, 0x12);
MENU1_AT(MenuDataHeader, field_14, 0x14);
MENU1_AT(MenuDataHeader, field_16, 0x16);
MENU1_AT(MenuDataHeader, field_18, 0x18);
MENU1_AT(MenuDataHeader, field_1A, 0x1A);
MENU1_AT(MenuDataHeader, field_1C, 0x1C);
MENU1_AT(MenuDataHeader, field_1E, 0x1E);
MENU1_AT(MenuDataHeader, field_20, 0x20);
MENU1_AT(MenuDataHeader, field_22, 0x22);
MENU1_AT(MenuDataHeader, field_24, 0x24);
MENU1_AT(MenuDataHeader, field_28, 0x28);

typedef struct Row18 {
    char pad_00[0xD];
    unsigned char field_0D;
    char pad_0E[0x6];
    short field_14;
    short field_16;
} Row18;
MENU1_SIZE(Row18, 0x18);
MENU1_AT(Row18, field_0D, 0xD);
MENU1_AT(Row18, field_14, 0x14);
MENU1_AT(Row18, field_16, 0x16);

typedef struct Row20 {
    char pad_00[0x6];
    short field_06;
    char pad_08[0x8];
    short field_10;
    char pad_12[0xE];
} Row20;
MENU1_SIZE(Row20, 0x20);
MENU1_AT(Row20, field_06, 0x6);
MENU1_AT(Row20, field_10, 0x10);

typedef struct Obj68 {
    char pad_00[0x8];
    short field_08;
} Obj68;
MENU1_AT(Obj68, field_08, 0x8);

/* One of the nine entries at Work0B6A0 +0x29CC. */
typedef struct Entry84 {
    unsigned char kind;
    char field_01;
    char pad_02[0x1];
    unsigned char field_03;
    unsigned int flags;
    char pad_08[0x4];
    char field_0C;
    char pad_0D[0x1];
    short field_0E;
    char pad_10[0x74];
} Entry84;
MENU1_SIZE(Entry84, 0x84);
MENU1_AT(Entry84, kind, 0x0);
MENU1_AT(Entry84, field_01, 0x1);
MENU1_AT(Entry84, field_03, 0x3);
MENU1_AT(Entry84, flags, 0x4);
MENU1_AT(Entry84, field_0C, 0xC);
MENU1_AT(Entry84, field_0E, 0xE);

typedef struct Slot4 {
    char field_00;
    unsigned char field_01;
    char pad_02[0x2];
} Slot4;
MENU1_SIZE(Slot4, 0x4);
MENU1_AT(Slot4, field_00, 0x0);
MENU1_AT(Slot4, field_01, 0x1);

/* Block at Work0B6A0 +0x3158 handed to func_089EF640 and its siblings. */
typedef struct Block3158 {
    int field_00;
    unsigned int field_04;
    char pad_08[0xC];
    short field_14;
} Block3158;
MENU1_AT(Block3158, field_00, 0x0);
MENU1_AT(Block3158, field_04, 0x4);
MENU1_AT(Block3158, field_14, 0x14);

/* Work block of Screen0B6A0 (+0x14C). */
typedef struct Work0B6A0 {
    char pad_00[0x4];
    char *field_04; /* data block; func_08A05B0C points the next fields into it */
    MenuDataHeader *header;
    char *field_0C;
    char *field_10;
    char *field_14;
    char *field_18;
    char *field_1C;
    char *field_20;
    char *field_24;
    char *field_28;
    char *field_2C;
    char *field_30;
    char *field_34;
    char *field_38;
    char *field_3C;
    MenuDataHeader *field_40;
    Row20 *field_44;
    short *field_48;
    char pad_4C[0x18];
    Pair8 *field_64;
    Obj68 *field_68;
    char pad_6C[0x1C];
    MenuLayout layout;
    MenuModel *handles[0x180];
    void *messages;
    void *texts[0x71];
    char pad_C50[0x1AA8];
    MenuPrompt prompt;
    unsigned int flags;
    char pad_2728[0x8];
    unsigned char field_2730;
    char field_2731;
    char field_2732;
    char pad_2733[0x1];
    unsigned short field_2734;
    unsigned short field_2736;
    char pad_2738[0x4];
    unsigned int field_273C;
    int field_2740;
    char pad_2744[0x84];
    int field_27C8;
    char pad_27CC[0x200];
    Entry84 entries[9];
    Slot4 slots[5];
    int field_2E84[5]; /* record ids (func_088CB164) */
    char pad_2E98[0x238];
    char field_30D0;
    char pad_30D1[0x87];
    Block3158 field_3158;
    char pad_3170[0xA4];
    int field_3214;
    short field_3218;
    short field_321A;
    char pad_321C[0x8];
    int field_3224;
    int field_3228;
    unsigned short field_322C;
    unsigned short field_322E;
    char pad_3230[0x8];
    int field_3238;
    char pad_323C[0x1C];
    short field_3258;
    short field_325A;
    char pad_325C[0x10];
    short field_326C;
    short field_326E;
    char pad_3270[0x8];
    int state;
    char pad_327C[0x4];
    unsigned int field_3280;
    char pad_3284[0x60];
    int field_32E4;
    int field_32E8;
    int field_32EC;
    int field_32F0;
    char pad_32F4[0x7C];
    MenuCursor field_3370;
    MenuCursor field_339C;
    char pad_33C8[0x20];
    int field_33E8;
    char pad_33EC[0x24];
    unsigned char field_3410;
    char pad_3411[0x7];
    int field_3418;
    int field_341C;
    int field_3420;
    char pad_3424[0x4];
    int field_3428;
    char pad_342C[0x4];
    int field_3430;
    char pad_3434[0x20];
    int field_3454;
    char pad_3458[0x8];
    int field_3460;
    char pad_3464[0x43C];
    int field_38A0;
    unsigned int field_38A4;
    int field_38A8;
    MenuModel *handles2[0x40]; /* second handle set (func_088D3B5C with 0x40) */
    char pad_39AC[0x26C5C];
    int field_2A608;
} Work0B6A0;
MENU1_AT(Work0B6A0, field_04, 0x4);
MENU1_AT(Work0B6A0, header, 0x8);
MENU1_AT(Work0B6A0, field_0C, 0xC);
MENU1_AT(Work0B6A0, field_10, 0x10);
MENU1_AT(Work0B6A0, field_14, 0x14);
MENU1_AT(Work0B6A0, field_18, 0x18);
MENU1_AT(Work0B6A0, field_1C, 0x1C);
MENU1_AT(Work0B6A0, field_20, 0x20);
MENU1_AT(Work0B6A0, field_24, 0x24);
MENU1_AT(Work0B6A0, field_28, 0x28);
MENU1_AT(Work0B6A0, field_2C, 0x2C);
MENU1_AT(Work0B6A0, field_30, 0x30);
MENU1_AT(Work0B6A0, field_34, 0x34);
MENU1_AT(Work0B6A0, field_38, 0x38);
MENU1_AT(Work0B6A0, field_3C, 0x3C);
MENU1_AT(Work0B6A0, field_40, 0x40);
MENU1_AT(Work0B6A0, field_44, 0x44);
MENU1_AT(Work0B6A0, field_48, 0x48);
MENU1_AT(Work0B6A0, field_64, 0x64);
MENU1_AT(Work0B6A0, field_68, 0x68);
MENU1_AT(Work0B6A0, layout, 0x88);
MENU1_AT(Work0B6A0, handles, 0x488);
MENU1_AT(Work0B6A0, messages, 0xA88);
MENU1_AT(Work0B6A0, texts, 0xA8C);
MENU1_AT(Work0B6A0, prompt, 0x26F8);
MENU1_AT(Work0B6A0, flags, 0x2724);
MENU1_AT(Work0B6A0, field_2730, 0x2730);
MENU1_AT(Work0B6A0, field_2731, 0x2731);
MENU1_AT(Work0B6A0, field_2732, 0x2732);
MENU1_AT(Work0B6A0, field_2734, 0x2734);
MENU1_AT(Work0B6A0, field_2736, 0x2736);
MENU1_AT(Work0B6A0, field_273C, 0x273C);
MENU1_AT(Work0B6A0, field_2740, 0x2740);
MENU1_AT(Work0B6A0, field_27C8, 0x27C8);
MENU1_AT(Work0B6A0, entries, 0x29CC);
MENU1_AT(Work0B6A0, slots, 0x2E70);
MENU1_AT(Work0B6A0, field_2E84, 0x2E84);
MENU1_AT(Work0B6A0, field_30D0, 0x30D0);
MENU1_AT(Work0B6A0, field_3158, 0x3158);
MENU1_AT(Work0B6A0, field_3214, 0x3214);
MENU1_AT(Work0B6A0, field_3218, 0x3218);
MENU1_AT(Work0B6A0, field_321A, 0x321A);
MENU1_AT(Work0B6A0, field_3224, 0x3224);
MENU1_AT(Work0B6A0, field_3228, 0x3228);
MENU1_AT(Work0B6A0, field_322C, 0x322C);
MENU1_AT(Work0B6A0, field_322E, 0x322E);
MENU1_AT(Work0B6A0, field_3238, 0x3238);
MENU1_AT(Work0B6A0, field_3258, 0x3258);
MENU1_AT(Work0B6A0, field_325A, 0x325A);
MENU1_AT(Work0B6A0, field_326C, 0x326C);
MENU1_AT(Work0B6A0, field_326E, 0x326E);
MENU1_AT(Work0B6A0, state, 0x3278);
MENU1_AT(Work0B6A0, field_3280, 0x3280);
MENU1_AT(Work0B6A0, field_32E4, 0x32E4);
MENU1_AT(Work0B6A0, field_32E8, 0x32E8);
MENU1_AT(Work0B6A0, field_32EC, 0x32EC);
MENU1_AT(Work0B6A0, field_32F0, 0x32F0);
MENU1_AT(Work0B6A0, field_3370, 0x3370);
MENU1_AT(Work0B6A0, field_339C, 0x339C);
MENU1_AT(Work0B6A0, field_33E8, 0x33E8);
MENU1_AT(Work0B6A0, field_3410, 0x3410);
MENU1_AT(Work0B6A0, field_3418, 0x3418);
MENU1_AT(Work0B6A0, field_341C, 0x341C);
MENU1_AT(Work0B6A0, field_3420, 0x3420);
MENU1_AT(Work0B6A0, field_3428, 0x3428);
MENU1_AT(Work0B6A0, field_3430, 0x3430);
MENU1_AT(Work0B6A0, field_3454, 0x3454);
MENU1_AT(Work0B6A0, field_3460, 0x3460);
MENU1_AT(Work0B6A0, field_38A0, 0x38A0);
MENU1_AT(Work0B6A0, field_38A4, 0x38A4);
MENU1_AT(Work0B6A0, field_38A8, 0x38A8);
MENU1_AT(Work0B6A0, handles2, 0x38AC);
MENU1_AT(Work0B6A0, field_2A608, 0x2A608);

/* Screen in D_08A0B6A0 (0x089EF150..0x08A05B0C); constructor func_089FE524. */
typedef struct Screen0B6A0 {
    char field_000[0x100];
    MenuTask task;
    void *field_134; /* released with func_0888208C */
    void *heap;
    char pad_13C[0x10];
    Work0B6A0 *work;
} Screen0B6A0;
MENU1_AT(Screen0B6A0, field_000, 0x0);
MENU1_AT(Screen0B6A0, task, 0x100);
MENU1_AT(Screen0B6A0, field_134, 0x134);
MENU1_AT(Screen0B6A0, heap, 0x138);
MENU1_AT(Screen0B6A0, work, 0x14C);

extern Screen0B6A0 *D_08A0B6A0;

/* vtable D_08A0B920; constructor func_08A0B740. */
typedef struct Delegate0B920 {
    void *vtable;
    Screen0B6A0 *owner;
    int field_08;
} Delegate0B920;
MENU1_SIZE(Delegate0B920, 0xC);
MENU1_AT(Delegate0B920, vtable, 0x0);
MENU1_AT(Delegate0B920, owner, 0x4);
MENU1_AT(Delegate0B920, field_08, 0x8);

/* ------------------------------------------------------------ menu root (D_08A23FF0) */

typedef struct Row154 {
    char data[0x154];
} Row154;
MENU1_SIZE(Row154, 0x154);

/* Work block of the menu root (+0x154). */
typedef struct RootWork {
    char pad_00[0x88C];
    void *messages;
    char pad_890[0x4C];
    void *field_8DC; /* start of a texts run (MenuPrompt.field_14) */
    char pad_8E0[0x4A0];
    unsigned int field_D80;
    char pad_D84[0x10];
    int field_D94;
    char pad_D98[0x10AC];
    int field_1E44;
    Row154 field_1E48[10];
    int field_2B90[10];
    char pad_2BB8[0x3BC4];
    short field_677C[0x1C];
    int field_67B4;
    char pad_67B8[0x98];
    int field_6850;
    char pad_6854[0x8C];
    char field_68E0[0x34];
    void *field_6914;
    char pad_6918[0x8];
    MenuCallback field_6920;
    MenuCallback field_6924;
    char pad_6928[0x8];
    MenuPrompt prompt;
    char pad_695C[0x8];
    void *field_6964;
    int state;
    char pad_696C[0x24F4];
    unsigned int flags;
    char pad_8E64[0x1CD20];
    int field_25B84;
    char pad_25B88[0xC];
    int field_25B94;
    char pad_25B98[0xB8];
    int field_25C50;
    char pad_25C54[0x34];
    int field_25C88;
    int field_25C8C;
    int field_25C90;
    char pad_25C94[0x4];
    int field_25C98;
} RootWork;
MENU1_AT(RootWork, messages, 0x88C);
MENU1_AT(RootWork, field_8DC, 0x8DC);
MENU1_AT(RootWork, field_D80, 0xD80);
MENU1_AT(RootWork, field_D94, 0xD94);
MENU1_AT(RootWork, field_1E44, 0x1E44);
MENU1_AT(RootWork, field_1E48, 0x1E48);
MENU1_AT(RootWork, field_2B90, 0x2B90);
MENU1_AT(RootWork, field_677C, 0x677C);
MENU1_AT(RootWork, field_67B4, 0x67B4);
MENU1_AT(RootWork, field_6850, 0x6850);
MENU1_AT(RootWork, field_68E0, 0x68E0);
MENU1_AT(RootWork, field_6914, 0x6914);
MENU1_AT(RootWork, field_6920, 0x6920);
MENU1_AT(RootWork, field_6924, 0x6924);
MENU1_AT(RootWork, prompt, 0x6930);
MENU1_AT(RootWork, field_6964, 0x6964);
MENU1_AT(RootWork, state, 0x6968);
MENU1_AT(RootWork, flags, 0x8E60);
MENU1_AT(RootWork, field_25B84, 0x25B84);
MENU1_AT(RootWork, field_25B94, 0x25B94);
MENU1_AT(RootWork, field_25C50, 0x25C50);
MENU1_AT(RootWork, field_25C88, 0x25C88);
MENU1_AT(RootWork, field_25C8C, 0x25C8C);
MENU1_AT(RootWork, field_25C90, 0x25C90);
MENU1_AT(RootWork, field_25C98, 0x25C98);

typedef struct Root140 {
    char pad_00[0x2C90];
    int field_2C90;
} Root140;
MENU1_AT(Root140, field_2C90, 0x2C90);

/* The menu root in D_08A23FF0 (0x08A1B394..0x08A20ACC). */
typedef struct MenuRoot {
    char pad_00[0x134];
    void *field_134; /* released with func_0888208C */
    void *heap; /* the screens are freed from it (func_08A240C8) */
    int field_13C;
    Root140 *field_140;
    char pad_144[0x10];
    RootWork *work;
} MenuRoot;
MENU1_AT(MenuRoot, field_134, 0x134);
MENU1_AT(MenuRoot, heap, 0x138);
MENU1_AT(MenuRoot, field_13C, 0x13C);
MENU1_AT(MenuRoot, field_140, 0x140);
MENU1_AT(MenuRoot, work, 0x154);

extern MenuRoot *D_08A23FF0;

/* Forwarders 0x08A20998..0x08A20AB0 (owner at +4). */
typedef struct DelegateRoot {
    void *field_00;
    MenuRoot *owner;
} DelegateRoot;
MENU1_AT(DelegateRoot, field_00, 0x0);
MENU1_AT(DelegateRoot, owner, 0x4);

/* func_08A20ACC (owner at +8). */
typedef struct DelegateRoot8 {
    void *field_00;
    int field_04;
    MenuRoot *owner;
} DelegateRoot8;
MENU1_AT(DelegateRoot8, field_00, 0x0);
MENU1_AT(DelegateRoot8, field_04, 0x4);
MENU1_AT(DelegateRoot8, owner, 0x8);

/* ------------------------------------------------------------ screens on the vtable base */

/* Work block of Screen11510 (+0x1320). */
typedef struct Work11510 {
    MenuLayout layout;
    MenuModel *handles[0x120];
    void *texts[0x58];
    char pad_9E0[0x5F0];
    int state;
    char pad_FD4[0x3C];
    unsigned int flags;
    char pad_1014[0x4];
    int field_1018;
    char pad_101C[0xD4];
    MenuCursor field_10F0;
    char pad_111C[0x20];
    int field_113C;
} Work11510;
MENU1_AT(Work11510, layout, 0x0);
MENU1_AT(Work11510, handles, 0x400);
MENU1_AT(Work11510, texts, 0x880);
MENU1_AT(Work11510, state, 0xFD0);
MENU1_AT(Work11510, flags, 0x1010);
MENU1_AT(Work11510, field_1018, 0x1018);
MENU1_AT(Work11510, field_10F0, 0x10F0);
MENU1_AT(Work11510, field_113C, 0x113C);

/* vtable D_08A11510 (0x08A0BEEC..0x08A0E284). */
typedef struct Screen11510 {
    void *vtable;
    char pad_04[0x131C];
    Work11510 *work;
} Screen11510;
MENU1_AT(Screen11510, vtable, 0x0);
MENU1_AT(Screen11510, work, 0x1320);

/* Board built by func_08A28B74 (hand-written: sixteen 0x6B18-byte cells). */
typedef struct Board {
    char pad_00[0x6B3F4];
    int field_6B3F4;
    char pad_6B3F8[0x8];
    int field_6B400;
} Board;
MENU1_SIZE(Board, 0x6B404);
MENU1_AT(Board, field_6B3F4, 0x6B3F4);
MENU1_AT(Board, field_6B400, 0x6B400);

/* Work block of Screen15A88 (+0xF60). */
typedef struct Work15A88 {
    MenuLayout layout;
    MenuModel *handles[0x120];
    void *texts[1];
    char pad_884[0x408];
    int state;
    unsigned int flags;
    char pad_C94[0x1C];
    int field_CB0;
    char pad_CB4[0x214];
    void *field_EC8; /* the board's block in field_EFC */
    Board *board;
    int field_ED0;
    char pad_ED4[0x8];
    int field_EDC;
    int field_EE0;
    char pad_EE4[0x14];
    void *field_EF8;
    void *field_EFC; /* a heap */
} Work15A88;
MENU1_AT(Work15A88, layout, 0x0);
MENU1_AT(Work15A88, handles, 0x400);
MENU1_AT(Work15A88, texts, 0x880);
MENU1_AT(Work15A88, state, 0xC8C);
MENU1_AT(Work15A88, flags, 0xC90);
MENU1_AT(Work15A88, field_CB0, 0xCB0);
MENU1_AT(Work15A88, field_EC8, 0xEC8);
MENU1_AT(Work15A88, board, 0xECC);
MENU1_AT(Work15A88, field_ED0, 0xED0);
MENU1_AT(Work15A88, field_EDC, 0xEDC);
MENU1_AT(Work15A88, field_EE0, 0xEE0);
MENU1_AT(Work15A88, field_EF8, 0xEF8);
MENU1_AT(Work15A88, field_EFC, 0xEFC);

/* vtable D_08A15A88 (0x08A11EF4..0x08A13E20). */
typedef struct Screen15A88 {
    void *vtable;
    char pad_04[0xF5C];
    Work15A88 *work;
} Screen15A88;
MENU1_AT(Screen15A88, vtable, 0x0);
MENU1_AT(Screen15A88, work, 0xF60);

/* Forwarders 0x08A13F64..0x08A13FF0. */
typedef struct Delegate15A88 {
    void *field_00;
    Screen15A88 *owner;
} Delegate15A88;
MENU1_AT(Delegate15A88, field_00, 0x0);
MENU1_AT(Delegate15A88, owner, 0x4);

/* Work block of Screen181E0 (+0xD80). */
typedef struct Work181E0 {
    MenuLayout layout;
    MenuModel *handles[0x120];
    void *texts[1];
    char pad_884[0x3F8];
    unsigned int field_C7C;
    char pad_C80[0x5C];
    int field_CDC;
} Work181E0;
MENU1_AT(Work181E0, layout, 0x0);
MENU1_AT(Work181E0, handles, 0x400);
MENU1_AT(Work181E0, texts, 0x880);
MENU1_AT(Work181E0, field_C7C, 0xC7C);
MENU1_AT(Work181E0, field_CDC, 0xCDC);

/* vtable D_08A181E0 (0x08A1602C..0x08A175A4). */
typedef struct Screen181E0 {
    void *vtable;
    char pad_04[0xD7C];
    Work181E0 *work;
} Screen181E0;
MENU1_AT(Screen181E0, vtable, 0x0);
MENU1_AT(Screen181E0, work, 0xD80);

/* Work block of Screen1AB08 (+0x17B0). */
typedef struct Work1AB08 {
    MenuLayout layout;
    MenuModel *handles[0x120];
    void *texts[1];
    char pad_884[0x874];
    int field_10F8;
    char pad_10FC[0x28];
    int field_1124;
    int field_1128;
    char pad_112C[0x414];
    int field_1540;
    char pad_1544[0x13C];
    Pair8 field_1680[4];
    int field_16A0;
    int field_16A4;
    int field_16A8;
    int field_16AC;
    char pad_16B0[0x4];
    int field_16B4;
    char pad_16B8[0x28];
    int field_16E0;
    char pad_16E4[0x24];
    int field_1708;
} Work1AB08;
MENU1_AT(Work1AB08, layout, 0x0);
MENU1_AT(Work1AB08, handles, 0x400);
MENU1_AT(Work1AB08, texts, 0x880);
MENU1_AT(Work1AB08, field_10F8, 0x10F8);
MENU1_AT(Work1AB08, field_1124, 0x1124);
MENU1_AT(Work1AB08, field_1128, 0x1128);
MENU1_AT(Work1AB08, field_1540, 0x1540);
MENU1_AT(Work1AB08, field_1680, 0x1680);
MENU1_AT(Work1AB08, field_16A0, 0x16A0);
MENU1_AT(Work1AB08, field_16A4, 0x16A4);
MENU1_AT(Work1AB08, field_16A8, 0x16A8);
MENU1_AT(Work1AB08, field_16AC, 0x16AC);
MENU1_AT(Work1AB08, field_16B4, 0x16B4);
MENU1_AT(Work1AB08, field_16E0, 0x16E0);
MENU1_AT(Work1AB08, field_1708, 0x1708);

/* vtable D_08A1AB08 (0x08A18CE0..0x08A19690). */
typedef struct Screen1AB08 {
    void *vtable;
    char pad_04[0x17AC];
    Work1AB08 *work;
} Screen1AB08;
MENU1_AT(Screen1AB08, vtable, 0x0);
MENU1_AT(Screen1AB08, work, 0x17B0);

typedef struct Row1E9C {
    int field_00;
    char pad_04[0xC];
} Row1E9C;
MENU1_SIZE(Row1E9C, 0x10);

/* Work block of Screen28560 (+0x28F0). */
typedef struct Work28560 {
    MenuLayout layout;
    MenuModel *handles[0x120];
    void *texts[1];
    char pad_884[0x1618];
    Row1E9C field_1E9C[19];
    char pad_1FCC[0x8];
    short field_1FD4[0x16];
    int field_2000;
    char pad_2004[0x40];
    MenuCursor field_2044;
    char pad_2070[0x39C];
    int field_240C;
    char pad_2410[0x30];
    unsigned short field_2440;
    char pad_2442[0xA];
    int field_244C;
    char pad_2450[0x4C];
    int field_249C;
    char pad_24A0[0x108];
    unsigned char field_25A8;
} Work28560;
MENU1_AT(Work28560, layout, 0x0);
MENU1_AT(Work28560, handles, 0x400);
MENU1_AT(Work28560, texts, 0x880);
MENU1_AT(Work28560, field_1E9C, 0x1E9C);
MENU1_AT(Work28560, field_1FD4, 0x1FD4);
MENU1_AT(Work28560, field_2000, 0x2000);
MENU1_AT(Work28560, field_2044, 0x2044);
MENU1_AT(Work28560, field_240C, 0x240C);
MENU1_AT(Work28560, field_2440, 0x2440);
MENU1_AT(Work28560, field_244C, 0x244C);
MENU1_AT(Work28560, field_249C, 0x249C);
MENU1_AT(Work28560, field_25A8, 0x25A8);

/* vtable D_08A28560 (0x08A24304..0x08A2594C). */
typedef struct Screen28560 {
    void *vtable;
    char pad_04[0x28EC];
    Work28560 *work;
} Screen28560;
MENU1_AT(Screen28560, vtable, 0x0);
MENU1_AT(Screen28560, work, 0x28F0);

/* Forwarders 0x08A293DC..0x08A29468. */
typedef struct DelegateBoard {
    void *field_00;
    Board *owner;
} DelegateBoard;
MENU1_AT(DelegateBoard, field_00, 0x0);
MENU1_AT(DelegateBoard, owner, 0x4);

/* ------------------------------------------------------------ ScreenE75C8 (D_089E75C8) */

typedef struct Triple {
    int field_00;
    int field_04;
    int field_08;
} Triple;
MENU1_SIZE(Triple, 0xC);
MENU1_AT(Triple, field_00, 0x0);
MENU1_AT(Triple, field_04, 0x4);
MENU1_AT(Triple, field_08, 0x8);

/* Work block of ScreenE75C8 (+0x138). */
typedef struct WorkE75C8 {
    char pad_00[0x448];
    MenuModel *field_448;
    MenuModel *field_44C;
    char pad_450[0x18];
    MenuModel *field_468;
    MenuModel *field_46C;
    MenuModel *field_470[7];
    MenuModel *field_48C[7];
    MenuModel *field_4A8[7];
    MenuModel *field_4C4[7];
    char pad_4E0[0x3C];
    MenuModel *field_51C;
    char pad_520[0x36C];
    void *messages;
    char pad_890[0x98];
    void *field_928; /* a text (func_0887B778) */
    char pad_92C[0x248];
    Triple field_B74;
    char pad_B80[0x974];
    int field_14F4;
    unsigned int flags;
    int state;
} WorkE75C8;
MENU1_AT(WorkE75C8, field_448, 0x448);
MENU1_AT(WorkE75C8, field_44C, 0x44C);
MENU1_AT(WorkE75C8, field_468, 0x468);
MENU1_AT(WorkE75C8, field_46C, 0x46C);
MENU1_AT(WorkE75C8, field_470, 0x470);
MENU1_AT(WorkE75C8, field_48C, 0x48C);
MENU1_AT(WorkE75C8, field_4A8, 0x4A8);
MENU1_AT(WorkE75C8, field_4C4, 0x4C4);
MENU1_AT(WorkE75C8, field_51C, 0x51C);
MENU1_AT(WorkE75C8, messages, 0x88C);
MENU1_AT(WorkE75C8, field_928, 0x928);
MENU1_AT(WorkE75C8, field_B74, 0xB74);
MENU1_AT(WorkE75C8, field_14F4, 0x14F4);
MENU1_AT(WorkE75C8, flags, 0x14F8);
MENU1_AT(WorkE75C8, state, 0x14FC);

/* Screen in D_089E75C8 (0x08A29D0C..0x08A2BCE4); constructor func_08A2BCE4. */
typedef struct ScreenE75C8 {
    char field_000[0x100]; /* handed to the task with func_0886504C */
    MenuTask task;
    void *field_134; /* a heap */
    WorkE75C8 *work;
} ScreenE75C8;
MENU1_AT(ScreenE75C8, field_000, 0x0);
MENU1_AT(ScreenE75C8, task, 0x100);
MENU1_AT(ScreenE75C8, field_134, 0x134);
MENU1_AT(ScreenE75C8, work, 0x138);

extern ScreenE75C8 *D_089E75C8;

/* ------------------------------------------------------------ screens on the second base (func_08A408B4) */

/* Work block of Screen35550 (+0x6DC). */
typedef struct Work35550 {
    MenuLayout *layout;
    MenuModel **handles;
    char pad_08[0x4];
    void *texts[0x14];
    char pad_5C[0xC4];
    int field_120; /* layout rows for func_08A41454 */
    char pad_124[0x584];
    int state;
    char pad_6AC[0x27];
    char field_6D3;
} Work35550;
MENU1_AT(Work35550, layout, 0x0);
MENU1_AT(Work35550, handles, 0x4);
MENU1_AT(Work35550, texts, 0xC);
MENU1_AT(Work35550, field_120, 0x120);
MENU1_AT(Work35550, state, 0x6A8);
MENU1_AT(Work35550, field_6D3, 0x6D3);

/* vtable D_08A35550 (0x08A2EB10..0x08A30C64). */
typedef struct Screen35550 {
    void *vtable;
    char pad_04[0x6D8];
    Work35550 *work;
} Screen35550;
MENU1_AT(Screen35550, vtable, 0x0);
MENU1_AT(Screen35550, work, 0x6DC);

typedef struct Obj367CC {
    char pad_00[0x4];
    int field_04;
    char pad_08[0x28];
    char field_30;
} Obj367CC;
MENU1_AT(Obj367CC, field_04, 0x4);
MENU1_AT(Obj367CC, field_30, 0x30);

/* Work block of Screen38EA8 (+0xF64). */
typedef struct Work38EA8 {
    char pad_00[0xC];
    void *texts[0x17];
    char pad_68[0x4];
    int field_6C;
    char pad_70[0x2C];
    int field_9C;
    char pad_A0[0x24];
    MenuCursor field_C4;
    char pad_F0[0x374];
    short field_464[7][12]; /* layout rows for func_08A41454 */
    char pad_50C[0x848];
    Obj367CC *field_D54;
    int field_D58;
    int field_D5C;
    int field_D60;
    int field_D64;
    char pad_D68[0x1C];
    int field_D84;
    int field_D88;
    int field_D8C;
    char pad_D90[0x18];
    int field_DA8;
    int field_DAC;
    int field_DB0;
    char pad_DB4[0x1];
    unsigned char field_DB5;
    char field_DB6;
} Work38EA8;
MENU1_AT(Work38EA8, texts, 0xC);
MENU1_AT(Work38EA8, field_6C, 0x6C);
MENU1_AT(Work38EA8, field_9C, 0x9C);
MENU1_AT(Work38EA8, field_C4, 0xC4);
MENU1_AT(Work38EA8, field_464, 0x464);
MENU1_AT(Work38EA8, field_D54, 0xD54);
MENU1_AT(Work38EA8, field_D58, 0xD58);
MENU1_AT(Work38EA8, field_D5C, 0xD5C);
MENU1_AT(Work38EA8, field_D60, 0xD60);
MENU1_AT(Work38EA8, field_D64, 0xD64);
MENU1_AT(Work38EA8, field_D84, 0xD84);
MENU1_AT(Work38EA8, field_D88, 0xD88);
MENU1_AT(Work38EA8, field_D8C, 0xD8C);
MENU1_AT(Work38EA8, field_DA8, 0xDA8);
MENU1_AT(Work38EA8, field_DAC, 0xDAC);
MENU1_AT(Work38EA8, field_DB0, 0xDB0);
MENU1_AT(Work38EA8, field_DB5, 0xDB5);
MENU1_AT(Work38EA8, field_DB6, 0xDB6);

/* vtable D_08A38EA8 (0x08A35A98..0x08A37988). */
typedef struct Screen38EA8 {
    void *vtable;
    char pad_04[0xF60];
    Work38EA8 *work;
} Screen38EA8;
MENU1_AT(Screen38EA8, vtable, 0x0);
MENU1_AT(Screen38EA8, work, 0xF64);

typedef struct Obj39330 {
    char pad_00[0x4C];
    void *field_4C[0x20];
} Obj39330;
MENU1_AT(Obj39330, field_4C, 0x4C);

/* Work block of Screen392E8 (+0x1274). */
typedef struct Work392E8 {
    char pad_00[0x8];
    Obj39330 *field_08;
    MenuModel **handles;
} Work392E8;
MENU1_AT(Work392E8, field_08, 0x8);
MENU1_AT(Work392E8, handles, 0xC);

/* Screen of 0x08A392E8..0x08A39504 (vtable not seen here). */
typedef struct Screen392E8 {
    void *vtable;
    char pad_04[0x1270];
    Work392E8 *work;
} Screen392E8;
MENU1_AT(Screen392E8, vtable, 0x0);
MENU1_AT(Screen392E8, work, 0x1274);

/* ------------------------------------------------------------ Screen35A00 (D_08A35A00) */

/* Work block of Screen35A00 (+0x48). */
typedef struct Work35A00 {
    char pad_00[0x20];
    void *field_20;
    char pad_24[0x4];
    int field_28;
    char pad_2C[0x148];
    unsigned int field_174;
    char pad_178[0x20];
    void *field_198;
    char pad_19C[0x7];
    unsigned char field_1A3;
} Work35A00;
MENU1_AT(Work35A00, field_20, 0x20);
MENU1_AT(Work35A00, field_28, 0x28);
MENU1_AT(Work35A00, field_174, 0x174);
MENU1_AT(Work35A00, field_198, 0x198);
MENU1_AT(Work35A00, field_1A3, 0x1A3);

/* Screen in D_08A35A00 (0x08A31004..0x08A3246C); constructor func_08A323E4. */
typedef struct Screen35A00 {
    void *field_00; /* 0x4000-byte block handed to the task; freed from heap */
    MenuTask task;
    void *heap;
    char pad_3C[0x8];
    int field_44; /* read by func_08A3200C */
    Work35A00 *work;
} Screen35A00;
MENU1_AT(Screen35A00, field_00, 0x0);
MENU1_AT(Screen35A00, task, 0x4);
MENU1_AT(Screen35A00, heap, 0x38);
MENU1_AT(Screen35A00, field_44, 0x44);
MENU1_AT(Screen35A00, work, 0x48);

extern Screen35A00 *D_08A35A00;

/* ------------------------------------------------------------ 0x08A3A4F0..0x08A3A864 */

/* 0x90-byte element on the base built by func_088698FC. */
typedef struct Elem3A4F0 {
    char pad_00[0x4];
    int field_04;
    char pad_08[0x84];
    int state;
} Elem3A4F0;
MENU1_SIZE(Elem3A4F0, 0x90);
MENU1_AT(Elem3A4F0, field_04, 0x4);
MENU1_AT(Elem3A4F0, state, 0x8C);

/* Constructor func_08A3A864. */
typedef struct Obj3A864 {
    char pad_00[0x4];
    Elem3A4F0 field_04[16];
} Obj3A864;
MENU1_AT(Obj3A864, field_04, 0x4);

/* ------------------------------------------------------------ main module */

void *func_08864FF0(MenuTask *task, int arg1, int id, char arg3);
void func_0886503C(MenuTask *task, MenuCallback enter, MenuCallback update, MenuCallback leave);
void func_0886504C(MenuTask *task, void *block, unsigned int size);
void func_088650A4(MenuTask *task);
void func_08865158(MenuTask *task);
void func_0885C488(void *heap);                                    /* destroy a heap */
void func_0885C538(void *heap, void *block, int arg);             /* free a block */
void func_0888208C(void *object);
void *func_08935828(void *dst, char value, int size);             /* memset */
void func_08993560(void *array, int count, int size, void *ctor); /* build each element */
void func_08992F04(void *object);                                  /* operator delete */

/* layout scripts and model handles */
void func_088D3F00(void *layout, void *handles, const short *script);
void func_088D3CF0(void *layout, void *handles, const unsigned short *script);
void func_088D3EA0(void *layout, void *handles, const unsigned short *script);
void func_088D3C24(void *layout, void *handles, const short *script, int count);
void func_088D3B88(void *layout, void *handles, const short *script, int arg, int count);
void func_088D44EC(void *handles, void *texts, const unsigned short *script);
void func_088D428C(void *handles, void *texts, const unsigned short *script, char arg);
void func_088D3F7C(void *messages, void *texts, const unsigned short *script, int arg);
void func_088D3AE8(void *handles, int x, int y, int index);
void func_088D3B5C(void *handles, int count);
void func_088D38F0(void *layout);
void func_088D3910(char arg1, void *layout, const short *script, int arg2, char arg3);
void func_088D9F5C(void *model, int motion, char arg);
void func_088D9CD8(void *model);
void func_088D9CFC(void *model);
void func_088D9A94(void *model);
void func_088D9B48(void *model, unsigned int color);
void func_088D9B84(void *model, const float *matrix);
void func_088DA0A8(void *model, int index, float *matrix, unsigned int *color);
void *func_088DAA68(int id);
void func_088DB10C(void *handle);

/* texts and messages */
void *func_0887B3EC(void *messages, int kind, int arg1, int arg2, int arg3, int arg4);
char *func_0887B500(void *messages, int kind, int id);
void func_0887B778(void *text, const float *matrix);
void func_0887B820(void *text, unsigned int color);
void func_0887B828(void *text, const unsigned int *color);
void func_0887B944(void *text);
void func_0887BA5C(void *text);
void *func_0887E534(void *text);
void func_0887E77C(void *owner, void *object);
void func_088F5FC0(void *messages, int id);
int func_088F5FD4(void);

/* MenuPrompt, MenuCursor */
void func_088D4E00(int value, MenuPrompt *prompt);
void func_088D4EC4(MenuPrompt *prompt);
void func_088D4F9C(MenuPrompt *prompt);
int func_088D4FBC(MenuPrompt *prompt);
void func_088D46E4(MenuCursor *cursor, int arg1, int arg2, int last, int arg4, int arg5);
int func_088D484C(MenuCursor *cursor, int value, char arg);
unsigned char func_088D4928(MenuCursor *cursor, int index);
void func_088D4B74(MenuCursor *cursor, char arg);
int func_088D4BF4(unsigned int value);
int func_088D5AC8(void *object, int arg1, unsigned int arg2, unsigned int arg3);

/* other main-module objects */
int func_088AC9A8(Global9D712C *object);
int func_088AC8C8(Global9D712C *object, char value);
Record8CB164 *func_088CB164(void *table, int id);
char func_088CBAD0(Record8CB164 *record);
int func_088705F0(void *object);
unsigned char func_0887066C(void *object);
int func_088A24D4(int arg1, int id, int arg2, int arg3);
int func_088FDC98(void *object);
void func_088FCA30(void *object);
void func_088FCAB8(void *object);
unsigned char func_088FCAE8(void *object);
void func_088FCB8C(void *object);
void func_088FCBBC(void *object);
void func_088FD928(void *object, void *heap, int arg1, char arg2, const short *script, int id,
                   void *messages, int arg3, void *entry, char arg4, char arg5, int arg6);
void func_0889F234(void *object, int id);
int func_088B1194(void *object, int kind, MenuCallback2 callback, int arg1, int arg2);
int func_088B1354(void *object, int kind, MenuCallback2 callback, int arg1, int arg2);
int func_088B1230(void *object, char *text, int arg1, int arg2, int arg3, char *arg4);
int func_088F7988(void *object);
int func_088F7B4C(void *object);
int func_088F7B68(void *object);
int func_088F7B84(void *object);
int func_08900BE4(void *object);
void func_08900C04(void *object, int arg1, int arg2, int arg3);
void func_08885528(void *object);
int func_088AF29C(void *object);
int func_088AF2AC(void *object);
int func_08929CCC(void *rows, int count, void *messages, int id, int *out);
void func_088CAF90(void *object);
int func_088CB054(void *object);
unsigned char func_08908D30(void *object);
void func_08841ED8(void);
void func_088CB9C8(void);
void func_0885A31C(void *object);
void func_0885A398(void *object);
void func_0885A41C(void *object);
void func_088A5E6C(void *object);
void func_088A952C(void *object);
short *func_088470DC(int table, int id, int arg);
void func_0893476C(void *out, int arg1, int arg2);
int func_0889F704(void *object);
void func_088A3D14(void *object, int id);
int func_088A0DFC(void *object, int id);
void *func_088698FC(void *self);
void func_08869908(void *self, unsigned int mode);
int func_08869954(void *self, int value);

/* other overlays' singletons */
void func_08AF32D8(void *object, int arg1, int arg2, int arg3, int arg4);
void func_08AF510C(void *object, void *block);
void func_08AFEBD4(void *object, void *block);
void func_08AF9880(void *object, void *block);
void func_08AF9934(void *object, void *block);
int func_08AF99CC(void *object, void *block);
void func_08AC47C4(void *object);
void func_08AC481C(void *object, int arg1, int arg2, char arg3, int arg4, int arg5);
void func_08AB8080(void *object);
void func_08AB80E4(void *object, int arg1, int arg2, char arg3, char arg4, int arg5, int arg6);
void func_08B58B64(void *object, void *block);

/* ------------------------------------------------------------ ScreenE75C4 */

int func_089E2E84(ScreenE75C4 *self, int arg);
void func_089E3DB8(ScreenE75C4 *self);
void func_089E0C68(ScreenE75C4 *self);
void func_089E0CEC(ScreenE75C4 *self);
void func_089E1BB0(ScreenE75C4 *self);
int func_089E34F8(ScreenE75C4 *self, void *layout, void *handles, void *texts);
int func_089E3768(ScreenE75C4 *self, void *layout, void *handles, void *texts);
void func_089E1E0C(ScreenE75C4 *self);
void func_089E1E5C(ScreenE75C4 *self);
void func_089E30B8(ScreenE75C4 *self);
void func_089E2D4C(ScreenE75C4 *self, int arg1, int arg2);
void func_08A29D0C(ScreenE75C8 *self);
void func_08A29D64(ScreenE75C8 *self, int arg1, int arg2);

/* ------------------------------------------------------------ ScreenE9D38 */

void func_089E7DF0(ScreenE9D38 *self, unsigned int value, unsigned char keep);
void func_089E7E7C(ScreenE9D38 *self, int value);
void func_089E8E78(ScreenE9D38 *self);
void func_089E8E90(ScreenE9D38 *self);
void func_089E8F98(ScreenE9D38 *self);
void func_089E8FC8(ScreenE9D38 *self);
void func_089E9288(ScreenE9D38 *self);
void func_089E92B8(ScreenE9D38 *self);
void func_089E771C(ScreenE9D38 *self);
void func_089E78AC(ScreenE9D38 *self);
void func_089E797C(ScreenE9D38 *self);
void func_089E93A0(void);
void func_089E93C0(void);
void func_089EA0F8(void *object);
void func_089EA664(void *object);
void func_089EA6E8(void *object);
void func_089EA804(void *object);

/* ------------------------------------------------------------ small objects */

void func_089EACFC(void *self, unsigned int flags);                /* base destructor */
void *func_089EDDD8(void *self);
void func_089EE700(void);
void func_089EEC64(void);
void func_089ED8F8(void);

/* ------------------------------------------------------------ Screen0B6A0 */

void func_089FE6C0(Screen0B6A0 *self);
void func_089EEDD0(Screen0B6A0 *self);
void func_089EEFDC(Screen0B6A0 *self);
void *func_089EF1E8(Screen0B6A0 *self, int arg1, int arg2, int arg3, int arg4);
int func_089EFB50(Screen0B6A0 *self, unsigned int value, int index);
void func_089F1FEC(Screen0B6A0 *self, Entry84 *entry);
int func_089F2214(Screen0B6A0 *self, int value);
int func_089F2FF0(Screen0B6A0 *self);
void func_089F3498(Screen0B6A0 *self, char value);
int func_089F6184(Screen0B6A0 *self, int value);
void func_089F3790(Screen0B6A0 *self);
unsigned short func_089F0064(Screen0B6A0 *self);
unsigned short func_089F00C8(Screen0B6A0 *self, unsigned int index);
void func_089F3B70(Screen0B6A0 *self);
void func_089F3DF4(Screen0B6A0 *self);
void func_089FFFA8(Screen0B6A0 *self, int arg1, unsigned int value, char arg3, char arg4);
void func_089F3F68(Screen0B6A0 *self, int value);
void func_089F3FC8(Screen0B6A0 *self, int value, int arg2, int arg3, int arg4);
short func_089EF640(Screen0B6A0 *self, Block3158 *block);
short func_089EF700(Screen0B6A0 *self, Block3158 *block);
short func_089EF748(Screen0B6A0 *self, Block3158 *block);
void func_089F5964(Screen0B6A0 *self);
void func_089F5990(Screen0B6A0 *self, int index);
void func_089F77E0(Screen0B6A0 *self, int arg1, int arg2, char arg3);
void func_089F24D8(Screen0B6A0 *self);
void func_089F652C(Screen0B6A0 *self);
void func_089F6D58(Screen0B6A0 *self);
void func_089F7224(Screen0B6A0 *self, int value);
int func_089F72DC(Screen0B6A0 *self);
void func_089F74E4(Screen0B6A0 *self);
void func_08A0118C(Screen0B6A0 *self, Entry84 *entry, int arg1, int arg2, int arg3, int arg4, int arg5);
void func_089F309C(Screen0B6A0 *self);
void func_089F777C(Screen0B6A0 *self);
void func_089FC9FC(Screen0B6A0 *self);
void func_089FF848(Screen0B6A0 *self);
void func_089F2D48(Screen0B6A0 *self);
void func_089F63C8(Screen0B6A0 *self);
void func_089F6AF4(Screen0B6A0 *self);
void func_089F6E58(Screen0B6A0 *self);
void func_089F70F0(Screen0B6A0 *self);
void func_089F7B8C(Screen0B6A0 *self);
void func_089F7D8C(Screen0B6A0 *self);
void func_08A00D98(Screen0B6A0 *self);
void func_089F30E8(Screen0B6A0 *self);
void func_089F8B1C(Screen0B6A0 *self);
void func_089F8CC8(Screen0B6A0 *self);
void func_08A03D48(Screen0B6A0 *self);
void func_08A04164(Screen0B6A0 *self);
void func_089F91F0(Screen0B6A0 *self);
void func_08A043E4(Screen0B6A0 *self);
void func_089F9388(Screen0B6A0 *self);
void func_089F9674(Screen0B6A0 *self);
void func_089F9954(Screen0B6A0 *self);
void func_089F9C08(Screen0B6A0 *self);
void func_08A05138(Screen0B6A0 *self);
void func_08A04AF8(Screen0B6A0 *self);
void func_089F8C24(Screen0B6A0 *self);
void func_089F8E18(Screen0B6A0 *self);
void func_089F8F80(Screen0B6A0 *self);
void func_089F914C(Screen0B6A0 *self);
void func_089F9264(Screen0B6A0 *self);
void func_089F92F0(Screen0B6A0 *self);
void func_089F9444(Screen0B6A0 *self);
void func_089F987C(Screen0B6A0 *self);
void func_089F99CC(Screen0B6A0 *self);
void func_089FB4A4(Screen0B6A0 *self, void *layout, void *handles, void *texts);
void func_089FCFE0(Screen0B6A0 *self);
void func_089FAE4C(Screen0B6A0 *self, int value);
int func_089FA3E8(Screen0B6A0 *self, int value);
void func_089FB6FC(Screen0B6A0 *self, int arg1, int arg2);
int func_089FE598(Screen0B6A0 *self);
int func_089FE5A4(Screen0B6A0 *self);
int func_089FE5B0(Screen0B6A0 *self);

/* ------------------------------------------------------------ screens on the vtable base */

void func_08A2405C(void *self, unsigned int flags);                /* base destructor */
void func_08A240C8(void *object);                                  /* free from the root's heap */
void func_08A0C0EC(Screen11510 *self);
void func_08A0E070(Screen11510 *self);
void func_08A0C990(Screen11510 *self);
void func_08A0CBC0(Screen11510 *self);
void func_08A0CD80(Screen11510 *self);
void func_08A0D0F0(Screen11510 *self);
void func_08A0D4A4(Screen11510 *self);

void func_08A12FAC(Screen15A88 *self, char value);
void func_08A11F10(Screen15A88 *self);
void func_08A12034(Screen15A88 *self);
void func_08A12EB4(Screen15A88 *self);
void func_08A12244(Screen15A88 *self, char value);
void func_08A124FC(Screen15A88 *self, int value);
void func_08A13988(Screen15A88 *self, char arg1, int arg2);
void func_08A155C0(Screen15A88 *self);
void func_08A12C44(Screen15A88 *self);
void func_08A12C78(Screen15A88 *self);
void func_08A12CAC(Screen15A88 *self);
void func_08A12CD4(Screen15A88 *self);
void func_08A12CE0(Screen15A88 *self);
void func_08A12D10(Screen15A88 *self, int value);
void func_08A28EC0(Board *self);
int func_08A28C24(Board *self);
void func_08A292A0(Board *self);
void func_08A292BC(Board *self);
void func_08A292EC(Board *self);
void func_08A2931C(Board *self);
void func_08A2934C(Board *self);
void func_08A2937C(Board *self);
void func_08A293AC(Board *self);

void func_08A1609C(Screen181E0 *self);
void func_08A16384(Screen181E0 *self);

void func_08A199E0(Screen1AB08 *self, int index);
void func_08A18C34(Screen1AB08 *self, int index);
void func_08A18D28(Screen1AB08 *self);
void func_08A18E08(Screen1AB08 *self);
void func_08A18E9C(Screen1AB08 *self);

void func_08A24C80(Screen28560 *self);
void func_08A244A4(Screen28560 *self);
void func_08A24A60(Screen28560 *self);
void func_08A243C4(Screen28560 *self);
void func_08A24304(Screen28560 *self);
void func_08A24524(Screen28560 *self);
void func_08A247AC(Screen28560 *self);
unsigned int func_08A255AC(Screen28560 *self, unsigned int value, unsigned char arg);

/* ------------------------------------------------------------ menu root */

void func_08A1D4DC(MenuRoot *self, short value);
void func_08A205E4(MenuRoot *self);
void func_08A22514(MenuRoot *self, int mode, void *layout, void *handles, void *texts, int arg1,
                   unsigned int arg2, char arg3);
void func_08A1E894(MenuRoot *self, int mode, void *layout, void *handles, void *texts, char arg);
void func_08A21C88(MenuRoot *self, unsigned int value, int arg1, int arg2, void *layout, void *handles,
                   void *texts, void *block, unsigned char arg3, unsigned int arg4, unsigned int arg5,
                   unsigned int arg6);
void func_08A1E6B4(MenuRoot *self, int mode, void *layout, void *handles, void *texts, void *block);
void func_08A1C07C(MenuRoot *self);
void func_08A1D8C0(MenuRoot *self);
unsigned char func_08A1F6D8(MenuRoot *self);
void func_08A20AE8(MenuRoot *self);
void func_08A20F90(MenuRoot *self);
void func_08A1B1F0(MenuRoot *self);
void func_08A1C3FC(MenuRoot *self);
void func_08A1C480(MenuRoot *self);
void func_08A1C504(MenuRoot *self);
void func_08A1C5C4(MenuRoot *self);
void func_08A1C5F0(MenuRoot *self);
void func_08A1C628(MenuRoot *self);
void func_08A1C654(MenuRoot *self);
void func_08A1C7E8(MenuRoot *self);
void func_08A1C910(MenuRoot *self);
void func_08A1C88C(MenuRoot *self);
void func_08A1C8DC(MenuRoot *self);
void func_08A1CA60(MenuRoot *self, unsigned int value);
void func_08A1D264(MenuRoot *self, int arg1, int arg2);
void func_08A1D280(MenuRoot *self, int arg1, int arg2);
unsigned char func_08A1D8E4(MenuRoot *self);
void func_08A1EA3C(MenuRoot *self, MenuModel **handles);
void func_08A1E188(MenuRoot *self);
void func_08A1F178(MenuRoot *self);
void func_08A1F268(MenuRoot *self);
unsigned char func_08A1F39C(void);
void func_08A1F528(void);
int func_08A1F108(void);
int func_08A1F1DC(void);
int func_08A1BA6C(MenuRoot *self, int value);
void func_08A1E2B4(MenuRoot *self);
void func_08A1E410(MenuRoot *self);
void func_08A20434(MenuRoot *self);
void func_08A2044C(int arg1, int arg2);
void func_08A20688(int arg1, int arg2);
void func_08A206B0(void);
void func_08A206D0(void);

/* ------------------------------------------------------------ ScreenE75C8 */

void func_08A2BE1C(ScreenE75C8 *self);
void func_08A29BF8(ScreenE75C8 *self);
void func_08A29C88(ScreenE75C8 *self);
void func_08A2BC64(void);
void func_08A2BC84(void);

/* ------------------------------------------------------------ second base, Screen35550, Screen38EA8 */

void func_08A408B4(void *self, unsigned int flags);                /* base destructor */
void func_08A40920(void *object);
void func_08A41454(void *root, void *rows);
void *func_08A412A8(void *root, int id, int arg1, int arg2);
void func_08A419A4(void *root);
void func_08A2F134(Screen35550 *self);
void func_08A2EB10(Screen35550 *self);
void func_08A37AC8(Screen38EA8 *self, int index);
int func_08A3C0C4(int id);
void func_08A35EE8(Screen38EA8 *self, int value);
void func_08A37E10(Screen38EA8 *self, char value);
void func_08A371E4(Screen38EA8 *self);
void func_08A382BC(Screen38EA8 *self, int value);
int func_08A37024(Screen38EA8 *self);
void func_08A39184(Screen392E8 *self, int index);

/* ------------------------------------------------------------ Screen35A00 */

void func_08A31438(Screen35A00 *self);
void func_08A3210C(Screen35A00 *self);
int func_08A3200C(Screen35A00 *self);
void func_08A31688(Screen35A00 *self);
void func_08A30DB8(Screen35A00 *self);
void func_08A31004(Screen35A00 *self);
void func_08A310A8(Screen35A00 *self);
void func_08A310E0(Screen35A00 *self);
void func_08A32364(void);
void func_08A32384(void);
void func_08A323A4(void);

/* ------------------------------------------------------------ 0x08A3A4F0.. */

Elem3A4F0 *func_08A3A4F0(Elem3A4F0 *self);
void func_08A3A8A8(Obj3A864 *self);


#ifdef __cplusplus
}
#endif

#endif
