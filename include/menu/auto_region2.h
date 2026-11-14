#ifndef DISSIDIA_MENU_AUTO_REGION2_H
#define DISSIDIA_MENU_AUTO_REGION2_H

/* Classes behind the automatically matched functions in menu
 * 0x08A3CD08..0x08A70904 (readability pass, menu region 2).
 *
 * The range continues the menu screens of auto_region1.h (whose types and
 * prototypes it reuses) and then holds the save/load screens: a screen in
 * D_08A4FFF4 whose work block points at nine panels, each a class on the
 * panel base built by func_08A51DD0 (vtable at +0x24, Panel), and the
 * widgets, items, parts and pages those panels embed (each family has its
 * own base with the vtable at 0, +0x130, +0x8 or +0xC). Structs are named
 * after the singleton pointer or the vtable of their class, as in region 1;
 * without either, after their constructor or first function here.
 *
 * As in region 1, "handles" are model handles driven by the script players
 * (func_088D3F00 plays a layout script over them, func_088D44EC hands them
 * texts), "texts" are the objects func_0887B3EC / func_0887D868 make (freed
 * with func_0887BA5C). Panels and widgets keep the two scripts inline:
 * script for func_088D3F00, text_script for func_088D44EC. Array lengths
 * follow the loops that walk them; a length nothing walks is "not known".
 *
 * auto_region1.h names only MenuCursor.flags and MenuModel.field_0C/20;
 * MenuCursorView and MenuModelView below give the fields this region also
 * reads: MenuCursorView is the member type wherever the index is read (a
 * cast of a member's address does not fold the offset into the load), and
 * MenuModelView is reached by casting a handle.
 *
 * Fields whose use is not plain keep their offset as their name. */

#include "menu/auto_region1.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MENU2_OFFSET(type, field) ((unsigned int)&((type *)0)->field)
#define MENU2_ASSERT(name, cond) typedef char menu2_assert_##name[(cond) ? 1 : -1]
#define MENU2_SIZE(type, size) MENU2_ASSERT(size_##type, sizeof(type) == (size))
#define MENU2_AT(type, field, offset) \
    MENU2_ASSERT(at_##type##_##field, MENU2_OFFSET(type, field) == (offset))

/* ------------------------------------------------------------ views of region-1 types */

/* MenuCursor with the selected index (the second argument of func_088D46E4).
 * Declared as the member type where the index is read: a cast of the
 * member's address does not fold the offset into the load.
 */
typedef struct MenuCursorView {
    unsigned int flags;
    int index;
    char pad_08[0x24];
} MenuCursorView;
MENU2_SIZE(MenuCursorView, 0x2C);
MENU2_AT(MenuCursorView, flags, 0x0);
MENU2_AT(MenuCursorView, index, 0x4);

/* MenuModel with the field_0E that two functions here set. */
typedef struct MenuModelView {
    char pad_00[0xC];
    short field_0C;
    short field_0E;
    char pad_10[0x10];
    unsigned short field_20;
} MenuModelView;
MENU2_AT(MenuModelView, field_0C, 0xC);
MENU2_AT(MenuModelView, field_0E, 0xE);
MENU2_AT(MenuModelView, field_20, 0x20);

/* Second argument of func_088D4A68 (a list scroll: field_04 the first row shown). */
typedef struct ScrollState {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
} ScrollState;
MENU2_SIZE(ScrollState, 0x14);
MENU2_AT(ScrollState, field_00, 0x0);
MENU2_AT(ScrollState, field_04, 0x4);
MENU2_AT(ScrollState, field_08, 0x8);
MENU2_AT(ScrollState, field_0C, 0xC);
MENU2_AT(ScrollState, field_10, 0x10);

/* ------------------------------------------------------------ globals */

typedef struct Global9D726C {
    char pad_00[0xA8];
    unsigned int field_A8;
} Global9D726C;
MENU2_AT(Global9D726C, field_A8, 0xA8);

typedef struct Global9D7E88 {
    char pad_00[0x184D];
    unsigned char field_184D; /* set while a system dialog runs */
} Global9D7E88;
MENU2_AT(Global9D7E88, field_184D, 0x184D);

typedef struct Obj2C90 {
    char pad_00[0x2C90];
    int field_2C90;
} Obj2C90;
MENU2_AT(Obj2C90, field_2C90, 0x2C90);

typedef struct Global9D8530 {
    char pad_00[0x8];
    Obj2C90 *field_08;
} Global9D8530;
MENU2_AT(Global9D8530, field_08, 0x8);

typedef struct Data59230 {
    char pad_00[0xC];
    int field_0C;
} Data59230;
MENU2_AT(Data59230, field_0C, 0xC);

/* What D_08B59230 points to (declared void * in auto_region1.h). */
typedef struct Obj59230 {
    Data59230 *field_00;
} Obj59230;
MENU2_AT(Obj59230, field_00, 0x0);

extern Global9D726C *D_089D726C;
extern Global9D7E88 *D_089D7E88;
extern Global9D8530 *D_089D8530;
extern int D_09B6DD00;

/* ------------------------------------------------------------ Screen40248 (0x08A3CD08..0x08A3E21C) */

/* Work block of Screen40248 (+0x2A14). */
typedef struct Work40248 {
    MenuLayout *layout;
    MenuModel **handles; /* 0x4C of them */
    char pad_08[0x4];
    void *texts[0x20];
    char pad_8C[0x2978];
    int field_2A04;
    int state;
} Work40248;
MENU2_AT(Work40248, layout, 0x0);
MENU2_AT(Work40248, handles, 0x4);
MENU2_AT(Work40248, texts, 0xC);
MENU2_AT(Work40248, field_2A04, 0x2A04);
MENU2_AT(Work40248, state, 0x2A08);

/* vtable D_08A40248, on the second screen base (destructor func_08A408B4). */
typedef struct Screen40248 {
    void *vtable;
    char pad_04[0x2A10];
    Work40248 *work;
} Screen40248;
MENU2_AT(Screen40248, vtable, 0x0);
MENU2_AT(Screen40248, work, 0x2A14);

/* ------------------------------------------------------------ MenuRoot2 (D_08A457EC, 0x08A408A0..0x08A43094) */

/* The second screen base (constructor func_08A408A0, destructor func_08A408B4). */
typedef struct ScreenBase2 {
    void *vtable;
} ScreenBase2;
MENU2_SIZE(ScreenBase2, 0x4);
MENU2_AT(ScreenBase2, vtable, 0x0);

typedef struct QueuePair {
    short field_00;
    short field_02;
} QueuePair;
MENU2_SIZE(QueuePair, 0x4);
MENU2_AT(QueuePair, field_00, 0x0);
MENU2_AT(QueuePair, field_02, 0x2);

/* Up to three pairs queued by func_08A41674 (cleared whole by func_08A4162C). */
typedef struct PairQueue {
    int count;
    int field_04;
    int field_08;
    QueuePair pairs[3];
} PairQueue;
MENU2_SIZE(PairQueue, 0x18);
MENU2_AT(PairQueue, count, 0x0);
MENU2_AT(PairQueue, field_04, 0x4);
MENU2_AT(PairQueue, field_08, 0x8);
MENU2_AT(PairQueue, pairs, 0xC);

/* Work block of the second menu root (+0x14C). */
typedef struct Root2Work {
    MenuLayout layout;
    MenuModel *handles[0x142];
    PairQueue queue;
    void *messages;
    void *texts[0x14];
    char pad_974[0xB8];
    MenuPrompt prompt;
    char pad_A58[0x4];
    unsigned int bits[1]; /* bit set (length not known) */
    char pad_A60[0x3D4];
    void *field_E34; /* freed from the heap */
    void *field_E38; /* freed from the heap */
    char pad_E3C[0x4];
    unsigned int flags;
    int state;
} Root2Work;
MENU2_AT(Root2Work, layout, 0x0);
MENU2_AT(Root2Work, handles, 0x400);
MENU2_AT(Root2Work, queue, 0x908);
MENU2_AT(Root2Work, messages, 0x920);
MENU2_AT(Root2Work, texts, 0x924);
MENU2_AT(Root2Work, prompt, 0xA2C);
MENU2_AT(Root2Work, bits, 0xA5C);
MENU2_AT(Root2Work, field_E34, 0xE34);
MENU2_AT(Root2Work, field_E38, 0xE38);
MENU2_AT(Root2Work, flags, 0xE40);
MENU2_AT(Root2Work, state, 0xE44);

/* The second menu root in D_08A457EC (declared void * in auto_region1.h). */
typedef struct MenuRoot2 {
    char pad_00[0x100];
    MenuTask task;
    void *heap; /* its screens are freed from it (func_08A40920) */
    void *field_138; /* released with func_0888208C */
    char field_13C[0x10]; /* set up by func_088D5318 */
    Root2Work *work;
} MenuRoot2;
MENU2_AT(MenuRoot2, task, 0x100);
MENU2_AT(MenuRoot2, heap, 0x134);
MENU2_AT(MenuRoot2, field_138, 0x138);
MENU2_AT(MenuRoot2, field_13C, 0x13C);
MENU2_AT(MenuRoot2, work, 0x14C);

/* ------------------------------------------------------------ Screen457F0 (D_08A457F0) */

typedef struct Work457F0 {
    char pad_00[0x408];
    int field_408;
} Work457F0;
MENU2_AT(Work457F0, field_408, 0x408);

/* Screen in D_08A457F0 (0x08A434E0..0x08A436C0); constructor func_08A436C0. */
typedef struct Screen457F0 {
    char pad_00[0x100];
    MenuTask task;
    void *heap;
    void *field_138; /* released with func_0888208C */
    Work457F0 *work;
} Screen457F0;
MENU2_AT(Screen457F0, task, 0x100);
MENU2_AT(Screen457F0, heap, 0x134);
MENU2_AT(Screen457F0, field_138, 0x138);
MENU2_AT(Screen457F0, work, 0x13C);

extern Screen457F0 *D_08A457F0;

/* ------------------------------------------------------------ Screen47D90, IdTable */

/* A row of four ids in an IdTable. */
typedef struct IdRow {
    short field_00;
    short field_02;
    short field_04;
    short field_06;
} IdRow;
MENU2_SIZE(IdRow, 0x8);
MENU2_AT(IdRow, field_00, 0x0);
MENU2_AT(IdRow, field_02, 0x2);
MENU2_AT(IdRow, field_04, 0x4);
MENU2_AT(IdRow, field_06, 0x6);

/* Rows of four ids (0x08A482A8..0x08A48608); the rows come from heap. */
typedef struct IdTable {
    IdRow *rows;
    int field_04;
    int field_08;
    int count;
    void *heap;
} IdTable;
MENU2_AT(IdTable, rows, 0x0);
MENU2_AT(IdTable, field_04, 0x4);
MENU2_AT(IdTable, field_08, 0x8);
MENU2_AT(IdTable, count, 0xC);
MENU2_AT(IdTable, heap, 0x10);

/* Work block of Screen47D90 (+0x418). */
typedef struct Work47D90 {
    char pad_00[0x4];
    MenuModel **handles;
    char pad_08[0x4];
    void *texts[4];
    char pad_1C[0x3C8];
    unsigned int flags;
    char pad_3E8[0x28];
    IdTable *table;
} Work47D90;
MENU2_AT(Work47D90, handles, 0x4);
MENU2_AT(Work47D90, texts, 0xC);
MENU2_AT(Work47D90, flags, 0x3E4);
MENU2_AT(Work47D90, table, 0x410);

/* vtable D_08A47D90, on the second screen base. */
typedef struct Screen47D90 {
    void *vtable;
    char pad_04[0x414];
    Work47D90 *work;
} Screen47D90;
MENU2_AT(Screen47D90, vtable, 0x0);
MENU2_AT(Screen47D90, work, 0x418);

/* ------------------------------------------------------------ BlockView (D_08A49748) */

/* Pointers into a data block, set by func_08A49634 (header, three record runs, a tail). */
typedef struct BlockView {
    void *header;
    void *field_04;
    void *field_08;
    void *field_0C;
    int *field_10;
} BlockView;
MENU2_SIZE(BlockView, 0x14);
MENU2_AT(BlockView, header, 0x0);
MENU2_AT(BlockView, field_04, 0x4);
MENU2_AT(BlockView, field_08, 0x8);
MENU2_AT(BlockView, field_0C, 0xC);
MENU2_AT(BlockView, field_10, 0x10);

extern BlockView *D_08A49748;

/* ------------------------------------------------------------ Screen4C0FC (D_08A4C0FC) */

/* Work block of Screen4C0FC (+0x14C). */
typedef struct Work4C0FC {
    char pad_00[0x4];
    void *field_04; /* freed from the heap */
    int field_08;
    char pad_0C[0x880];
    void *messages;
    char pad_890[0x353];
    char field_BE3;
    void *field_BE4; /* freed from the heap */
    void *field_BE8; /* released with func_0887E77C */
} Work4C0FC;
MENU2_AT(Work4C0FC, field_04, 0x4);
MENU2_AT(Work4C0FC, field_08, 0x8);
MENU2_AT(Work4C0FC, messages, 0x88C);
MENU2_AT(Work4C0FC, field_BE3, 0xBE3);
MENU2_AT(Work4C0FC, field_BE4, 0xBE4);
MENU2_AT(Work4C0FC, field_BE8, 0xBE8);

/* Screen in D_08A4C0FC (0x08A49CB0..0x08A4AC64); constructor func_08A4ABFC. */
typedef struct Screen4C0FC {
    char pad_00[0x100];
    MenuTask task;
    void *field_134; /* released with func_0888208C */
    void *heap;
    char pad_13C[0x10];
    Work4C0FC *work;
} Screen4C0FC;
MENU2_AT(Screen4C0FC, task, 0x100);
MENU2_AT(Screen4C0FC, field_134, 0x134);
MENU2_AT(Screen4C0FC, heap, 0x138);
MENU2_AT(Screen4C0FC, work, 0x14C);

extern Screen4C0FC *D_08A4C0FC;

/* ------------------------------------------------------------ panel base (func_08A51DD0) */

/* Panel base: constructor func_08A51DD0 (vtable D_08A522A0 at +0x24), reset
 * func_08A51E3C, destructor func_08A51E08. func_08A575A4 shows the first six
 * fields: three arrays the derived panel owns and their lengths.
 */
typedef struct Panel {
    void **field_00;
    MenuModel **handles;
    void **texts;
    int field_0C; /* length of field_00 */
    int handle_count;
    int text_count;
    int field_18;
    void *field_1C;
    void *field_20; /* released with func_088825D4 (field_20, field_1C) */
    void *vtable;
} Panel;
MENU2_SIZE(Panel, 0x28);
MENU2_AT(Panel, field_00, 0x0);
MENU2_AT(Panel, handles, 0x4);
MENU2_AT(Panel, texts, 0x8);
MENU2_AT(Panel, field_0C, 0xC);
MENU2_AT(Panel, handle_count, 0x10);
MENU2_AT(Panel, text_count, 0x14);
MENU2_AT(Panel, field_18, 0x18);
MENU2_AT(Panel, field_1C, 0x1C);
MENU2_AT(Panel, field_20, 0x20);
MENU2_AT(Panel, vtable, 0x24);

/* ------------------------------------------------------------ actions (func_08A51D38) */

/* vtable at 0 (D_08A51D98); func_08A51D70 is its empty initialiser. */
typedef struct Action {
    void *vtable;
} Action;
MENU2_SIZE(Action, 0x4);
MENU2_AT(Action, vtable, 0x0);

/* Initialised by func_08A5C02C; hands field_08/field_04 to the save screen. */
typedef struct Action5C02C {
    Action base;
    int field_04;
    int field_08;
} Action5C02C;
MENU2_SIZE(Action5C02C, 0xC);
MENU2_AT(Action5C02C, base, 0x0);
MENU2_AT(Action5C02C, field_04, 0x4);
MENU2_AT(Action5C02C, field_08, 0x8);

/* Initialised by func_08A5C224. */
typedef struct Action5C240 {
    Action base;
    unsigned char field_04;
    char pad_05[0x3];
    unsigned int field_08;
} Action5C240;
MENU2_SIZE(Action5C240, 0xC);
MENU2_AT(Action5C240, base, 0x0);
MENU2_AT(Action5C240, field_04, 0x4);
MENU2_AT(Action5C240, field_08, 0x8);

/* Initialised by func_08A5C3CC. */
typedef struct Action5C3CC {
    Action base;
    int field_04;
} Action5C3CC;
MENU2_SIZE(Action5C3CC, 0x8);
MENU2_AT(Action5C3CC, base, 0x0);
MENU2_AT(Action5C3CC, field_04, 0x4);

/* ------------------------------------------------------------ widgets (vtable at 0, base func_08A52300) */

/* Widget base: vtable D_08A52318 (func_08A52300). */
typedef struct Widget {
    void *vtable;
} Widget;
MENU2_SIZE(Widget, 0x4);
MENU2_AT(Widget, vtable, 0x0);

/* Element built by func_08A5C390. */
typedef struct Elem5C390 {
    int field_00;
    int field_04;
} Elem5C390;
MENU2_SIZE(Elem5C390, 0x8);
MENU2_AT(Elem5C390, field_00, 0x0);
MENU2_AT(Elem5C390, field_04, 0x4);

/* Element built by func_08A5C1E8. */
typedef struct Elem5C1E8 {
    int field_00;
    char pad_04[0x8];
} Elem5C1E8;
MENU2_SIZE(Elem5C1E8, 0xC);
MENU2_AT(Elem5C1E8, field_00, 0x0);

/* Element built by func_08A5BFF0. */
typedef struct Elem5BFF0 {
    int field_00;
    char pad_04[0x8];
} Elem5BFF0;
MENU2_SIZE(Elem5BFF0, 0xC);
MENU2_AT(Elem5BFF0, field_00, 0x0);

/* vtable D_08A52C98; constructor func_08A52390. */
typedef struct Widget52C98 {
    Widget base;
    char pad_04[0x4];
    MenuModel *handles[5];
    char pad_1C[0x4];
    short script[0x18];
    void *texts[2];
    unsigned short text_script[0xC];
    int field_70;
    Elem5C390 field_74[2];
    char pad_84[0xC];
} Widget52C98;
MENU2_SIZE(Widget52C98, 0x90);
MENU2_AT(Widget52C98, base, 0x0);
MENU2_AT(Widget52C98, handles, 0x8);
MENU2_AT(Widget52C98, script, 0x20);
MENU2_AT(Widget52C98, texts, 0x50);
MENU2_AT(Widget52C98, text_script, 0x58);
MENU2_AT(Widget52C98, field_70, 0x70);
MENU2_AT(Widget52C98, field_74, 0x74);

/* 0x88-byte element built by func_08A52D60 (set up by func_08A52DDC). */
typedef struct Elem52D60 {
    int state;
    MenuModel *handles[7];
    char pad_20[0x4];
    short script[0x20];
    void *texts[4];
    int field_74;
    int field_78;
    int field_7C;
    unsigned char field_80;
    char pad_81[0x7];
} Elem52D60;
MENU2_SIZE(Elem52D60, 0x88);
MENU2_AT(Elem52D60, state, 0x0);
MENU2_AT(Elem52D60, handles, 0x4);
MENU2_AT(Elem52D60, script, 0x24);
MENU2_AT(Elem52D60, texts, 0x64);
MENU2_AT(Elem52D60, field_74, 0x74);
MENU2_AT(Elem52D60, field_78, 0x78);
MENU2_AT(Elem52D60, field_7C, 0x7C);
MENU2_AT(Elem52D60, field_80, 0x80);

/* vtable D_08A54438; constructor func_08A531E0. */
typedef struct Widget54438 {
    Widget base;
    char pad_04[0x8];
    MenuModel *handles[13];
    char pad_40[0x4];
    short script[0x38];
    void *texts[3];
    unsigned short text_script[0x10];
    Elem52D60 field_E0[2];
    char pad_1F0[0x4];
    Elem5C390 field_1F4[2];
    Elem5C1E8 field_204[2];
    Elem5BFF0 field_21C[1];
    char pad_228[0x18];
} Widget54438;
MENU2_SIZE(Widget54438, 0x240);
MENU2_AT(Widget54438, base, 0x0);
MENU2_AT(Widget54438, handles, 0xC);
MENU2_AT(Widget54438, script, 0x44);
MENU2_AT(Widget54438, texts, 0xB4);
MENU2_AT(Widget54438, text_script, 0xC0);
MENU2_AT(Widget54438, field_E0, 0xE0);
MENU2_AT(Widget54438, field_1F4, 0x1F4);
MENU2_AT(Widget54438, field_204, 0x204);
MENU2_AT(Widget54438, field_21C, 0x21C);

/* vtable D_08A54E98; constructor func_08A546F0. */
typedef struct Widget54E98 {
    Widget base;
    char pad_04[0x4];
    MenuModel *handles[3];
    char pad_14[0x4];
    short script[0x10];
    void *texts[1];
    unsigned short text_script[8];
    int field_4C;
    Elem5C390 field_50[1];
    char pad_58[0x8];
} Widget54E98;
MENU2_SIZE(Widget54E98, 0x60);
MENU2_AT(Widget54E98, base, 0x0);
MENU2_AT(Widget54E98, handles, 0x8);
MENU2_AT(Widget54E98, script, 0x18);
MENU2_AT(Widget54E98, texts, 0x38);
MENU2_AT(Widget54E98, text_script, 0x3C);
MENU2_AT(Widget54E98, field_4C, 0x4C);
MENU2_AT(Widget54E98, field_50, 0x50);

/* ------------------------------------------------------------ items (vtable at +0x130, base func_08A54F38) */

/* Item base: constructor func_08A54F38 (vtable D_08A58360 at +0x130),
 * destructor func_08A54F70.
 */
typedef struct Item {
    MenuModel *handles[6];
    char pad_18[0x4];
    short script[0x20];
    void *texts[12];
    unsigned short text_script[0x34];
    int field_F4;
    char field_F8;
    char pad_F9[0x7];
    MenuCursor cursor;
    int count; /* entries the cursor moves over */
    void *vtable;
} Item;
MENU2_SIZE(Item, 0x134);
MENU2_AT(Item, handles, 0x0);
MENU2_AT(Item, script, 0x1C);
MENU2_AT(Item, texts, 0x5C);
MENU2_AT(Item, text_script, 0x8C);
MENU2_AT(Item, field_F4, 0xF4);
MENU2_AT(Item, field_F8, 0xF8);
MENU2_AT(Item, cursor, 0x100);
MENU2_AT(Item, count, 0x12C);
MENU2_AT(Item, vtable, 0x130);

/* vtable D_08A58380. */
typedef struct Item58380 {
    Item base;
    int field_134[0x80];
    int field_334[0x80];
    int field_534;
} Item58380;
MENU2_SIZE(Item58380, 0x538);
MENU2_AT(Item58380, base, 0x0);
MENU2_AT(Item58380, field_134, 0x134);
MENU2_AT(Item58380, field_334, 0x334);
MENU2_AT(Item58380, field_534, 0x534);

/* vtable D_08A583A0. */
typedef struct Item583A0 {
    Item base;
    int field_134;
    char pad_138[0x200];
    int field_338;
    int field_33C;
    int field_340;
    int field_344;
    int field_348;
} Item583A0;
MENU2_SIZE(Item583A0, 0x34C);
MENU2_AT(Item583A0, base, 0x0);
MENU2_AT(Item583A0, field_134, 0x134);
MENU2_AT(Item583A0, field_338, 0x338);
MENU2_AT(Item583A0, field_33C, 0x33C);
MENU2_AT(Item583A0, field_340, 0x340);
MENU2_AT(Item583A0, field_344, 0x344);
MENU2_AT(Item583A0, field_348, 0x348);

/* vtable D_08A583C0. */
typedef struct Item583C0 {
    Item base;
    int field_134;
    char pad_138[0x100];
    char field_238;
    char field_239;
    char field_23A;
    char pad_23B[0x1];
} Item583C0;
MENU2_SIZE(Item583C0, 0x23C);
MENU2_AT(Item583C0, base, 0x0);
MENU2_AT(Item583C0, field_134, 0x134);
MENU2_AT(Item583C0, field_238, 0x238);
MENU2_AT(Item583C0, field_239, 0x239);
MENU2_AT(Item583C0, field_23A, 0x23A);

/* vtable D_08A583E0. */
typedef struct Item583E0 {
    Item base;
    char field_134[0x20]; /* handed to func_089292C0 */
    int field_154;
} Item583E0;
MENU2_SIZE(Item583E0, 0x158);
MENU2_AT(Item583E0, base, 0x0);
MENU2_AT(Item583E0, field_134, 0x134);
MENU2_AT(Item583E0, field_154, 0x154);

/* vtable D_08A58400 (size: up to the next field of Panel58420). */
typedef struct Item58400 {
    Item base;
    char pad_134[0x64];
} Item58400;
MENU2_SIZE(Item58400, 0x198);
MENU2_AT(Item58400, base, 0x0);

/* ------------------------------------------------------------ Panel58420 and its record */

/* 0x164-byte record: Panel6C638 lists them, Panel58420 shows one
 * (through field_111C).
 */
typedef struct Rec164 {
    short field_00;
    short field_02;
    short field_04;
    short field_06;
    char field_08;
    char field_09;
    char field_0A;
    char pad_0B[0x1];
    char field_0C;
    char field_0D;
    char field_0E;
    char pad_0F[0x1];
    char field_10[4]; /* length not known */
    char pad_14[0x150];
} Rec164;
MENU2_SIZE(Rec164, 0x164);
MENU2_AT(Rec164, field_00, 0x0);
MENU2_AT(Rec164, field_02, 0x2);
MENU2_AT(Rec164, field_04, 0x4);
MENU2_AT(Rec164, field_06, 0x6);
MENU2_AT(Rec164, field_08, 0x8);
MENU2_AT(Rec164, field_09, 0x9);
MENU2_AT(Rec164, field_0A, 0xA);
MENU2_AT(Rec164, field_0C, 0xC);
MENU2_AT(Rec164, field_0D, 0xD);
MENU2_AT(Rec164, field_0E, 0xE);
MENU2_AT(Rec164, field_10, 0x10);

/* Record Panel5B608 opens on; its bytes at 8 and 0xC are read unsigned (Rec164 has char there). */
typedef struct Rec5B608 {
    char pad_00[0x8];
    unsigned char field_08;
    char pad_09[0x3];
    unsigned char field_0C;
} Rec5B608;
MENU2_AT(Rec5B608, field_08, 0x8);
MENU2_AT(Rec5B608, field_0C, 0xC);

/* vtable D_08A58420; constructor func_08A573B4. Shows one Rec164 in six items. */
typedef struct Panel58420 {
    Panel base;
    int field_28;
    int state;
    int field_30;
    void *field_34[0x27];
    MenuModel *handles[5];
    void *texts[1];
    Item *items[6]; /* the six members below */
    Item583C0 field_100;
    Item583C0 field_33C;
    Item58380 field_578;
    Item583A0 field_AB0;
    Item583E0 field_DFC;
    Item58400 field_F54;
    int field_10EC;
    char pad_10F0[0x2C];
    Rec164 **record;
    int field_1120;
} Panel58420;
MENU2_SIZE(Panel58420, 0x1124);
MENU2_AT(Panel58420, base, 0x0);
MENU2_AT(Panel58420, field_28, 0x28);
MENU2_AT(Panel58420, state, 0x2C);
MENU2_AT(Panel58420, field_30, 0x30);
MENU2_AT(Panel58420, field_34, 0x34);
MENU2_AT(Panel58420, handles, 0xD0);
MENU2_AT(Panel58420, texts, 0xE4);
MENU2_AT(Panel58420, items, 0xE8);
MENU2_AT(Panel58420, field_100, 0x100);
MENU2_AT(Panel58420, field_33C, 0x33C);
MENU2_AT(Panel58420, field_578, 0x578);
MENU2_AT(Panel58420, field_AB0, 0xAB0);
MENU2_AT(Panel58420, field_DFC, 0xDFC);
MENU2_AT(Panel58420, field_F54, 0xF54);
MENU2_AT(Panel58420, field_10EC, 0x10EC);
MENU2_AT(Panel58420, record, 0x111C);
MENU2_AT(Panel58420, field_1120, 0x1120);

/* ------------------------------------------------------------ parts (vtable at +8, base func_08A58740) */

/* Part base: constructor func_08A58740 (vtable D_08A5B530 at +8), reset func_08A58778. */
typedef struct Part {
    char field_00;
    char field_01;
    short field_02;
    short field_04;
    char pad_06[0x2];
    void *vtable;
} Part;
MENU2_SIZE(Part, 0xC);
MENU2_AT(Part, field_00, 0x0);
MENU2_AT(Part, field_01, 0x1);
MENU2_AT(Part, field_02, 0x2);
MENU2_AT(Part, field_04, 0x4);
MENU2_AT(Part, vtable, 0x8);

/* vtable D_08A5B578; constructor func_08A58868. */
typedef struct Part5B578 {
    Part base;
    char pad_0C[0x8];
    MenuModel *handles[4];
    char pad_24[0x4];
    short script[0x14];
    void *texts[2];
    int count;
    int values[3]; /* filled by func_08A58C24 */
} Part5B578;
MENU2_SIZE(Part5B578, 0x68);
MENU2_AT(Part5B578, base, 0x0);
MENU2_AT(Part5B578, handles, 0x14);
MENU2_AT(Part5B578, script, 0x28);
MENU2_AT(Part5B578, texts, 0x50);
MENU2_AT(Part5B578, count, 0x58);
MENU2_AT(Part5B578, values, 0x5C);

/* vtable D_08A5B5C0; constructor func_08A59028. */
typedef struct Part5B5C0 {
    Part base;
    char pad_0C[0x4];
    int field_10;
    char pad_14[0x4];
    MenuModel *handles[7];
    char pad_34[0x4];
    short script[0x20];
    void *texts[8];
    char pad_98[0x4];
    int field_9C; /* index into field_2C4 */
    char pad_A0[0x224];
    int field_2C4[1]; /* length not known */
    char pad_2C8[0x1FC];
} Part5B5C0;
MENU2_SIZE(Part5B5C0, 0x4C4);
MENU2_AT(Part5B5C0, base, 0x0);
MENU2_AT(Part5B5C0, field_10, 0x10);
MENU2_AT(Part5B5C0, handles, 0x18);
MENU2_AT(Part5B5C0, script, 0x38);
MENU2_AT(Part5B5C0, texts, 0x78);
MENU2_AT(Part5B5C0, field_9C, 0x9C);
MENU2_AT(Part5B5C0, field_2C4, 0x2C4);

/* ------------------------------------------------------------ other panels */

/* vtable D_08A60670; constructor func_08A601B0. */
typedef struct Panel60670 {
    Panel base;
    int state;
    void *messages;
    int field_30;
    void *field_34[1];
    MenuModel *handles[2];
    void *texts[2];
    short script[8];
} Panel60670;
MENU2_SIZE(Panel60670, 0x58);
MENU2_AT(Panel60670, base, 0x0);
MENU2_AT(Panel60670, state, 0x28);
MENU2_AT(Panel60670, messages, 0x2C);
MENU2_AT(Panel60670, field_30, 0x30);
MENU2_AT(Panel60670, field_34, 0x34);
MENU2_AT(Panel60670, handles, 0x38);
MENU2_AT(Panel60670, texts, 0x40);
MENU2_AT(Panel60670, script, 0x48);

/* vtable D_08A5B608; constructor func_08A59C5C. */
typedef struct Panel5B608 {
    Panel base;
    int field_28;
    int state;
    char pad_30[0x14];
    MenuModel *field_44;
    MenuModel *field_48;
    char pad_4C[0x2C];
    Part5B578 field_78[3];
    Part5B5C0 field_1B0[5];
    Panel60670 field_1984;
    MenuCursor cursor;
    int field_1A08; /* a time (func_088729A0) */
    Rec5B608 *record;
} Panel5B608;
MENU2_AT(Panel5B608, base, 0x0);
MENU2_AT(Panel5B608, field_28, 0x28);
MENU2_AT(Panel5B608, state, 0x2C);
MENU2_AT(Panel5B608, field_44, 0x44);
MENU2_AT(Panel5B608, field_48, 0x48);
MENU2_AT(Panel5B608, field_78, 0x78);
MENU2_AT(Panel5B608, field_1B0, 0x1B0);
MENU2_AT(Panel5B608, field_1984, 0x1984);
MENU2_AT(Panel5B608, cursor, 0x19DC);
MENU2_AT(Panel5B608, field_1A08, 0x1A08);
MENU2_AT(Panel5B608, record, 0x1A0C);

typedef struct Obj56C {
    char pad_00[0x8];
    unsigned char field_08;
} Obj56C;
MENU2_AT(Obj56C, field_08, 0x8);

/* Constructor func_08A5B958 (no vtable): a layout and the object func_08A80E34 drives. */
typedef struct Obj5B958 {
    int state;
    int field_04;
    void *heap;
    MenuLayout layout;
    void *field_40C; /* freed from heap */
    char field_410[4]; /* set up by func_08A5BC98 (length not known) */
    char pad_414[0x158];
    Obj56C *field_56C;
} Obj5B958;
MENU2_AT(Obj5B958, state, 0x0);
MENU2_AT(Obj5B958, field_04, 0x4);
MENU2_AT(Obj5B958, heap, 0x8);
MENU2_AT(Obj5B958, layout, 0xC);
MENU2_AT(Obj5B958, field_40C, 0x40C);
MENU2_AT(Obj5B958, field_410, 0x410);
MENU2_AT(Obj5B958, field_56C, 0x56C);

/* Entry of D_08A5DC38 (sixteen of them, one per Cell5C5C8). */
typedef struct CellSetup {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
} CellSetup;
MENU2_SIZE(CellSetup, 0x10);
MENU2_AT(CellSetup, field_00, 0x0);
MENU2_AT(CellSetup, field_04, 0x4);
MENU2_AT(CellSetup, field_08, 0x8);
MENU2_AT(CellSetup, field_0C, 0xC);

/* 0x54-byte cell built by func_08A5C5C8 (sixteen in Panel5DB80). */
typedef struct Cell5C5C8 {
    int field_00;
    MenuModel *handles[4];
    char pad_14[0x4];
    short script[0x14];
    void *texts[2];
    char field_48; /* selected */
    char field_49;
    char field_4A;
    char pad_4B[0x1];
    int field_4C;
    int field_50;
} Cell5C5C8;
MENU2_SIZE(Cell5C5C8, 0x54);
MENU2_AT(Cell5C5C8, field_00, 0x0);
MENU2_AT(Cell5C5C8, handles, 0x4);
MENU2_AT(Cell5C5C8, script, 0x18);
MENU2_AT(Cell5C5C8, texts, 0x40);
MENU2_AT(Cell5C5C8, field_48, 0x48);
MENU2_AT(Cell5C5C8, field_49, 0x49);
MENU2_AT(Cell5C5C8, field_4A, 0x4A);
MENU2_AT(Cell5C5C8, field_4C, 0x4C);
MENU2_AT(Cell5C5C8, field_50, 0x50);

/* 16-byte entry of a List66. */
typedef struct Entry10 {
    char pad_00[0x6];
    unsigned short field_06; /* index of its name */
    char pad_08[0x8];
} Entry10;
MENU2_SIZE(Entry10, 0x10);
MENU2_AT(Entry10, field_06, 0x6);

/* 0x66-byte name of a List66. */
typedef struct Name66 {
    char text[100];
    short field_64; /* cleared after a name is copied in */
} Name66;
MENU2_SIZE(Name66, 0x66);
MENU2_AT(Name66, text, 0x0);
MENU2_AT(Name66, field_64, 0x64);

/* What Panel68180.field_C4 points to: entries and their names. */
typedef struct List66 {
    Entry10 *entries;
    Name66 *names;
    int field_08;
} List66;
MENU2_AT(List66, entries, 0x0);
MENU2_AT(List66, names, 0x4);
MENU2_AT(List66, field_08, 0x8);

/* vtable D_08A5DB80; constructor func_08A5CBA4. Sixteen cells over pages. */
typedef struct Panel5DB80 {
    Panel base;
    int state;
    void *messages;
    char pad_30[0x4];
    int field_34;
    int field_38;
    MenuModel *handles[4];
    void *texts[1];
    Panel60670 field_50;
    Cell5C5C8 cells[16];
    MenuCursorView field_5E8; /* cell on the page */
    MenuCursorView field_614; /* page (0 or 1) */
    int field_640; /* a time (func_088729A0) */
    Entry10 *field_644;
} Panel5DB80;
MENU2_AT(Panel5DB80, base, 0x0);
MENU2_AT(Panel5DB80, state, 0x28);
MENU2_AT(Panel5DB80, messages, 0x2C);
MENU2_AT(Panel5DB80, field_34, 0x34);
MENU2_AT(Panel5DB80, field_38, 0x38);
MENU2_AT(Panel5DB80, handles, 0x3C);
MENU2_AT(Panel5DB80, texts, 0x4C);
MENU2_AT(Panel5DB80, field_50, 0x50);
MENU2_AT(Panel5DB80, cells, 0xA8);
MENU2_AT(Panel5DB80, field_5E8, 0x5E8);
MENU2_AT(Panel5DB80, field_614, 0x614);
MENU2_AT(Panel5DB80, field_640, 0x640);
MENU2_AT(Panel5DB80, field_644, 0x644);

/* vtable D_08A5EED8; constructor func_08A5DE10. */
typedef struct Panel5EED8 {
    Panel base;
    int state;
    void *messages;
    void *field_30[4];
    MenuModel *handles[0x11];
    void *texts[7];
    Panel60670 field_A0;
    MenuCursor field_F8;
    MenuCursor field_124;
    int field_150;
    int field_154;
    char pad_158[0x8];
    int field_160; /* the value func_08A5ECCC prints */
    char pad_164[0x4];
    int field_168;
    int field_16C;
    char field_170;
    char field_171;
} Panel5EED8;
MENU2_AT(Panel5EED8, base, 0x0);
MENU2_AT(Panel5EED8, state, 0x28);
MENU2_AT(Panel5EED8, messages, 0x2C);
MENU2_AT(Panel5EED8, field_30, 0x30);
MENU2_AT(Panel5EED8, handles, 0x40);
MENU2_AT(Panel5EED8, texts, 0x84);
MENU2_AT(Panel5EED8, field_A0, 0xA0);
MENU2_AT(Panel5EED8, field_F8, 0xF8);
MENU2_AT(Panel5EED8, field_124, 0x124);
MENU2_AT(Panel5EED8, field_150, 0x150);
MENU2_AT(Panel5EED8, field_154, 0x154);
MENU2_AT(Panel5EED8, field_160, 0x160);
MENU2_AT(Panel5EED8, field_168, 0x168);
MENU2_AT(Panel5EED8, field_16C, 0x16C);
MENU2_AT(Panel5EED8, field_170, 0x170);
MENU2_AT(Panel5EED8, field_171, 0x171);

/* vtable D_08A5FA00; constructor func_08A5F030. */
typedef struct Panel5FA00 {
    Panel base;
    int state;
    int field_2C;
    void *field_30[3];
    MenuModel *handles[0xB9];
    int field_320; /* a time (func_088729A0) */
    MenuCursor field_324;
    MenuCursor field_350;
    int field_37C;
    int field_380;
    int field_384;
} Panel5FA00;
MENU2_AT(Panel5FA00, base, 0x0);
MENU2_AT(Panel5FA00, state, 0x28);
MENU2_AT(Panel5FA00, field_2C, 0x2C);
MENU2_AT(Panel5FA00, field_30, 0x30);
MENU2_AT(Panel5FA00, handles, 0x3C);
MENU2_AT(Panel5FA00, field_320, 0x320);
MENU2_AT(Panel5FA00, field_324, 0x324);
MENU2_AT(Panel5FA00, field_350, 0x350);
MENU2_AT(Panel5FA00, field_37C, 0x37C);
MENU2_AT(Panel5FA00, field_380, 0x380);
MENU2_AT(Panel5FA00, field_384, 0x384);

/* Block at Work4FFF4 +0x6FFC handed to Panel61C40. */
typedef struct Data6FFC {
    char pad_00[0x10];
    int field_10;
    char pad_14[0x10];
} Data6FFC;
MENU2_SIZE(Data6FFC, 0x24);
MENU2_AT(Data6FFC, field_10, 0x10);

/* vtable D_08A61C40; constructor func_08A6072C. */
typedef struct Panel61C40 {
    Panel base;
    int state;
    int field_2C;
    void *messages;
    int field_34[3];
    MenuModel *handles[0x19];
    void *texts[10];
    MenuCursorView cursor;
    char pad_F8[0x4];
    int field_FC;
    char pad_100[0x24];
    int field_124; /* a time (func_088729A0) */
    Data6FFC *data;
    char field_12C;
} Panel61C40;
MENU2_AT(Panel61C40, base, 0x0);
MENU2_AT(Panel61C40, state, 0x28);
MENU2_AT(Panel61C40, field_2C, 0x2C);
MENU2_AT(Panel61C40, messages, 0x30);
MENU2_AT(Panel61C40, field_34, 0x34);
MENU2_AT(Panel61C40, handles, 0x40);
MENU2_AT(Panel61C40, texts, 0xA4);
MENU2_AT(Panel61C40, cursor, 0xCC);
MENU2_AT(Panel61C40, field_FC, 0xFC);
MENU2_AT(Panel61C40, field_124, 0x124);
MENU2_AT(Panel61C40, data, 0x128);
MENU2_AT(Panel61C40, field_12C, 0x12C);

/* vtable D_08A63680; constructor func_08A61EE0. */
typedef struct Panel63680 {
    Panel base;
    int state;
    char pad_2C[0x4];
    int field_30;
    char pad_34[0x94];
    MenuModel *field_C8;
    char pad_CC[0x4];
    int field_D0;
    char pad_D4[0x4];
    int field_D8;
    char pad_DC[0x8];
    Widget54E98 field_E4;
    Widget52C98 field_144;
    Widget54438 field_1D4[5];
    char pad_D14[0x20];
    int field_D34;
    char pad_D38[0x28];
    int field_D60;
    char pad_D64[0x24];
    int field_D88; /* a time (func_088729A0) */
    char pad_D8C[0x4];
    int field_D90;
    char pad_D94[0x24];
    ScrollState scroll;
    char pad_DCC[0x18];
    char field_DE4[4]; /* first argument of func_088D4A68 (length not known) */
    char pad_DE8[0x18];
    void *field_E00;
    char pad_E04[0x3C];
    int field_E40;
    unsigned char field_E44;
    unsigned char field_E45;
} Panel63680;
MENU2_AT(Panel63680, base, 0x0);
MENU2_AT(Panel63680, state, 0x28);
MENU2_AT(Panel63680, field_30, 0x30);
MENU2_AT(Panel63680, field_C8, 0xC8);
MENU2_AT(Panel63680, field_D0, 0xD0);
MENU2_AT(Panel63680, field_D8, 0xD8);
MENU2_AT(Panel63680, field_E4, 0xE4);
MENU2_AT(Panel63680, field_144, 0x144);
MENU2_AT(Panel63680, field_1D4, 0x1D4);
MENU2_AT(Panel63680, field_D34, 0xD34);
MENU2_AT(Panel63680, field_D60, 0xD60);
MENU2_AT(Panel63680, field_D88, 0xD88);
MENU2_AT(Panel63680, field_D90, 0xD90);
MENU2_AT(Panel63680, scroll, 0xDB8);
MENU2_AT(Panel63680, field_DE4, 0xDE4);
MENU2_AT(Panel63680, field_E00, 0xE00);
MENU2_AT(Panel63680, field_E40, 0xE40);
MENU2_AT(Panel63680, field_E44, 0xE44);
MENU2_AT(Panel63680, field_E45, 0xE45);

/* ------------------------------------------------------------ pages (vtable at +0xC, base func_08A638B0) */

/* Page base: constructor func_08A638B0 (vtable D_08A67E38 at +0xC), reset func_08A638C4. */
typedef struct Page {
    char field_00;
    char field_01;
    char field_02;
    char field_03;
    char field_04;
    char pad_05[0x3];
    int field_08;
    void *vtable;
} Page;
MENU2_SIZE(Page, 0x10);
MENU2_AT(Page, field_00, 0x0);
MENU2_AT(Page, field_01, 0x1);
MENU2_AT(Page, field_02, 0x2);
MENU2_AT(Page, field_03, 0x3);
MENU2_AT(Page, field_04, 0x4);
MENU2_AT(Page, field_08, 0x8);
MENU2_AT(Page, vtable, 0xC);

/* vtable D_08A67EE0; constructor func_08A63A48. */
typedef struct Page67EE0 {
    Page base;
    int field_10;
    int field_14;
    int field_18;
    MenuModel *handles[6];
    short script[0x18];
    void *texts[4];
    char pad_74[0x2C];
    int field_A0;
    char pad_A4[0x800];
} Page67EE0;
MENU2_SIZE(Page67EE0, 0x8A4);
MENU2_AT(Page67EE0, base, 0x0);
MENU2_AT(Page67EE0, field_10, 0x10);
MENU2_AT(Page67EE0, field_14, 0x14);
MENU2_AT(Page67EE0, field_18, 0x18);
MENU2_AT(Page67EE0, handles, 0x1C);
MENU2_AT(Page67EE0, script, 0x34);
MENU2_AT(Page67EE0, texts, 0x64);
MENU2_AT(Page67EE0, field_A0, 0xA0);

/* vtable D_08A67F88; constructor func_08A644A4. */
typedef struct Page67F88 {
    Page base;
    char pad_10[0x4];
    MenuModel *handles[3];
    char pad_20[0x4];
    short script[0x10];
    void *texts[2];
} Page67F88;
MENU2_SIZE(Page67F88, 0x4C);
MENU2_AT(Page67F88, base, 0x0);
MENU2_AT(Page67F88, handles, 0x14);
MENU2_AT(Page67F88, script, 0x24);
MENU2_AT(Page67F88, texts, 0x44);

/* vtable D_08A68030; constructor func_08A64B84. */
typedef struct Page68030 {
    Page base;
    char pad_10[0x8];
    MenuModel *handles[5];
    char pad_2C[0x4];
    short script[0x18];
    void *texts[2];
    char pad_68[0x30];
} Page68030;
MENU2_SIZE(Page68030, 0x98);
MENU2_AT(Page68030, base, 0x0);
MENU2_AT(Page68030, handles, 0x18);
MENU2_AT(Page68030, script, 0x30);
MENU2_AT(Page68030, texts, 0x60);

/* vtable D_08A680D8; constructor func_08A65420. Shows one Entry10 and its name. */
typedef struct Page680D8 {
    Page base;
    int field_10;
    int field_14;
    char pad_18[0x4];
    MenuModel *handles[8];
    short script[0x20];
    void *texts[3];
    Entry10 *entry;
    Name66 *names;
    int field_90;
    int field_94;
} Page680D8;
MENU2_SIZE(Page680D8, 0x98);
MENU2_AT(Page680D8, base, 0x0);
MENU2_AT(Page680D8, field_10, 0x10);
MENU2_AT(Page680D8, field_14, 0x14);
MENU2_AT(Page680D8, handles, 0x1C);
MENU2_AT(Page680D8, script, 0x3C);
MENU2_AT(Page680D8, texts, 0x7C);
MENU2_AT(Page680D8, entry, 0x88);
MENU2_AT(Page680D8, names, 0x8C);
MENU2_AT(Page680D8, field_90, 0x90);
MENU2_AT(Page680D8, field_94, 0x94);

/* vtable D_08A68180; constructor func_08A66218. A list of Entry10 shown on pages. */
typedef struct Panel68180 {
    Panel base;
    char pad_28[0x8];
    int field_30;
    char pad_34[0x4];
    int field_38;
    char pad_3C[0x4];
    int field_40;
    char pad_44[0xC];
    int field_50;
    char pad_54[0x24];
    ScrollState scroll;
    char pad_8C[0x18];
    char field_A4[4]; /* first argument of func_088D4A68 (length not known) */
    char pad_A8[0x1C];
    List66 *list;
    char pad_C8[0xC0];
    Page67EE0 field_188[2];
    Page67F88 field_12D0;
    Page68030 field_131C;
    Page680D8 field_13B4[0x14];
    int field_1F94;
} Panel68180;
MENU2_AT(Panel68180, base, 0x0);
MENU2_AT(Panel68180, field_30, 0x30);
MENU2_AT(Panel68180, field_38, 0x38);
MENU2_AT(Panel68180, field_40, 0x40);
MENU2_AT(Panel68180, field_50, 0x50);
MENU2_AT(Panel68180, scroll, 0x78);
MENU2_AT(Panel68180, field_A4, 0xA4);
MENU2_AT(Panel68180, list, 0xC4);
MENU2_AT(Panel68180, field_188, 0x188);
MENU2_AT(Panel68180, field_12D0, 0x12D0);
MENU2_AT(Panel68180, field_131C, 0x131C);
MENU2_AT(Panel68180, field_13B4, 0x13B4);
MENU2_AT(Panel68180, field_1F94, 0x1F94);

/* vtable D_08A694A8; constructor func_08A68388. */
typedef struct Panel694A8 {
    Panel base;
    int field_28;
    int field_2C;
    int state;
    int field_34;
    void *field_38[2];
    MenuModel *handles[0x10];
    void *texts[9];
    char pad_A4[0x2C];
    int field_D0; /* a time (func_088729A0) */
    int field_D4;
    int field_D8;
    int field_DC;
} Panel694A8;
MENU2_AT(Panel694A8, base, 0x0);
MENU2_AT(Panel694A8, field_28, 0x28);
MENU2_AT(Panel694A8, field_2C, 0x2C);
MENU2_AT(Panel694A8, state, 0x30);
MENU2_AT(Panel694A8, field_34, 0x34);
MENU2_AT(Panel694A8, field_38, 0x38);
MENU2_AT(Panel694A8, handles, 0x40);
MENU2_AT(Panel694A8, texts, 0x80);
MENU2_AT(Panel694A8, field_D0, 0xD0);
MENU2_AT(Panel694A8, field_D4, 0xD4);
MENU2_AT(Panel694A8, field_D8, 0xD8);
MENU2_AT(Panel694A8, field_DC, 0xDC);

/* vtable D_08A69A78; constructor func_08A69600. Fills a MenuPrompt. */
typedef struct Panel69A78 {
    Panel base;
    int state;
    int field_2C;
    void *messages;
    int field_34;
    int field_38;
    int field_3C;
    int field_40;
    void *field_44[1];
    MenuModel *handles[1];
    void *texts[6];
    unsigned char field_64[4]; /* table for the prompt (length not known) */
    char pad_68[0x148];
    MenuPrompt prompt;
} Panel69A78;
MENU2_AT(Panel69A78, base, 0x0);
MENU2_AT(Panel69A78, state, 0x28);
MENU2_AT(Panel69A78, field_2C, 0x2C);
MENU2_AT(Panel69A78, messages, 0x30);
MENU2_AT(Panel69A78, field_34, 0x34);
MENU2_AT(Panel69A78, field_38, 0x38);
MENU2_AT(Panel69A78, field_3C, 0x3C);
MENU2_AT(Panel69A78, field_40, 0x40);
MENU2_AT(Panel69A78, field_44, 0x44);
MENU2_AT(Panel69A78, handles, 0x48);
MENU2_AT(Panel69A78, texts, 0x4C);
MENU2_AT(Panel69A78, field_64, 0x64);
MENU2_AT(Panel69A78, prompt, 0x1B0);

/* vtable D_08A6B170; constructor func_08A69C20. */
typedef struct Panel6B170 {
    Panel base;
    int state;
    int field_2C;
    int field_30;
    MenuLayout layout;
    void *field_434; /* driven by func_08A80E34 */
    char pad_438[0x40];
    int field_478;
    char pad_47C[0x118];
    void *field_594[9];
    MenuModel *handles[0x1C];
    void *texts[1];
    char pad_62C[0x40];
    int field_66C;
    int field_670;
    int field_674;
    int field_678;
} Panel6B170;
MENU2_AT(Panel6B170, base, 0x0);
MENU2_AT(Panel6B170, state, 0x28);
MENU2_AT(Panel6B170, field_2C, 0x2C);
MENU2_AT(Panel6B170, field_30, 0x30);
MENU2_AT(Panel6B170, layout, 0x34);
MENU2_AT(Panel6B170, field_434, 0x434);
MENU2_AT(Panel6B170, field_478, 0x478);
MENU2_AT(Panel6B170, field_594, 0x594);
MENU2_AT(Panel6B170, handles, 0x5B8);
MENU2_AT(Panel6B170, texts, 0x628);
MENU2_AT(Panel6B170, field_66C, 0x66C);
MENU2_AT(Panel6B170, field_670, 0x670);
MENU2_AT(Panel6B170, field_674, 0x674);
MENU2_AT(Panel6B170, field_678, 0x678);

/* What Panel6C638.field_210 points to (through one more pointer). */
typedef struct Data6C638 {
    char pad_00[0x1AC];
    Rec164 records[1]; /* length not known */
} Data6C638;
MENU2_AT(Data6C638, records, 0x1AC);

/* vtable D_08A6C638; constructor func_08A6B418. A list of Rec164 and one shown in a Panel58420. */
typedef struct Panel6C638 {
    Panel base;
    int state;
    int field_2C;
    int field_30;
    void *field_34[0x1D];
    MenuModel *handles[0x3A];
    void *texts[0x15];
    MenuCursorView cursor;
    Data6C638 **data;
    int field_214; /* cursor index to restore, -1 for none */
    Panel58420 view;
    Rec164 *record; /* the record the view shows */
    int field_1340;
} Panel6C638;
MENU2_AT(Panel6C638, base, 0x0);
MENU2_AT(Panel6C638, state, 0x28);
MENU2_AT(Panel6C638, field_2C, 0x2C);
MENU2_AT(Panel6C638, field_30, 0x30);
MENU2_AT(Panel6C638, field_34, 0x34);
MENU2_AT(Panel6C638, handles, 0xA8);
MENU2_AT(Panel6C638, texts, 0x190);
MENU2_AT(Panel6C638, cursor, 0x1E4);
MENU2_AT(Panel6C638, data, 0x210);
MENU2_AT(Panel6C638, field_214, 0x214);
MENU2_AT(Panel6C638, view, 0x218);
MENU2_AT(Panel6C638, record, 0x133C);
MENU2_AT(Panel6C638, field_1340, 0x1340);

/* ------------------------------------------------------------ Screen4FFF4 (D_08A4FFF4) */

/* Work block of Screen4FFF4 (+0x5C). */
typedef struct Work4FFF4 {
    char pad_00[0x4];
    int state;
    void *field_08; /* freed from the heap */
    void *messages; /* handed to every panel */
    int field_10;
    char pad_14[0x6B0C];
    char *field_6B20;
    char pad_6B24[0xC];
    char field_6B30;
    unsigned char field_6B31;
    char field_6B32[0x66]; /* a name being edited */
    char pad_6B98[0x19C];
    int field_6D34; /* block handed to func_08B58B64 */
    int field_6D38;
    char pad_6D3C[0x88];
    char field_6DC4[0x34];
    char *field_6DF8;
    char pad_6DFC[0x8];
    MenuCallback field_6E04;
    MenuCallback field_6E08;
    char pad_6E0C[0x80];
    Name66 *field_6E8C;
    char pad_6E90[0x6C];
    int field_6EFC; /* block handed to func_08A4E4E0 */
    char pad_6F00[0xD4];
    Panel61C40 *field_6FD4;
    Panel63680 *field_6FD8;
    Panel5FA00 *field_6FDC;
    Panel694A8 *field_6FE0;
    char pad_6FE4[0x4];
    Obj5B958 *field_6FE8;
    Panel68180 *field_6FEC;
    Panel5DB80 *field_6FF0;
    Panel5EED8 *field_6FF4;
    Panel69A78 *field_6FF8;
    Data6FFC field_6FFC;
    int field_7020; /* block handed to func_08A62128 */
    char pad_7024[0x160];
    Obj3A864 *field_7184; /* freed from the heap */
    char pad_7188[0x8];
    int field_7190;
    int field_7194;
    char pad_7198[0x4];
    int field_719C;
} Work4FFF4;
MENU2_AT(Work4FFF4, state, 0x4);
MENU2_AT(Work4FFF4, field_08, 0x8);
MENU2_AT(Work4FFF4, messages, 0xC);
MENU2_AT(Work4FFF4, field_10, 0x10);
MENU2_AT(Work4FFF4, field_6B20, 0x6B20);
MENU2_AT(Work4FFF4, field_6B30, 0x6B30);
MENU2_AT(Work4FFF4, field_6B31, 0x6B31);
MENU2_AT(Work4FFF4, field_6B32, 0x6B32);
MENU2_AT(Work4FFF4, field_6D34, 0x6D34);
MENU2_AT(Work4FFF4, field_6D38, 0x6D38);
MENU2_AT(Work4FFF4, field_6DC4, 0x6DC4);
MENU2_AT(Work4FFF4, field_6DF8, 0x6DF8);
MENU2_AT(Work4FFF4, field_6E04, 0x6E04);
MENU2_AT(Work4FFF4, field_6E08, 0x6E08);
MENU2_AT(Work4FFF4, field_6E8C, 0x6E8C);
MENU2_AT(Work4FFF4, field_6EFC, 0x6EFC);
MENU2_AT(Work4FFF4, field_6FD4, 0x6FD4);
MENU2_AT(Work4FFF4, field_6FD8, 0x6FD8);
MENU2_AT(Work4FFF4, field_6FDC, 0x6FDC);
MENU2_AT(Work4FFF4, field_6FE0, 0x6FE0);
MENU2_AT(Work4FFF4, field_6FE8, 0x6FE8);
MENU2_AT(Work4FFF4, field_6FEC, 0x6FEC);
MENU2_AT(Work4FFF4, field_6FF0, 0x6FF0);
MENU2_AT(Work4FFF4, field_6FF4, 0x6FF4);
MENU2_AT(Work4FFF4, field_6FF8, 0x6FF8);
MENU2_AT(Work4FFF4, field_6FFC, 0x6FFC);
MENU2_AT(Work4FFF4, field_7020, 0x7020);
MENU2_AT(Work4FFF4, field_7184, 0x7184);
MENU2_AT(Work4FFF4, field_7190, 0x7190);
MENU2_AT(Work4FFF4, field_7194, 0x7194);
MENU2_AT(Work4FFF4, field_719C, 0x719C);

/* Screen in D_08A4FFF4 (0x08A4C180..0x08A4EEC4). */
typedef struct Screen4FFF4 {
    void *field_00; /* released with func_08882158 */
    MenuTask task;
    void *field_38; /* released with func_0888208C */
    void *heap;
    int field_40;
    int field_44;
    char pad_48[0x14];
    Work4FFF4 *work;
} Screen4FFF4;
MENU2_AT(Screen4FFF4, field_00, 0x0);
MENU2_AT(Screen4FFF4, task, 0x4);
MENU2_AT(Screen4FFF4, field_38, 0x38);
MENU2_AT(Screen4FFF4, heap, 0x3C);
MENU2_AT(Screen4FFF4, field_40, 0x40);
MENU2_AT(Screen4FFF4, field_44, 0x44);
MENU2_AT(Screen4FFF4, work, 0x5C);

extern Screen4FFF4 *D_08A4FFF4;

/* ------------------------------------------------------------ Screen51CBC (D_08A51CBC) */

typedef struct Row2E0 {
    char data[0x2E0];
} Row2E0;
MENU2_SIZE(Row2E0, 0x2E0);
MENU2_AT(Row2E0, data, 0x0);

/* Work block of Screen51CBC (+0x58). */
typedef struct Work51CBC {
    char pad_00[0x8B0];
    Row2E0 rows[1]; /* length not known */
    char pad_B90[0x5F88];
    void *field_6B18; /* freed from the heap */
    char pad_6B1C[0xE88];
    int field_79A4;
    char pad_79A8[0x4];
    int field_79AC; /* block handed to func_08A51208 */
    char pad_79B0[0x210];
    char field_7BC0[4]; /* length not known */
    char pad_7BC4[0x774];
    Row2E0 *row;
} Work51CBC;
MENU2_AT(Work51CBC, rows, 0x8B0);
MENU2_AT(Work51CBC, field_6B18, 0x6B18);
MENU2_AT(Work51CBC, field_79A4, 0x79A4);
MENU2_AT(Work51CBC, field_79AC, 0x79AC);
MENU2_AT(Work51CBC, field_7BC0, 0x7BC0);
MENU2_AT(Work51CBC, row, 0x8338);

/* Screen in D_08A51CBC (0x08A500A8..0x08A5112C); constructor func_08A500A8. */
typedef struct Screen51CBC {
    char pad_00[0x8];
    void *field_08; /* released with func_08882158 */
    void *field_0C; /* released with func_0888208C */
    void *heap;
    MenuTask task;
    char pad_48[0x10];
    Work51CBC *work;
} Screen51CBC;
MENU2_AT(Screen51CBC, field_08, 0x8);
MENU2_AT(Screen51CBC, field_0C, 0xC);
MENU2_AT(Screen51CBC, heap, 0x10);
MENU2_AT(Screen51CBC, task, 0x14);
MENU2_AT(Screen51CBC, work, 0x58);

extern Screen51CBC *D_08A51CBC;

/* ------------------------------------------------------------ Screen6D710 (D_08A6D710) */

/* Screen in D_08A6D710 (0x08A6CBE0..0x08A6D38C). */
typedef struct Screen6D710 {
    char pad_00[0x100];
    MenuTask task;
    char pad_134[0x7C];
    char field_1B0[4]; /* machine run by func_08A79EF0 (length not known) */
    char pad_1B4[0xB0];
    char field_264[4]; /* length not known */
} Screen6D710;
MENU2_AT(Screen6D710, task, 0x100);
MENU2_AT(Screen6D710, field_1B0, 0x1B0);
MENU2_AT(Screen6D710, field_264, 0x264);

extern Screen6D710 *D_08A6D710;

/* Forwarders 0x08A6D2E4..0x08A6D370 (owner at +4). */
typedef struct Delegate6D710 {
    void *field_00;
    Screen6D710 *owner;
} Delegate6D710;
MENU2_AT(Delegate6D710, field_00, 0x0);
MENU2_AT(Delegate6D710, owner, 0x4);

/* func_08A6D38C (owner at +8). */
typedef struct Delegate6D38C {
    void *field_00;
    int field_04;
    Screen6D710 *owner;
} Delegate6D38C;
MENU2_AT(Delegate6D38C, field_00, 0x0);
MENU2_AT(Delegate6D38C, field_04, 0x4);
MENU2_AT(Delegate6D38C, owner, 0x8);

/* ------------------------------------------------------------ 0x08A6D7CC..0x08A70904 */

/* Request block (constructor func_08A6E168, reset func_08A6E388). */
typedef struct Request6E168 {
    void *field_00; /* released with func_08A6E450 */
    int field_04;
    int field_08;
    int kind;
    char data[0x40];
    int size; /* bytes in data */
} Request6E168;
MENU2_SIZE(Request6E168, 0x54);
MENU2_AT(Request6E168, field_00, 0x0);
MENU2_AT(Request6E168, field_04, 0x4);
MENU2_AT(Request6E168, field_08, 0x8);
MENU2_AT(Request6E168, kind, 0xC);
MENU2_AT(Request6E168, data, 0x10);
MENU2_AT(Request6E168, size, 0x50);

/* Owner of a Request6E168; the state objects below act on it. */
typedef struct Obj6ECF4 {
    int field_00;
    char pad_04[0x4];
    char field_08[4]; /* main-module object (func_0890A69C ...; length not known) */
    char pad_0C[0x1C4];
    int field_1D0; /* handle closed by func_0890EA18 */
    Request6E168 request;
} Obj6ECF4;
MENU2_AT(Obj6ECF4, field_00, 0x0);
MENU2_AT(Obj6ECF4, field_08, 0x8);
MENU2_AT(Obj6ECF4, field_1D0, 0x1D0);
MENU2_AT(Obj6ECF4, request, 0x1D4);

/* State objects with vtables D_08A6DAD0 ... D_08A6EAC8; base destructor func_08A6D7E8. */
typedef struct State6D7E8 {
    void *vtable;
    unsigned char field_04;
} State6D7E8;
MENU2_AT(State6D7E8, vtable, 0x0);
MENU2_AT(State6D7E8, field_04, 0x4);

/* Forwarders 0x08A6F610..0x08A6F750 (owner at +4). */
typedef struct Delegate6F610 {
    void *field_00;
    Obj6ECF4 *owner;
} Delegate6F610;
MENU2_AT(Delegate6F610, field_00, 0x0);
MENU2_AT(Delegate6F610, owner, 0x4);

/* Forwarders 0x08A6FF6C, 0x08A6FF88 (owner at +4). */
typedef struct Delegate6FF6C {
    void *field_00;
    void *owner;
} Delegate6FF6C;
MENU2_AT(Delegate6FF6C, field_00, 0x0);
MENU2_AT(Delegate6FF6C, owner, 0x4);

/* 0xF0-byte element built by func_08A6EAF8. */
typedef struct Elem6EAF8 {
    char data[0xF0];
} Elem6EAF8;
MENU2_SIZE(Elem6EAF8, 0xF0);
MENU2_AT(Elem6EAF8, data, 0x0);

/* Constructor func_08A6EB20: ten Elem6EAF8. */
typedef struct Obj6EB20 {
    Elem6EAF8 elems[10];
} Obj6EB20;
MENU2_AT(Obj6EB20, elems, 0x0);

typedef struct Obj70270 {
    char pad_00[0x210];
    int field_210;
} Obj70270;
MENU2_AT(Obj70270, field_210, 0x210);

typedef struct Obj7058C {
    short field_00;
    char pad_02[0x18];
    short field_1A;
} Obj7058C;
MENU2_AT(Obj7058C, field_00, 0x0);
MENU2_AT(Obj7058C, field_1A, 0x1A);

/* Six bytes set by func_08A70798 / func_08A707BC. */
typedef struct Bytes6 {
    char field_00;
    char field_01;
    char field_02;
    char field_03;
    char field_04;
    char field_05;
} Bytes6;
MENU2_SIZE(Bytes6, 0x6);
MENU2_AT(Bytes6, field_00, 0x0);
MENU2_AT(Bytes6, field_01, 0x1);
MENU2_AT(Bytes6, field_02, 0x2);
MENU2_AT(Bytes6, field_03, 0x3);
MENU2_AT(Bytes6, field_04, 0x4);
MENU2_AT(Bytes6, field_05, 0x5);

typedef struct Obj7084C {
    int field_00;
    int field_04;
} Obj7084C;
MENU2_SIZE(Obj7084C, 0x8);
MENU2_AT(Obj7084C, field_00, 0x0);
MENU2_AT(Obj7084C, field_04, 0x4);

typedef struct Obj708C8 {
    char pad_00[0x148];
    char field_148[4]; /* set up by func_08A7059C (length not known) */
    char pad_14C[0xE9C];
    Obj7084C field_FE8;
    char field_FF0[4]; /* set up by func_08A7085C (length not known) */
} Obj708C8;
MENU2_AT(Obj708C8, field_148, 0x148);
MENU2_AT(Obj708C8, field_FE8, 0xFE8);
MENU2_AT(Obj708C8, field_FF0, 0xFF0);

/* ------------------------------------------------------------ main module */

int func_088729A0(int arg);                                       /* a time stamp */
void func_08882158(void *block);
void func_088825D4(void *owner, void *object);
void func_08885220(void *object);
void func_0887E684(void *object);
void *func_0887D868(void *messages, int kind, void *text, int arg1, int arg2, int arg3, int arg4,
                    int arg5);
int func_089291E4(void *out, int size, int value, int arg1, int arg2); /* print a value */
void *func_08935524(void *dst, const void *src, int size);         /* memcpy */
void func_088DA060(void *model);                                   /* release a model handle */
void func_088D3A04(void *layout);
void func_088D3A5C(void *layout, const short *script, int count);
void func_088D4A68(void *timer, ScrollState *scroll, int arg1, int arg2, short arg3, int frames,
                   char loop);
void func_088D4FE4(MenuPrompt *prompt);
void func_088D50DC(MenuPrompt *prompt);
void func_088D5318(void *object, int arg1, int arg2);
void func_088D53BC(void *object);
void func_088D54DC(void *object);
int func_088E9934(void);
void func_088E98B4(unsigned int arg1, unsigned char arg2, unsigned int arg3);
void func_088E991C(int arg, MenuCallback done);
void func_088E9958(unsigned char arg);
void func_088AEF98(void *object);
void func_088AF044(void *object);
void func_088B9E84(void *block, Row2E0 *row);
void func_088CBA10(void *block);
void func_08927458(void *object);
void func_08929644(void *object);
void func_089298F8(int arg, char *text);
void func_08929928(int arg, char *text);
int func_089292C0(void *object, int arg1, char arg2, char arg3);
void func_0890A69C(void *object);
void func_0890A7A8(void *object);
int func_0890A984(void *object);
void func_0890EA18(int handle);
unsigned char func_089050F8(void *object, int kind, void *data, int size);
int func_08B58C4C(void *object, int arg1, int arg2, char arg3);
int func_08B58F8C(void *object);
void func_08A3AA54(Obj3A864 *object);
int func_08A3AAEC(Obj3A864 *object);
void func_08A3BF2C(void);
void func_08A3BF9C(void);
void func_08A8104C(void *object, char arg);
void func_08A80D40(void *object, unsigned int arg);
int func_08A80E34(void *object);
void func_08A80EF4(void *object);
void func_08A79EF0(void *machine, Screen6D710 *owner, int event, int arg, int size);

/* ------------------------------------------------------------ Screen40248 */

void func_08A3CD08(Screen40248 *self, int time, int *hours, int *minutes, int *seconds);
void func_08A3CD40(Screen40248 *self, int time, int *minutes, int *seconds, int *hundredths);
void func_08A3CEB8(Screen40248 *self, int step);
void func_08A3D1D0(Screen40248 *self, int step);
void func_08A3E008(Screen40248 *self);
void func_08A3E21C(Screen40248 *self, unsigned int flags);

/* ------------------------------------------------------------ MenuRoot2 (some are declared with void *root in auto_region1.h) */

ScreenBase2 *func_08A408A0(ScreenBase2 *self);
void func_08A40F88(MenuRoot2 *self);
unsigned char func_08A41098(void);
void func_08A41244(MenuRoot2 *self);
char *func_08A412DC(MenuRoot2 *self, int id);
void func_08A41304(MenuRoot2 *self);
void func_08A4162C(MenuRoot2 *self);
void func_08A416DC(MenuRoot2 *self);
void func_08A4185C(MenuRoot2 *self, int index);
void func_08A418E8(MenuRoot2 *self);
void func_08A41940(MenuRoot2 *self);
void func_08A41960(MenuRoot2 *self);
void func_08A41980(MenuRoot2 *self, int value);
void func_08A419C4(MenuRoot2 *self);
void func_08A419E4(MenuRoot2 *self);
void func_08A41A04(MenuRoot2 *self);
void func_08A41BBC(MenuRoot2 *self, unsigned int bit);
void func_08A42678(MenuRoot2 *self);
void func_08A42AD0(MenuRoot2 *self);
void func_08A42B08(MenuRoot2 *self);
void func_08A42B5C(void);
void func_08A42B7C(void);
void func_08A42B9C(void);
void func_08A42BBC(void);
void func_08A42BDC(void);
void func_08A42BFC(void);
MenuRoot2 *func_08A42C1C(MenuRoot2 *self);
void func_08A42C84(void);
void func_08A42CA8(void);
int func_08A42EA4(unsigned short *node);
unsigned short *func_08A42E14(unsigned short *node);
void func_08A43094(void);
int func_08A430B8(void);
void func_08A4094C(MenuRoot2 *self);
void func_08A40C98(MenuRoot2 *self);
void func_08A40F04(MenuRoot2 *self);
void func_08A40FE0(MenuRoot2 *self, int value);

/* ------------------------------------------------------------ Screen457F0 */

void func_08A434E0(Screen457F0 *self);
void func_08A43538(Screen457F0 *self);
void func_08A4357C(Screen457F0 *self);
void func_08A435B4(Screen457F0 *self);
void func_08A43628(Screen457F0 *self);
void func_08A43660(void);
void func_08A43680(void);
void func_08A436A0(void);
Screen457F0 *func_08A436C0(Screen457F0 *self);
void func_08A43290(Screen457F0 *self);
void func_08A43374(Screen457F0 *self);
void func_08A43440(Screen457F0 *self);

/* ------------------------------------------------------------ Screen47D90, IdTable */

void func_08A45838(Screen47D90 *self);
void func_08A45C14(Screen47D90 *self, int arg);
void func_08A45C44(Screen47D90 *self);
void func_08A45C78(Screen47D90 *self);
void func_08A45C94(Screen47D90 *self, int arg);
void func_08A47130(Screen47D90 *self);
void func_08A47234(Screen47D90 *self, unsigned int flags);
IdTable *func_08A482A8(IdTable *self);
void func_08A4841C(IdTable *self);
int func_08A48450(IdTable *self, int index, int column);
int func_08A484D0(IdTable *self);
unsigned int func_08A484D8(IdTable *self, int first, int last);
int func_08A48608(IdTable *self);

/* ------------------------------------------------------------ BlockView, Screen4C0FC */

BlockView *func_08A49610(BlockView *self);
void func_08A49684(BlockView *self);
void func_08A49CB0(Screen4C0FC *self);
void func_08A49CE8(void);
void func_08A49D08(void);
void func_08A49D28(void);
void func_08A49D48(Screen4C0FC *self, int arg1, int arg2, int arg3, int arg4);
void func_08A49E10(Screen4C0FC *self);
void func_08A49F7C(Screen4C0FC *self);
void func_08A4AA48(Screen4C0FC *self);
void func_08A4AB60(void);
void func_08A4AB80(void);
void func_08A4ABA0(Screen4C0FC *self);
Screen4C0FC *func_08A4ABFC(Screen4C0FC *self);
void func_08A4AC64(void);
void func_08A49750(Screen4C0FC *self);
void func_08A499A0(Screen4C0FC *self);
void func_08A49B5C(Screen4C0FC *self);
void func_08A49BDC(Screen4C0FC *self, int arg);

/* ------------------------------------------------------------ Screen4FFF4 */

void func_08A4C128(void);
void func_08A4C180(void);
void func_08A4C1A0(void);
void func_08A4C1D4(void);
void func_08A4C1F4(void);
void func_08A4C214(void);
void func_08A4C304(Screen4FFF4 *self);
void func_08A4C3A4(Screen4FFF4 *self);
void func_08A4C4EC(Screen4FFF4 *self);
void func_08A4C598(Screen4FFF4 *self);
void func_08A4C7B8(Screen4FFF4 *self);
void func_08A4CDB4(Screen4FFF4 *self);
void func_08A4CDD4(Screen4FFF4 *self);
void func_08A4CFC4(Screen4FFF4 *self);
void func_08A4D154(Screen4FFF4 *self);
void func_08A4D1C8(Screen4FFF4 *self, int arg1, int arg2);
void func_08A4D1F8(Screen4FFF4 *self, Name66 *name);
void func_08A4D350(void);
int func_08A4D370(void);
void func_08A4D384(Screen4FFF4 *self);
void func_08A4D45C(unsigned int arg1, unsigned int arg2, unsigned char arg3);
void func_08A4D4C0(void);
void func_08A4D57C(unsigned char flag);
void func_08A4D5A8(int value);
void func_08A4D610(void);
void func_08A4D630(void);
void func_08A4D650(void);
void func_08A4D678(void);
void func_08A4D694(void);
void func_08A4D6B8(void);
void func_08A4D6D8(void);
void func_08A4D6F8(void);
void func_08A4D718(void);
void func_08A4D738(int value);
void func_08A4D77C(void);
void func_08A4D79C(Screen4FFF4 *self, unsigned char flag);
void func_08A4D824(Screen4FFF4 *self, unsigned char flag);
void func_08A4D848(Screen4FFF4 *self);
void func_08A4D868(Screen4FFF4 *self, unsigned char flag);
void func_08A4D910(Screen4FFF4 *self);
void func_08A4D930(Screen4FFF4 *self);
void func_08A4D950(Screen4FFF4 *self);
void func_08A4D984(Screen4FFF4 *self);
int func_08A4DB78(Screen4FFF4 *self, int value);
void func_08A4DE14(Screen4FFF4 *self);
void func_08A4DE34(Screen4FFF4 *self);
void func_08A4DE54(Screen4FFF4 *self);
void func_08A4DE74(Screen4FFF4 *self);
void func_08A4DEF4(Screen4FFF4 *self);
void func_08A4DF14(Screen4FFF4 *self);
void func_08A4DF78(Screen4FFF4 *self);
void func_08A4DF98(Screen4FFF4 *self);
void func_08A4DFB8(Screen4FFF4 *self);
void func_08A4E2C4(Screen4FFF4 *self, int value);
void func_08A4E2E4(Screen4FFF4 *self, int text, MenuCallback yes, MenuCallback no, MenuCallback cancel,
                   int arg5, int arg6, int arg7, int arg8);
void func_08A4E320(Screen4FFF4 *self);
void func_08A4E478(Screen4FFF4 *self);
void func_08A4E4C0(Screen4FFF4 *self);
void func_08A4E63C(Screen4FFF4 *self, Entry10 *entry);
void func_08A4E72C(Screen4FFF4 *self);
void func_08A4EA6C(Screen4FFF4 *self);
void func_08A4ECF0(Screen4FFF4 *self);
void func_08A4EDC0(void);
void func_08A4EEC4(Screen4FFF4 *self);
void func_08A4F1A8(Screen4FFF4 *self);
void func_08A4C234(Screen4FFF4 *self);
unsigned char func_08A4EDE0(Screen4FFF4 *self);
int func_08A4EE44(Screen4FFF4 *self);
void func_08A4C2B8(Screen4FFF4 *self, unsigned char flag);
void func_08A4C3DC(Screen4FFF4 *self);
void func_08A4CFD4(Screen4FFF4 *self, float delay);
void func_08A4D07C(Screen4FFF4 *self, float delay, char arg);
void func_08A4D204(Screen4FFF4 *self);
void func_08A4E1F4(Screen4FFF4 *self, int arg1, int arg2);
void func_08A4E4E0(Screen4FFF4 *self, int *block);
int func_08A4E5B8(Screen4FFF4 *self, int value);
void func_08A4E660(Screen4FFF4 *self, Entry10 *entry, int arg2, int index, Name66 *name);
void func_08A4E74C(Screen4FFF4 *self);
void func_08A4EA48(Screen4FFF4 *self);
void func_08A4ED34(Screen4FFF4 *self, int id, unsigned int value);
void func_08A4F924(Screen4FFF4 *self);
void func_08A4DBD4(Screen4FFF4 *self, int arg);
unsigned int func_08A4DA98(Screen4FFF4 *self, unsigned int arg1, unsigned char arg2);
unsigned int func_08A4DB1C(Screen4FFF4 *self, unsigned int arg1, unsigned char arg2);
int func_08A4DC18(Screen4FFF4 *self, int arg);
int func_08A4D9D0(Screen4FFF4 *self, int arg);
unsigned char func_08A4DA2C(Screen4FFF4 *self, int arg);

/* ------------------------------------------------------------ Screen51CBC */

Screen51CBC *func_08A500A8(Screen51CBC *self);
void func_08A50118(Screen51CBC *self);
void func_08A50160(void);
void func_08A5018C(void);
void func_08A501FC(void);
void func_08A5021C(void);
void func_08A5023C(void);
void func_08A50270(Screen51CBC *self);
void func_08A50438(Screen51CBC *self);
void func_08A504DC(Screen51CBC *self);
void func_08A50FB8(Screen51CBC *self);
void func_08A510BC(int value);
void func_08A5112C(Screen51CBC *self, int index);
int func_08A50650(Screen51CBC *self);
void func_08A50808(Screen51CBC *self);
void func_08A502B8(Screen51CBC *self);
void func_08A51208(Screen51CBC *self, int *block);
int func_08A5137C(Screen51CBC *self, int value);
void func_08A51598(void *self, char **out, char *data);

/* ------------------------------------------------------------ actions */

Action *func_08A51D38(Action *self);
void func_08A51D70(Action *self);
void func_08A5C02C(Action5C02C *self);
void func_08A5C058(Action5C02C *self, int value);
void func_08A5C060(Action5C02C *self);
void func_08A5C088(Action5C02C *self);
void func_08A5C0AC(Action5C02C *self);
void func_08A5C224(Action5C240 *self);
void func_08A5C240(Action5C240 *self, unsigned int value, unsigned char flag);
void func_08A5C278(Action5C240 *self);
void func_08A5C2A0(Action5C240 *self);
void func_08A5C3CC(Action5C3CC *self);
void func_08A5C3F4(Action5C3CC *self, int value);
void func_08A5C3FC(Action5C3CC *self);
void func_08A5C420(Action5C3CC *self);
void func_08A5C444(Action5C3CC *self);

/* ------------------------------------------------------------ panel base */

Panel *func_08A51DD0(Panel *self);
void func_08A51E3C(Panel *self);
void func_08A51E08(Panel *self, unsigned int flags);
void func_08A51E64(Panel *self, int arg1, int arg2, int arg3);
void func_08A51EB8(Panel *self);
void func_08A5224C(Panel *self);

/* ------------------------------------------------------------ widgets */

Widget *func_08A52300(Widget *self);
Widget52C98 *func_08A52390(Widget52C98 *self);
void func_08A523E4(Widget52C98 *self);
void func_08A52548(Widget52C98 *self);
void func_08A5258C(Widget52C98 *self);
void func_08A525DC(Widget52C98 *self);
void func_08A526FC(Widget52C98 *self);
void func_08A5272C(Widget52C98 *self);
void func_08A52748(Widget52C98 *self);
void func_08A5287C(Widget52C98 *self);
void func_08A529F8(Widget52C98 *self);
void func_08A52A4C(Widget52C98 *self, int value);
void func_08A52C08(Widget52C98 *self);
Elem52D60 *func_08A52D60(Elem52D60 *self);
void func_08A52E90(Elem52D60 *self);
void func_08A52EBC(Elem52D60 *self);
void func_08A52EE8(Elem52D60 *self);
void func_08A530D0(Elem52D60 *self);
void func_08A5318C(Elem52D60 *self);
void func_08A53F58(Elem52D60 *self);
void func_08A5417C(Elem52D60 *self);
Widget54438 *func_08A531E0(Widget54438 *self);
void func_08A5327C(Widget54438 *self);
void func_08A5352C(Widget54438 *self);
void func_08A535A4(Widget54438 *self);
void func_08A5360C(Widget54438 *self);
void func_08A537A4(Widget54438 *self);
void func_08A537C0(Widget54438 *self);
void func_08A538F4(Widget54438 *self);
void func_08A53B1C(Widget54438 *self);
void func_08A53C08(Widget54438 *self);
void func_08A53C5C(Widget54438 *self);
void func_08A53CA0(Widget54438 *self);
void func_08A53E00(Widget54438 *self);
Widget54E98 *func_08A546F0(Widget54E98 *self);
void func_08A54744(Widget54E98 *self);
void func_08A54890(Widget54E98 *self);
void func_08A548D4(Widget54E98 *self);
void func_08A54924(Widget54E98 *self);
void func_08A54940(Widget54E98 *self);
void func_08A5495C(Widget54E98 *self);
void func_08A54978(Widget54E98 *self);
void func_08A54994(Widget54E98 *self);
void func_08A549B0(Widget54E98 *self);
void func_08A54AE4(Widget54E98 *self);
void func_08A54C60(Widget54E98 *self);
void func_08A54CB4(Widget54E98 *self, int value);
void func_08A54E0C(Widget54E98 *self);
void func_08A5C390(void);                                           /* element constructors */
void func_08A5C1E8(void);
void func_08A5BFF0(void);

/* ------------------------------------------------------------ items */

Item *func_08A54F38(Item *self);
void func_08A54F70(Item *self, unsigned int flags);
void func_08A54FA4(Item *self);
void func_08A55044(Item *self);
void func_08A55070(Item *self);
void func_08A550A0(Item *self);
void func_08A550D8(Item *self);
void func_08A55114(Item *self);
void func_08A55174(Item *self);
void func_08A551FC(Item *self, int index);
void func_08A55244(Item *self);
void func_08A55284(Item *self);
void func_08A5550C(Item *self);
void func_08A5577C(Item *self);
void func_08A557D0(Item *self);
void func_08A5585C(Item *self);
void func_08A55878(Item *self);
void func_08A558D8(Item *self);
Item58380 *func_08A55A10(Item58380 *self);
void func_08A55A4C(Item58380 *self, unsigned int flags);
void func_08A55AA8(Item58380 *self);
void func_08A55B4C(Item58380 *self, short *out);
void func_08A55B58(Item58380 *self, int value);
Item583A0 *func_08A55EDC(Item583A0 *self);
void func_08A55F18(Item583A0 *self, unsigned int flags);
void func_08A55F74(Item583A0 *self);
void func_08A56008(Item583A0 *self, short *out);
void func_08A56014(Item583A0 *self, int arg1, int arg2, int arg3, int arg4);
void func_08A56028(Item583A0 *self);
void func_08A56058(Item583A0 *self, int value);
void func_08A5613C(Item583A0 *self, int value);
Item583C0 *func_08A5657C(Item583C0 *self);
void func_08A565B8(Item583C0 *self, unsigned int flags);
void func_08A56614(Item583C0 *self);
void func_08A566B8(Item583C0 *self, char *out1, char *out2, char *out3);
void func_08A566D4(Item583C0 *self, int value);
Item583E0 *func_08A56A50(Item583E0 *self);
void func_08A56A8C(Item583E0 *self, unsigned int flags);
void func_08A56AE8(Item583E0 *self);
void func_08A56B6C(Item583E0 *self, short *out);
void func_08A56B78(Item583E0 *self, int value);
void func_08A56CA8(Item583E0 *self);
Item58400 *func_08A56CD0(Item58400 *self);
void func_08A56D0C(Item58400 *self, unsigned int flags);
void func_08A56D68(Item58400 *self);
void func_08A56E30(Item58400 *self, short *out, char *data);
void func_08A56F60(Item58400 *self, int value, char *data);

/* ------------------------------------------------------------ Panel58420 */

int func_08A5738C(void *self, int value);
int func_08A573A0(void *self, int value);
Panel58420 *func_08A573B4(Panel58420 *self);
void func_08A57428(Panel58420 *self, unsigned int flags);
void func_08A574CC(Panel58420 *self);
void func_08A575A4(Panel58420 *self);
void func_08A57734(Panel58420 *self);
void func_08A5776C(Panel58420 *self);
void func_08A577A4(Panel58420 *self);
void func_08A577FC(Panel58420 *self);
void func_08A579A8(Panel58420 *self);
void func_08A57EB8(Panel58420 *self);
void func_08A57EF0(Panel58420 *self);
void func_08A57FC0(Panel58420 *self);
int func_08A580B0(Panel58420 *self);
void func_08A581F0(Panel58420 *self, Rec164 **record, int arg2, MenuCallback done, int arg4);

/* ------------------------------------------------------------ parts, Panel5B608 */

Part *func_08A58740(Part *self);
void func_08A58778(Part *self);
Part5B578 *func_08A58868(Part5B578 *self);
void func_08A588A4(Part5B578 *self);
void func_08A58C24(Part5B578 *self, int value);
void func_08A58C40(Part5B578 *self);
void func_08A58EF4(Part5B578 *self);
void func_08A58FD4(Part5B578 *self);
Part5B5C0 *func_08A59028(Part5B5C0 *self);
void func_08A59064(Part5B5C0 *self);
void func_08A5919C(Part5B5C0 *self);
int func_08A591D8(Part5B5C0 *self);
void func_08A591EC(Part5B5C0 *self);
void func_08A5923C(Part5B5C0 *self);
void func_08A59680(Part5B5C0 *self);
void func_08A597B0(Part5B5C0 *self);
Panel5B608 *func_08A59C5C(Panel5B608 *self);
void func_08A59CD0(Panel5B608 *self);
void func_08A59F18(Panel5B608 *self, Rec5B608 *record, int arg);
void func_08A59FB4(Panel5B608 *self);
void func_08A59FF4(Panel5B608 *self);
void func_08A5A47C(Panel5B608 *self, int arg);
void func_08A5A74C(Panel5B608 *self);
void func_08A5A778(Panel5B608 *self);
unsigned int func_08A5A82C(Panel5B608 *self, unsigned char arg1, unsigned char arg2);
void func_08A5AF08(Panel5B608 *self);
void func_08A5B060(Panel5B608 *self);
void func_08A5B220(Panel5B608 *self);
void func_08A5B38C(Panel5B608 *self);

/* ------------------------------------------------------------ Obj5B958 */

Obj5B958 *func_08A5B958(Obj5B958 *self);
void func_08A5B980(Obj5B958 *self);
void func_08A5B9A8(Obj5B958 *self, void *heap, int arg, Obj56C *data);
void func_08A5B9D0(Obj5B958 *self, char arg);
void func_08A5BB08(Obj5B958 *self);
void func_08A5BB34(Obj5B958 *self);
void func_08A5BB5C(Obj5B958 *self);
int func_08A5BBC8(Obj5B958 *self);
Obj56C *func_08A5BBD0(Obj5B958 *self);
unsigned char func_08A5BBD8(Obj5B958 *self);
void func_08A5BC98(void *block, MenuLayout *layout);
void func_08A5BCF0(MenuLayout *layout);
void func_08A5BE40(Obj5B958 *self);
void func_08A5BE78(Obj5B958 *self);

/* ------------------------------------------------------------ Cell5C5C8, Panel5DB80 */

void func_08A5C5C8(void);                                           /* cell constructor */
void func_08A5C5F0(Cell5C5C8 *self);
void func_08A5C64C(Cell5C5C8 *self, int arg1, int arg2, int arg3, int arg4, MenuModel *model,
                   int arg6, int arg7, void *messages);
void func_08A5C6B4(Cell5C5C8 *self);
void func_08A5C6E8(Cell5C5C8 *self);
void func_08A5C708(Cell5C5C8 *self);
void func_08A5C748(Cell5C5C8 *self);
void func_08A5C784(Cell5C5C8 *self);
void func_08A5C7A4(Cell5C5C8 *self);
int func_08A5C7C8(Cell5C5C8 *self);
int func_08A5C7D0(Cell5C5C8 *self);
void func_08A5C7D8(Cell5C5C8 *self);
void func_08A5C804(Cell5C5C8 *self);
void func_08A5C98C(Cell5C5C8 *self);
void func_08A5CA8C(Cell5C5C8 *self);
void func_08A5CAE0(Cell5C5C8 *self);
Panel5DB80 *func_08A5CBA4(Panel5DB80 *self);
void func_08A5CC00(Panel5DB80 *self);
void func_08A5CD9C(Panel5DB80 *self, Entry10 *entry, void *messages);
void func_08A5CE08(Panel5DB80 *self);
void func_08A5CE48(Panel5DB80 *self);
void func_08A5CE80(Panel5DB80 *self, void *messages);
void func_08A5D078(Panel5DB80 *self);
void func_08A5D460(Panel5DB80 *self);
void func_08A5D504(Panel5DB80 *self, void *messages);
void func_08A5D580(Panel5DB80 *self);
void func_08A5D5F4(Panel5DB80 *self);
int func_08A5D654(Panel5DB80 *self);
void func_08A5D678(Panel5DB80 *self);
void func_08A5D714(Panel5DB80 *self);
void func_08A5D754(Panel5DB80 *self, int index);
void func_08A5D95C(Panel5DB80 *self);
void func_08A5D9B8(Panel5DB80 *self);
void func_08A5D9D4(Panel5DB80 *self, Entry10 *entry);
void func_08A5DACC(Panel5DB80 *self, Entry10 *entry);

/* ------------------------------------------------------------ Panel5EED8, Panel5FA00, Panel60670 */

Panel5EED8 *func_08A5DE10(Panel5EED8 *self);
void func_08A5DE54(Panel5EED8 *self);
void func_08A5E13C(Panel5EED8 *self);
void func_08A5E148(Panel5EED8 *self);
void func_08A5E174(Panel5EED8 *self);
void func_08A5E180(Panel5EED8 *self);
void func_08A5E188(Panel5EED8 *self, int value);
void func_08A5E300(Panel5EED8 *self);
void func_08A5E5F4(Panel5EED8 *self);
void func_08A5E640(Panel5EED8 *self);
void func_08A5E680(Panel5EED8 *self);
void func_08A5E7B8(Panel5EED8 *self);
void func_08A5E7EC(Panel5EED8 *self, void *messages);
void func_08A5E848(Panel5EED8 *self);
void func_08A5E94C(Panel5EED8 *self, void *messages);
void func_08A5EA48(Panel5EED8 *self, int index);
void func_08A5ECCC(Panel5EED8 *self);
Panel5FA00 *func_08A5F030(Panel5FA00 *self);
void func_08A5F06C(Panel5FA00 *self);
void func_08A5F108(Panel5FA00 *self, int arg1, int arg2, int arg3);
void func_08A5F140(Panel5FA00 *self, int value);
void func_08A5F178(Panel5FA00 *self);
void func_08A5F26C(Panel5FA00 *self);
void func_08A5F364(Panel5FA00 *self);
void func_08A5F710(Panel5FA00 *self);
void func_08A5F7CC(Panel5FA00 *self);
void func_08A5F830(Panel5FA00 *self);
void func_08A5F8F8(Panel5FA00 *self);
void func_08A5F970(Panel5FA00 *self, int first, int count, int arg);
Panel60670 *func_08A601B0(Panel60670 *self);
void func_08A601EC(Panel60670 *self);
void func_08A602A4(Panel60670 *self, MenuModel *model, int arg2, void *messages, int count, int arg5);
void func_08A60310(Panel60670 *self, void *messages, int count, int arg);
void func_08A60488(Panel60670 *self);
void func_08A60570(Panel60670 *self);

/* ------------------------------------------------------------ Panel61C40, Panel63680 */

void func_08A6070C(void);
Panel61C40 *func_08A6072C(Panel61C40 *self);
void func_08A60768(Panel61C40 *self);
void func_08A6082C(Panel61C40 *self, Data6FFC *data, void *messages, unsigned char flag);
void func_08A608B8(Panel61C40 *self);
void func_08A60944(Panel61C40 *self);
void func_08A60950(Panel61C40 *self, unsigned char flag);
void func_08A60988(Panel61C40 *self);
int func_08A60A2C(Panel61C40 *self);
void func_08A60A34(Panel61C40 *self);
void func_08A60A9C(Panel61C40 *self);
void func_08A60AA8(Panel61C40 *self);
int func_08A60B30(Panel61C40 *self);
void func_08A60BEC(Panel61C40 *self);
void func_08A6108C(Panel61C40 *self, void *messages, unsigned char flag);
void func_08A610C8(Panel61C40 *self, unsigned char flag);
void func_08A61240(Panel61C40 *self);
void func_08A613E8(Panel61C40 *self);
void func_08A61468(Panel61C40 *self);
void func_08A615C4(Panel61C40 *self);
void func_08A61618(Panel61C40 *self);
void func_08A6171C(Panel61C40 *self);
void func_08A61EC0(void);
Panel63680 *func_08A61EE0(Panel63680 *self);
void func_08A61F44(Panel63680 *self);
void func_08A62094(Panel63680 *self, int arg1, int arg2, int arg3);
void func_08A620C0(Panel63680 *self);
void func_08A62128(Panel63680 *self, void *data, void *messages);
void func_08A62184(Panel63680 *self);
void func_08A621BC(Panel63680 *self);
void func_08A621C8(Panel63680 *self);
void func_08A621D4(Panel63680 *self);
void func_08A62204(Panel63680 *self);
void func_08A6225C(Panel63680 *self, int kind, unsigned char flag);
void func_08A622FC(Panel63680 *self, int value);
int func_08A6233C(Panel63680 *self);
void func_08A625C8(Panel63680 *self, void *messages);
void func_08A6282C(Panel63680 *self);
void func_08A628E0(Panel63680 *self);
void func_08A629BC(Panel63680 *self);
void func_08A62A3C(Panel63680 *self);
void func_08A62AFC(Panel63680 *self);
void func_08A62C58(Panel63680 *self);
void func_08A62CAC(Panel63680 *self);
void func_08A62D9C(Panel63680 *self);
void func_08A62E14(Panel63680 *self);
void func_08A62E5C(Panel63680 *self, int value);
void func_08A63024(Panel63680 *self);
void func_08A631EC(Panel63680 *self);
void func_08A6324C(Panel63680 *self);

/* ------------------------------------------------------------ pages, Panel68180 */

void func_08A63830(void);
void func_08A63850(void);
void func_08A63870(void);
void func_08A63890(void);
Page *func_08A638B0(Page *self);
void func_08A638C4(Page *self);
void func_08A63A40(Page *self, int value);
Page67EE0 *func_08A63A48(Page67EE0 *self);
void func_08A63A84(Page67EE0 *self);
void func_08A63C38(Page67EE0 *self);
void func_08A63D34(Page67EE0 *self);
void func_08A63D84(Page67EE0 *self);
void func_08A63FEC(Page67EE0 *self);
void func_08A64100(Page67EE0 *self);
Page67F88 *func_08A644A4(Page67F88 *self);
void func_08A644E0(Page67F88 *self);
void func_08A647BC(Page67F88 *self);
void func_08A64990(Page67F88 *self);
void func_08A64A58(Page67F88 *self);
Page68030 *func_08A64B84(Page68030 *self);
void func_08A64BC0(Page68030 *self);
void func_08A64FF4(Page68030 *self);
void func_08A652F0(Page68030 *self);
void func_08A653CC(Page68030 *self);
Page680D8 *func_08A65420(Page680D8 *self);
void func_08A6545C(Page680D8 *self);
int func_08A656FC(void);
void func_08A658C4(void);
void func_08A659A8(Page680D8 *self);
void func_08A65A28(Page680D8 *self, const void *src);
Entry10 *func_08A65A48(Page680D8 *self);
void func_08A65B24(Page680D8 *self);
void func_08A65CCC(Page680D8 *self);
void func_08A65ED8(Page680D8 *self);
void func_08A6610C(void);
Panel68180 *func_08A66218(Panel68180 *self);
void func_08A66294(Panel68180 *self);
void func_08A66478(Panel68180 *self, List66 *list);
void func_08A66644(Panel68180 *self);
void func_08A666B0(Panel68180 *self);
void func_08A66780(Panel68180 *self);
void func_08A667D8(Panel68180 *self);
void func_08A6681C(Panel68180 *self);
void func_08A668AC(Panel68180 *self);
void func_08A668E0(Panel68180 *self, int value);
void func_08A66A20(Panel68180 *self, int value);
List66 *func_08A66AA0(Panel68180 *self);
void func_08A66AA8(Panel68180 *self);
int func_08A66C80(Panel68180 *self);
void func_08A67004(Panel68180 *self);
void func_08A67540(Panel68180 *self);
int func_08A6772C(Panel68180 *self);
int func_08A67788(Panel68180 *self);

/* ------------------------------------------------------------ Panel694A8, Panel69A78, Panel6B170 */

Panel694A8 *func_08A68388(Panel694A8 *self);
void func_08A683C4(Panel694A8 *self);
void func_08A68494(Panel694A8 *self, int mode, int arg2, int arg3, int arg4, int arg5, int arg6,
                   int arg7, int arg8, int arg9);
void func_08A68544(Panel694A8 *self);
void func_08A68654(Panel694A8 *self);
void func_08A689EC(Panel694A8 *self);
void func_08A68AA8(Panel694A8 *self, int arg1, int arg2, int arg3, int arg4);
void func_08A68C34(Panel694A8 *self);
void func_08A68CD8(Panel694A8 *self);
void func_08A68D5C(Panel694A8 *self);
void func_08A69268(Panel694A8 *self);
Panel69A78 *func_08A69600(Panel69A78 *self);
void func_08A6963C(Panel69A78 *self);
void func_08A69704(Panel69A78 *self, int arg1, int arg2, int arg3);
void func_08A69714(Panel69A78 *self, void *messages, int arg);
void func_08A697B0(Panel69A78 *self, int arg1, int arg2);
void func_08A69830(Panel69A78 *self);
void func_08A698F4(Panel69A78 *self);
void func_08A69938(Panel69A78 *self);
void func_08A69A20(Panel69A78 *self);
Panel6B170 *func_08A69C20(Panel6B170 *self);
void func_08A69C5C(Panel6B170 *self, unsigned int flags);
void func_08A69CB8(Panel6B170 *self);
void func_08A69D94(Panel6B170 *self, int arg1, int arg2, int arg3);
void func_08A69DF4(Panel6B170 *self);
void func_08A69FE8(Panel6B170 *self);
int func_08A69FF4(Panel6B170 *self);
int func_08A69FFC(Panel6B170 *self);
int func_08A6A004(Panel6B170 *self);
void func_08A6A0F4(Panel6B170 *self);
void func_08A6A20C(Panel6B170 *self);
void func_08A6A260(Panel6B170 *self);

/* ------------------------------------------------------------ Panel6C638 */

Panel6C638 *func_08A6B418(Panel6C638 *self);
void func_08A6B45C(Panel6C638 *self, unsigned int flags);
void func_08A6B4C4(Panel6C638 *self);
void func_08A6B600(Panel6C638 *self);
void func_08A6B630(Panel6C638 *self, Data6C638 **data);
void func_08A6B638(Panel6C638 *self, int value);
int func_08A6B728(Panel6C638 *self);
char *func_08A6B730(Panel6C638 *self);
void func_08A6B8B0(Panel6C638 *self);
void func_08A6B8BC(Panel6C638 *self);
void func_08A6B8C8(Panel6C638 *self);
void func_08A6BABC(Panel6C638 *self);
void func_08A6BB60(Panel6C638 *self, int value);
void func_08A6BDBC(Panel6C638 *self);
void func_08A6BDEC(Panel6C638 *self);
void func_08A6BED0(Panel6C638 *self);
void func_08A6C03C(Panel6C638 *self);
void func_08A6C144(Panel6C638 *self);
void func_08A6C2F8(Panel6C638 *self, int index);
void func_08A6C374(void);
void func_08A6C3A0(Panel6C638 *self);
void func_08A6C3EC(Panel6C638 *self);
extern Panel6C638 *D_08A6CBC0;

/* ------------------------------------------------------------ Screen6D710 */

void func_08A6CBE0(Screen6D710 *self);
void func_08A6CCA4(Screen6D710 *self);
void func_08A6CF2C(Screen6D710 *self);
void func_08A6CF8C(Screen6D710 *self);
void func_08A6CFA8(Screen6D710 *self);
void func_08A6D050(Screen6D710 *self);
void func_08A6D0D4(Screen6D710 *self);
void func_08A6D100(Screen6D710 *self);
void func_08A6D12C(Screen6D710 *self, int value);
void func_08A6D158(Screen6D710 *self);
void func_08A6D184(Screen6D710 *self);
void func_08A6D1B0(Screen6D710 *self);
void func_08A6D284(void);
void func_08A6D2A4(void);
void func_08A6D2C4(void);
void func_08A6D3A8(Screen6D710 *self);
void func_08A6D2E4(Delegate6D710 *self);
void func_08A6D300(Delegate6D710 *self);
void func_08A6D31C(Delegate6D710 *self);
void func_08A6D338(Delegate6D710 *self, int value);
void func_08A6D354(Delegate6D710 *self);
void func_08A6D370(Delegate6D710 *self);
void func_08A6D38C(Delegate6D38C *self);

/* ------------------------------------------------------------ states, Obj6ECF4 */

Pair8 *func_08A6D7CC(Pair8 *self);
void func_08A6D7E8(State6D7E8 *self, unsigned int flags);           /* base destructor */
void func_08A6DA70(State6D7E8 *self, unsigned int flags);
void func_08A6DB50(State6D7E8 *self, Obj6ECF4 *owner, int arg);
void func_08A6DBA4(State6D7E8 *self, unsigned int flags);
void func_08A6DC54(State6D7E8 *self, Obj6ECF4 *owner);
void func_08A6DC90(State6D7E8 *self, unsigned int flags);
void func_08A6DFB8(State6D7E8 *self, unsigned int flags);
void func_08A6E0AC(State6D7E8 *self, Obj6ECF4 *owner);
void func_08A6E0DC(State6D7E8 *self, unsigned int flags);
Request6E168 *func_08A6E168(Request6E168 *self);
void func_08A6E1C4(Request6E168 *self);
void func_08A6E1F4(Request6E168 *self, Obj6ECF4 *owner);
void func_08A6E2CC(Request6E168 *self, int kind, const void *data, int size);
void func_08A6E388(Request6E168 *self);
void func_08A6E450(void *object, int arg);
void func_08A6E750(State6D7E8 *self, Obj6ECF4 *owner);
void func_08A6E7E4(State6D7E8 *self, unsigned int flags);
void func_08A6E880(State6D7E8 *self, Obj6ECF4 *owner);
void func_08A6E928(State6D7E8 *self, unsigned int flags);
void func_08A6EA68(State6D7E8 *self, unsigned int flags);
void func_08A6EAF8(void);                                           /* element constructor */
Obj6EB20 *func_08A6EB20(Obj6EB20 *self);
void func_08A6ECF4(Obj6ECF4 *self);
void func_08A6EEEC(Obj6ECF4 *self);
void func_08A6EF38(Obj6ECF4 *self);
void func_08A6EF6C(Obj6ECF4 *self);
void func_08A6F03C(Obj6ECF4 *self);
void func_08A6F1E0(Obj6ECF4 *self);
void func_08A6F23C(Obj6ECF4 *self);
void func_08A6F298(Obj6ECF4 *self);
void func_08A6F2F4(Obj6ECF4 *self);
void func_08A6F350(Obj6ECF4 *self);
void func_08A6F3E0(Obj6ECF4 *self);
void func_08A6F448(Obj6ECF4 *self);
void func_08A6F4BC(Obj6ECF4 *self);
void func_08A6F518(Obj6ECF4 *self);
void func_08A6F580(Obj6ECF4 *self);
void func_08A6F5E8(Obj6ECF4 *self);
void func_08A6F610(Delegate6F610 *self);
void func_08A6F62C(Delegate6F610 *self);
void func_08A6F658(Delegate6F610 *self);
void func_08A6F674(Delegate6F610 *self);
void func_08A6F690(Delegate6F610 *self);
void func_08A6F6BC(Delegate6F610 *self);
void func_08A6F6D8(Delegate6F610 *self);
void func_08A6F6F4(Delegate6F610 *self);
void func_08A6F718(Delegate6F610 *self);
void func_08A6F734(Delegate6F610 *self);
void func_08A6F750(Delegate6F610 *self);
void func_08A6FEC8(void *owner);
void func_08A6FF28(void *owner);
void func_08A6FF6C(Delegate6FF6C *self);
void func_08A6FF88(Delegate6FF6C *self);
void func_08A6FFC8(void);
void func_08A6FFD8(void *self, void *object, int kind, void *data, int size);
void func_08A70270(void *self, Obj70270 *owner);
void func_08A702D0(Obj708C8 *self);
void func_08A7058C(Obj7058C *self);
void func_08A7059C(void *block);
void func_08A70798(Bytes6 *self);
void func_08A707BC(Bytes6 *self);
void func_08A7084C(Obj7084C *self);
void func_08A7085C(void *block);
void func_08A708C8(Obj708C8 *self);
void func_08A70904(Obj708C8 *self);

#ifdef __cplusplus
}
#endif

#endif
