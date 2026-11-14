#ifndef DISSIDIA_MENU_AUTO_REGION3_H
#define DISSIDIA_MENU_AUTO_REGION3_H

/* Classes behind the automatically matched functions in menu
 * 0x08A70948..0x08AF7124 (readability pass, menu region 3).
 *
 * The range is a long stretch of menu screens and their small parts. Most
 * of the parts are built from layout nodes and texts (LayoutNode,
 * LayoutText below): a node is a model handle placed in a layout and
 * driven by the func_088D2E70..func_088D3450 family, a text is drawn on
 * one of a node's elements. Screens follow the region-1 pattern: a block
 * and a MenuTask, a heap, and a pointer to a work block that holds the
 * layout, model handles, texts, the messages object and a MenuPrompt.
 * Step classes (vtable at 0, base destructor func_08A775D8) are the states
 * of a menu flow whose owner keeps a StateQueue at +0x1B0.
 *
 * Structs are named after the singleton pointer or the vtable of their
 * class where one is known, otherwise after their constructor or first
 * function here. Fields whose use is not plain keep their offset as their
 * name. Types and prototypes of auto_region1.h / auto_region2.h are reused. */

#include "menu/auto_region2.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MENU3_OFFSET(type, field) ((unsigned int)&((type *)0)->field)
#define MENU3_ASSERT(name, cond) typedef char menu3_assert_##name[(cond) ? 1 : -1]
#define MENU3_SIZE(type, size) MENU3_ASSERT(size_##type, sizeof(type) == (size))
#define MENU3_AT(type, field, offset) \
    MENU3_ASSERT(at_##type##_##field, MENU3_OFFSET(type, field) == (offset))

/* ------------------------------------------------------------ layout nodes and texts */

/* Text drawn on a layout node (what LayoutText points to). */
typedef struct LayoutTextObj {
    char pad_00[0x10];
    unsigned int flags;
} LayoutTextObj;
MENU3_AT(LayoutTextObj, flags, 0x10);

/* A layout node: a model handle placed in a layout. func_088D2E70 builds it,
 * func_088D2F30 releases it, func_088D2EC8 binds it to a layout entry and
 * func_088D2FB8 to an element of a parent node; func_088D2F68 plays a motion,
 * func_088D2FA0 queues the next one, func_088D3174 / func_088D319C show and
 * hide it. model->field_20 & 0x20 is set while a motion plays. */
typedef struct LayoutNode {
    MenuModel *model;
} LayoutNode;
MENU3_SIZE(LayoutNode, 0x4);
MENU3_AT(LayoutNode, model, 0x0);

/* A text on a node: func_088D31C4 builds it, func_088D3334 releases it,
 * func_088D3450 binds it to an element of a node, func_088D3278 /
 * func_088D321C set what it shows, func_088D32F8 copies another text.
 * func_088D30E4 / func_088D3068 open and close a node for text changes. */
typedef struct LayoutText {
    LayoutTextObj *obj;
} LayoutText;
MENU3_SIZE(LayoutText, 0x4);
MENU3_AT(LayoutText, obj, 0x0);

/* Two nodes and a text (constructor func_08A76574's first members). */
typedef struct NodeText {
    LayoutNode node;
    LayoutText text;
} NodeText;
MENU3_SIZE(NodeText, 0x8);
MENU3_AT(NodeText, node, 0x0);
MENU3_AT(NodeText, text, 0x4);

/* ------------------------------------------------------------ 0x08A70948..0x08A77968: parts */

typedef struct Obj70948 {
    char pad_00[0x4];
    int field_04;
    int field_08;
} Obj70948;
MENU3_AT(Obj70948, field_04, 0x4);
MENU3_AT(Obj70948, field_08, 0x8);

/* A label: three texts on nodes 0x0C and 0x04 (func_08A7259C binds them). */
typedef struct Label7259C {
    char pad_00[0x4];
    LayoutNode field_04;
    LayoutNode field_08;
    LayoutNode field_0C;
    char pad_10[0x4];
    LayoutNode field_14;
    LayoutNode field_18;
    LayoutText field_1C;
    LayoutText field_20; /* copy of field_1C */
    LayoutText field_24;
    int state; /* 1 while field_0C is shown */
} Label7259C;
MENU3_AT(Label7259C, field_04, 0x4);
MENU3_AT(Label7259C, field_08, 0x8);
MENU3_AT(Label7259C, field_0C, 0xC);
MENU3_AT(Label7259C, field_14, 0x14);
MENU3_AT(Label7259C, field_18, 0x18);
MENU3_AT(Label7259C, field_1C, 0x1C);
MENU3_AT(Label7259C, field_20, 0x20);
MENU3_AT(Label7259C, field_24, 0x24);
MENU3_AT(Label7259C, state, 0x28);

/* Layout of the hand-written func_08A72ABC (stage_start_08a72abc.c, A6Stage). */
typedef struct Stage72ABC {
    char pad_00[0x8];
    LayoutNode field_08;
    LayoutNode field_0C;
    LayoutNode field_10;
    char pad_14[0x8];
    unsigned int state;
    char pad_20[0x4];
    unsigned char field_24;
} Stage72ABC;
MENU3_AT(Stage72ABC, field_08, 0x8);
MENU3_AT(Stage72ABC, field_0C, 0xC);
MENU3_AT(Stage72ABC, field_10, 0x10);
MENU3_AT(Stage72ABC, state, 0x1C);
MENU3_AT(Stage72ABC, field_24, 0x24);

/* Six nodes and a text (0x1C; a member at +0x14 of the Step77C34 steps). */
typedef struct Icons72ED0 {
    LayoutNode field_00;
    LayoutNode field_04;
    LayoutNode field_08;
    LayoutNode field_0C;
    LayoutNode field_10;
    LayoutNode field_14;
    LayoutText field_18;
} Icons72ED0;
MENU3_SIZE(Icons72ED0, 0x1C);
MENU3_AT(Icons72ED0, field_00, 0x0);
MENU3_AT(Icons72ED0, field_04, 0x4);
MENU3_AT(Icons72ED0, field_08, 0x8);
MENU3_AT(Icons72ED0, field_0C, 0xC);
MENU3_AT(Icons72ED0, field_10, 0x10);
MENU3_AT(Icons72ED0, field_14, 0x14);
MENU3_AT(Icons72ED0, field_18, 0x18);

/* Five nodes and four texts on node field_10. */
typedef struct Box73E44 {
    LayoutNode field_00;
    LayoutNode field_04;
    LayoutNode field_08;
    LayoutNode field_0C;
    LayoutNode field_10;
    LayoutText field_14;
    LayoutText field_18;
    LayoutText field_1C;
    LayoutText field_20;
} Box73E44;
MENU3_AT(Box73E44, field_00, 0x0);
MENU3_AT(Box73E44, field_04, 0x4);
MENU3_AT(Box73E44, field_08, 0x8);
MENU3_AT(Box73E44, field_0C, 0xC);
MENU3_AT(Box73E44, field_10, 0x10);
MENU3_AT(Box73E44, field_14, 0x14);
MENU3_AT(Box73E44, field_18, 0x18);
MENU3_AT(Box73E44, field_1C, 0x1C);
MENU3_AT(Box73E44, field_20, 0x20);

/* A button: three nodes and a text (0x10; a member at +4 of the steps). */
typedef struct Btn74390 {
    LayoutNode field_00;
    LayoutNode field_04;
    LayoutNode field_08;
    LayoutText field_0C;
} Btn74390;
MENU3_SIZE(Btn74390, 0x10);
MENU3_AT(Btn74390, field_00, 0x0);
MENU3_AT(Btn74390, field_04, 0x4);
MENU3_AT(Btn74390, field_08, 0x8);
MENU3_AT(Btn74390, field_0C, 0xC);

typedef struct Obj74588 {
    unsigned char field_00[0x34]; /* cleared by func_08A74588 */
    unsigned char field_34[4]; /* built by func_0887B1E4 (size not known) */
} Obj74588;
MENU3_AT(Obj74588, field_00, 0x0);
MENU3_AT(Obj74588, field_34, 0x34);

/* Element built by func_08A74B28 (written by func_08A74CBC). */
typedef struct Row74B28 {
    char pad_00[0x14];
} Row74B28;
MENU3_SIZE(Row74B28, 0x14);

/* Three Row74B28 rows (func_08A74E80). */
typedef struct Rows74E80 {
    Row74B28 rows[3];
} Rows74E80;
MENU3_SIZE(Rows74E80, 0x3C);
MENU3_AT(Rows74E80, rows, 0x0);

typedef struct Obj7525C {
    LayoutNode field_00;
    LayoutNode field_04;
    LayoutNode field_08;
    LayoutNode field_0C;
    int field_10;
} Obj7525C;
MENU3_AT(Obj7525C, field_00, 0x0);
MENU3_AT(Obj7525C, field_04, 0x4);
MENU3_AT(Obj7525C, field_08, 0x8);
MENU3_AT(Obj7525C, field_0C, 0xC);
MENU3_AT(Obj7525C, field_10, 0x10);

/* Element built by func_08A75B78. */
typedef struct Row75B78 {
    char pad_00[0x14];
} Row75B78;
MENU3_SIZE(Row75B78, 0x14);

/* Element built by func_08A75D14 and released by func_08A75E7C. */
typedef struct Row75D14 {
    char pad_00[0x7C];
} Row75D14;
MENU3_SIZE(Row75D14, 0x7C);

/* A node and ten rows (constructor func_08A76158). */
typedef struct List76158 {
    LayoutNode node;
    Row75B78 rows[10];
} List76158;
MENU3_SIZE(List76158, 0xCC);
MENU3_AT(List76158, node, 0x0);
MENU3_AT(List76158, rows, 0x4);

/* Built by func_08A76344, released by func_08A763A4. */
typedef struct Obj76344 {
    char pad_00[0xC];
} Obj76344;
MENU3_SIZE(Obj76344, 0xC);

/* Constructor func_08A76574, destructor func_08A76710. */
typedef struct Page76574 {
    LayoutNode field_00;
    LayoutText field_04;
    Row75D14 rows[2];
    List76158 list;
    Obj76344 field_1CC;
    LayoutNode field_1D8;
    int field_1DC;
} Page76574;
MENU3_SIZE(Page76574, 0x1E0);
MENU3_AT(Page76574, field_00, 0x0);
MENU3_AT(Page76574, field_04, 0x4);
MENU3_AT(Page76574, rows, 0x8);
MENU3_AT(Page76574, list, 0x100);
MENU3_AT(Page76574, field_1CC, 0x1CC);
MENU3_AT(Page76574, field_1D8, 0x1D8);
MENU3_AT(Page76574, field_1DC, 0x1DC);

typedef struct Src76384 {
    char pad_00[0x10];
    void *field_10;
} Src76384;
MENU3_AT(Src76384, field_10, 0x10);

typedef struct Pair76938 {
    LayoutNode field_00;
    LayoutNode field_04;
} Pair76938;
MENU3_AT(Pair76938, field_00, 0x0);
MENU3_AT(Pair76938, field_04, 0x4);

typedef struct Obj76BF8 {
    LayoutNode node;
    char pad_04[0x14];
    int shown;
} Obj76BF8;
MENU3_AT(Obj76BF8, node, 0x0);
MENU3_AT(Obj76BF8, shown, 0x18);

/* Element built by func_08A768E0. */
typedef struct Row768E0 {
    char pad_00[0x20];
} Row768E0;
MENU3_SIZE(Row768E0, 0x20);

typedef struct Rows76C9C {
    Row768E0 rows[3];
    int field_60;
    int field_64;
} Rows76C9C;
MENU3_AT(Rows76C9C, rows, 0x0);
MENU3_AT(Rows76C9C, field_60, 0x60);
MENU3_AT(Rows76C9C, field_64, 0x64);

/* Two nodes (a member at +4 of the Step7ADD8 / StepBtn80 steps). */
typedef struct Btn77168 {
    LayoutNode field_00;
    LayoutNode field_04;
} Btn77168;
MENU3_SIZE(Btn77168, 0x8);
MENU3_AT(Btn77168, field_00, 0x0);
MENU3_AT(Btn77168, field_04, 0x4);

typedef struct Obj772D0 {
    char pad_00[0x28];
    int field_28;
    int field_2C;
} Obj772D0;
MENU3_AT(Obj772D0, field_28, 0x28);
MENU3_AT(Obj772D0, field_2C, 0x2C);

/* ------------------------------------------------------------ 0x08A77848..0x08A7EB18: flow steps */

/* One slot of a StateQueue (hand-written func_08A79F60 clears it to 0, -1, -1, 0). */
typedef struct StateSlot {
    int field_00;
    int field_04;
    int next; /* the state to enter (func_08A79E68) */
    unsigned char field_0C;
    char pad_0D[0x3];
} StateSlot;
MENU3_SIZE(StateSlot, 0x10);
MENU3_AT(StateSlot, field_00, 0x0);
MENU3_AT(StateSlot, field_04, 0x4);
MENU3_AT(StateSlot, next, 0x8);
MENU3_AT(StateSlot, field_0C, 0xC);

/* State queue of a flow owner (constructor func_08A79C30). */
typedef struct StateQueue {
    int field_00;
    StateSlot slots[8];
    int index;
} StateQueue;
MENU3_SIZE(StateQueue, 0x88);
MENU3_AT(StateQueue, field_00, 0x0);
MENU3_AT(StateQueue, slots, 0x4);
MENU3_AT(StateQueue, index, 0x84);

/* The owner the steps act on (second argument of their callbacks). */
typedef struct StepOwner {
    char pad_00[0x1B0];
    StateQueue queue;
    void *field_238; /* handed to func_08A6F170 */
    char pad_23C[0x24];
    int field_260; /* a dialog kind for func_088B1194 */
    char pad_264[0x10];
    int field_274;
} StepOwner;
MENU3_AT(StepOwner, queue, 0x1B0);
MENU3_AT(StepOwner, field_238, 0x238);
MENU3_AT(StepOwner, field_260, 0x260);
MENU3_AT(StepOwner, field_274, 0x274);

/* A step: vtable at 0; base destructor func_08A775D8. */
typedef struct Step {
    void *vtable;
} Step;
MENU3_AT(Step, vtable, 0x0);

/* Steps with a button at +4 and icons at +0x14. */
typedef struct Step77C34 {
    void *vtable;
    Btn74390 field_04;
    Icons72ED0 field_14;
    int state;
    int next;
} Step77C34;
MENU3_AT(Step77C34, vtable, 0x0);
MENU3_AT(Step77C34, field_04, 0x4);
MENU3_AT(Step77C34, field_14, 0x14);
MENU3_AT(Step77C34, state, 0x30);
MENU3_AT(Step77C34, next, 0x34);

/* A step that waits for a dialog (func_088B1194 calls back with it). */
typedef struct StepWait {
    void *vtable;
    unsigned char done;
} StepWait;
MENU3_AT(StepWait, vtable, 0x0);
MENU3_AT(StepWait, done, 0x4);

typedef struct Step79858 {
    void *vtable;
    int state;
} Step79858;
MENU3_AT(Step79858, vtable, 0x0);
MENU3_AT(Step79858, state, 0x4);

typedef struct Step7A844 {
    void *vtable;
    Btn74390 field_04;
} Step7A844;
MENU3_AT(Step7A844, vtable, 0x0);
MENU3_AT(Step7A844, field_04, 0x4);

typedef struct Step7ADD8 {
    void *vtable;
    Btn77168 field_04;
    char pad_0C[0xAC];
    int state;
} Step7ADD8;
MENU3_AT(Step7ADD8, vtable, 0x0);
MENU3_AT(Step7ADD8, field_04, 0x4);
MENU3_AT(Step7ADD8, state, 0xB8);

/* Steps with two nodes at +4 and a state at +0x80. */
typedef struct StepBtn80 {
    void *vtable;
    Btn77168 field_04;
    char pad_0C[0x74];
    int state;
} StepBtn80;
MENU3_AT(StepBtn80, vtable, 0x0);
MENU3_AT(StepBtn80, field_04, 0x4);
MENU3_AT(StepBtn80, state, 0x80);

typedef struct Step7D0A0 {
    void *vtable;
    char pad_04[0x218];
    int field_21C;
    char pad_220[0x8];
    int state;
} Step7D0A0;
MENU3_AT(Step7D0A0, vtable, 0x0);
MENU3_AT(Step7D0A0, field_21C, 0x21C);
MENU3_AT(Step7D0A0, state, 0x228);

typedef struct Step7E7EC {
    void *vtable;
    Btn74390 field_04;
    Icons72ED0 field_14;
    char pad_30[0x38];
    int state;
} Step7E7EC;
MENU3_AT(Step7E7EC, vtable, 0x0);
MENU3_AT(Step7E7EC, field_04, 0x4);
MENU3_AT(Step7E7EC, field_14, 0x14);
MENU3_AT(Step7E7EC, state, 0x68);

/* A delegate holding a step (forwards a callback to it). */
typedef struct StepLink {
    void *vtable;
    void *step;
} StepLink;
MENU3_AT(StepLink, vtable, 0x0);
MENU3_AT(StepLink, step, 0x4);

/* Element built by func_08A7A0D0 (func_089057BC). */
typedef struct Elem7A0D0 {
    char pad_00[0xF0];
} Elem7A0D0;
MENU3_SIZE(Elem7A0D0, 0xF0);

/* ------------------------------------------------------------ 0x08A7EEDC..0x08A812FC */

typedef struct Entry67C {
    short id;
    char pad_02[0x6];
} Entry67C;
MENU3_SIZE(Entry67C, 0x8);
MENU3_AT(Entry67C, id, 0x0);

/* Work block of Screen7EEDC (+0x28). */
typedef struct Work7EEDC {
    int mode;
    char pad_04[0x40C];
    MenuModel *field_410;
    MenuModel *field_414;
    MenuModel *field_418;
    MenuModel *field_41C;
    MenuModel *field_420;
    MenuModel *field_424;
    char pad_428[0x16C];
    void *field_594; /* a text */
    char pad_598[0x4C];
    int field_5E4;
    unsigned int flags;
    char pad_5EC[0x10];
    unsigned int field_5FC;
    unsigned int field_600;
    int field_604;
    int field_608;
    int field_60C;
    char pad_610[0x10];
    int field_620;
    char pad_624[0x58];
    Entry67C entries[0x28];
} Work7EEDC;
MENU3_AT(Work7EEDC, mode, 0x0);
MENU3_AT(Work7EEDC, field_410, 0x410);
MENU3_AT(Work7EEDC, field_414, 0x414);
MENU3_AT(Work7EEDC, field_418, 0x418);
MENU3_AT(Work7EEDC, field_41C, 0x41C);
MENU3_AT(Work7EEDC, field_420, 0x420);
MENU3_AT(Work7EEDC, field_424, 0x424);
MENU3_AT(Work7EEDC, field_594, 0x594);
MENU3_AT(Work7EEDC, field_5E4, 0x5E4);
MENU3_AT(Work7EEDC, flags, 0x5E8);
MENU3_AT(Work7EEDC, field_5FC, 0x5FC);
MENU3_AT(Work7EEDC, field_600, 0x600);
MENU3_AT(Work7EEDC, field_604, 0x604);
MENU3_AT(Work7EEDC, field_608, 0x608);
MENU3_AT(Work7EEDC, field_60C, 0x60C);
MENU3_AT(Work7EEDC, field_620, 0x620);
MENU3_AT(Work7EEDC, entries, 0x67C);

typedef struct Screen7EEDC {
    char pad_00[0x28];
    Work7EEDC *work;
} Screen7EEDC;
MENU3_AT(Screen7EEDC, work, 0x28);

typedef struct Obj80D24 {
    char pad_00[0x14];
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
} Obj80D24;
MENU3_AT(Obj80D24, field_14, 0x14);
MENU3_AT(Obj80D24, field_18, 0x18);
MENU3_AT(Obj80D24, field_1C, 0x1C);
MENU3_AT(Obj80D24, field_20, 0x20);
MENU3_AT(Obj80D24, field_24, 0x24);

/* ------------------------------------------------------------ 0x08A86A2C..0x08A8A194: task screens */

/* Work block of the screen in D_08A87EAC. */
typedef struct Work87EAC {
    MenuLayout layout;
    MenuModel *handles[2];
    char pad_408[0x47C];
    void *messages;
    char pad_888[0x94];
    void *field_91C; /* a block from the screen's heap */
    void *field_920; /* deleted with func_08A80D40 */
    char pad_924[0x168];
    void *texts[6];
    MenuPrompt prompt;
} Work87EAC;
MENU3_AT(Work87EAC, layout, 0x0);
MENU3_AT(Work87EAC, handles, 0x400);
MENU3_AT(Work87EAC, messages, 0x884);
MENU3_AT(Work87EAC, field_91C, 0x91C);
MENU3_AT(Work87EAC, field_920, 0x920);
MENU3_AT(Work87EAC, texts, 0xA8C);
MENU3_AT(Work87EAC, prompt, 0xAA4);

/* Screen in D_08A87EAC: a 0x100-byte block for its task, the task, a heap. */
typedef struct Screen87EAC {
    unsigned char block[0x100];
    MenuTask task;
    void *field_134; /* released with func_0888208C */
    void *heap;
    char pad_13C[0x10];
    Work87EAC *work;
} Screen87EAC;
MENU3_AT(Screen87EAC, block, 0x0);
MENU3_AT(Screen87EAC, task, 0x100);
MENU3_AT(Screen87EAC, field_134, 0x134);
MENU3_AT(Screen87EAC, heap, 0x138);
MENU3_AT(Screen87EAC, work, 0x14C);

typedef struct Work8B7A8 {
    char pad_00[0x418];
    int field_418;
    char pad_41C[0x8];
    int mode;
    int field_428;
} Work8B7A8;
MENU3_AT(Work8B7A8, field_418, 0x418);
MENU3_AT(Work8B7A8, mode, 0x424);
MENU3_AT(Work8B7A8, field_428, 0x428);

typedef struct Ref8A068 {
    char pad_00[0x4];
    int id;
} Ref8A068;
MENU3_AT(Ref8A068, id, 0x4);

typedef struct Link8A068 {
    Ref8A068 *ref;
} Link8A068;
MENU3_AT(Link8A068, ref, 0x0);

/* Screen in D_08A8B7A8. */
typedef struct Screen8B7A8 {
    unsigned char block[0x100];
    MenuTask task;
    void *field_134; /* released with func_0888208C */
    void *heap;
    void *field_13C; /* a block from D_089D5EC0's heap */
    void *field_140; /* a heap */
    char pad_144[0x10];
    void *field_154;
    Link8A068 *field_158;
    char pad_15C[0x1A8];
    int field_304;
    int field_308;
    Work8B7A8 *work;
} Screen8B7A8;
MENU3_AT(Screen8B7A8, block, 0x0);
MENU3_AT(Screen8B7A8, task, 0x100);
MENU3_AT(Screen8B7A8, field_134, 0x134);
MENU3_AT(Screen8B7A8, heap, 0x138);
MENU3_AT(Screen8B7A8, field_13C, 0x13C);
MENU3_AT(Screen8B7A8, field_140, 0x140);
MENU3_AT(Screen8B7A8, field_154, 0x154);
MENU3_AT(Screen8B7A8, field_158, 0x158);
MENU3_AT(Screen8B7A8, field_304, 0x304);
MENU3_AT(Screen8B7A8, field_308, 0x308);
MENU3_AT(Screen8B7A8, work, 0x30C);

typedef struct Obj88584 {
    char pad_00[0x4];
    int field_04;
} Obj88584;
MENU3_AT(Obj88584, field_04, 0x4);

/* layout nodes and texts */
void func_088D2E70(LayoutNode *node);
void func_088D2EC8(LayoutNode *node, void *source, int index);
void func_088D2F30(LayoutNode *node);
void func_088D2F68(LayoutNode *node, int motion);
void func_088D2FA0(LayoutNode *node, int motion);
void func_088D2FB8(LayoutNode *node, LayoutNode *parent, int index, unsigned int flags);
void func_088D3044(LayoutNode *node);
void func_088D3068(LayoutNode *node, LayoutText *text);
void func_088D30E4(LayoutNode *node, LayoutText *text);
void func_088D3174(LayoutNode *node);
void func_088D319C(LayoutNode *node);
void func_088D31C4(LayoutText *text);
void func_088D321C(LayoutText *text, int value);
void func_088D3278(LayoutText *text, int arg1, int arg2, int arg3, int arg4, int arg5);
void func_088D32F8(LayoutText *text, LayoutText *source);
void func_088D3334(LayoutText *text);
void func_088D3374(LayoutText *text, int arg1, int arg2);
void func_088D3450(LayoutText *text, LayoutNode *node, int index);

/* other callees */
void *func_0887B1E4(void *object);
void func_0887B924(void *text);
void func_089347E8(char *buffer, const char *format, ...); /* sprintf */
void func_0899372C(void *array, int count, int size, void *dtor, int arg1, int arg2); /* destroy each element */
void func_089057BC(void *object);
void func_088F69F8(void);
unsigned char func_08867894(void);
int func_08B2BB8C(int id);
void func_08A6F170(void *object, int arg1, int arg2);
void func_08A6FB68(void);

/* 0x08A70948..0x08A77968 */
void func_08A70948(Obj70948 *self);
void func_08A7259C(Label7259C *self);
void func_08A7262C(Label7259C *self, int arg1, int arg2, int arg3, int arg4, int arg5);
void func_08A72760(Label7259C *self);
void func_08A72800(Label7259C *self);
void func_08A72B38(Stage72ABC *self);
int func_08A72C04(Stage72ABC *self);
unsigned char func_08A72C24(Stage72ABC *self);
void func_08A72C58(Stage72ABC *self);
void func_08A72E5C(Icons72ED0 *self);
void func_08A72ED0(Icons72ED0 *self);
void func_08A72F5C(Icons72ED0 *self);
void func_08A72FB4(Icons72ED0 *self);
unsigned char func_08A7304C(Icons72ED0 *self);
LayoutNode *func_08A73C38(LayoutNode *self);
void func_08A73C60(LayoutNode *self, void *source, int index);
void func_08A73C7C(LayoutNode *self);
void func_08A73E0C(Box73E44 *self);
void func_08A73E28(Box73E44 *self);
void func_08A73E44(Box73E44 *self, void *source, int index);
void func_08A74334(Btn74390 *self);
void func_08A74390(Btn74390 *self);
void func_08A743E0(Btn74390 *self);
void func_08A74414(Btn74390 *self);
void func_08A74454(Btn74390 *self, int value);
void func_08A744BC(Btn74390 *self, int arg1, int arg2, int arg3, int arg4, int arg5);
unsigned char func_08A74564(Btn74390 *self);
Obj74588 *func_08A74588(Obj74588 *self);
Row74B28 *func_08A74B28(Row74B28 *self);
void func_08A74B8C(LayoutNode *self, void *source);
void func_08A74CA0(LayoutNode *self, int motion);
void func_08A74CBC(Row74B28 *self, const char *text, int arg1, int arg2);
Rows74E80 *func_08A74E80(Rows74E80 *self);
void func_08A750B4(Rows74E80 *self, const char *text);
void func_08A750D4(Rows74E80 *self, const char *text);
void func_08A750F8(Rows74E80 *self, int value);
void func_08A7525C(Obj7525C *self, void *source, int value);
void func_08A75A1C(Obj7525C *self);
void func_08A75A38(Obj7525C *self);
Row75B78 *func_08A75B78(Row75B78 *self);
void func_08A75BFC(NodeText *self);
void func_08A75CE4(NodeText *self);
Row75D14 *func_08A75D14(Row75D14 *self);
void func_08A75E7C(Row75D14 *self);
List76158 *func_08A76158(List76158 *self);
void func_08A76214(List76158 *self);
Obj76344 *func_08A76344(Obj76344 *self);
void func_08A76384(LayoutNode *self, Src76384 *source);
void func_08A763A4(Obj76344 *self);
void func_08A76468(LayoutNode *self);
void func_08A7649C(LayoutNode *self);
Page76574 *func_08A76574(Page76574 *self);
void func_08A76710(Page76574 *self);
Row768E0 *func_08A768E0(Row768E0 *self);
void func_08A76938(Pair76938 *self, void *source);
void func_08A76BF8(Obj76BF8 *self, int shown);
Rows76C9C *func_08A76C9C(Rows76C9C *self);
void func_08A770FC(Btn77168 *self);
void func_08A77168(Btn77168 *self);
void func_08A771B8(Btn77168 *self);
unsigned char func_08A772AC(Btn77168 *self);
void func_08A772D0(Obj772D0 *self);
void func_08A72998(void *self);
void func_08A72A14(void *self);
void func_08A772E0(void *self);
void func_08A7759C(void *self);

/* 0x08A77848..0x08A7EB18: flow steps */
void func_08A775D8(void *self, unsigned int flags);
void func_08A77848(Step *self, StepOwner *owner);
void func_08A7786C(Step *self, unsigned int flags);
void func_08A77968(Step *self, unsigned int flags);
void func_08A77C34(Step77C34 *self);
void func_08A77E98(Step77C34 *self, StepOwner *owner);
void func_08A77EFC(Step77C34 *self, StepOwner *owner);
void func_08A77F60(Step77C34 *self, int next);
void func_08A781F8(Step77C34 *self);
void func_08A78228(Step77C34 *self, int arg, int value);
void func_08A78328(Step77C34 *self);
void func_08A78904(Step *self, StepOwner *owner);
void func_08A7982C(Step *self, StepOwner *owner, int value);
void func_08A79858(Step79858 *self);
void func_08A79900(Step *self, unsigned int flags);
void func_08A79A38(Step *self, unsigned int flags);
void func_08A79AD0(StepWait *self, StepOwner *owner);
void func_08A79B78(StepWait *self);
void func_08A79B84(int arg1, int arg2);
void func_08A79BA0(Step *self, unsigned int flags);
StateQueue *func_08A79C30(StateQueue *self);
void func_08A79E68(StateQueue *self, int next);
void func_08A79F60(StateQueue *self);
Elem7A0D0 *func_08A7A0D0(Elem7A0D0 *self);
Elem7A0D0 *func_08A7A0F8(Elem7A0D0 *self);
void func_08A7A844(Step7A844 *self);
void func_08A7ADD8(Step7ADD8 *self);
void func_08A7B05C(Step7ADD8 *self, StepOwner *owner);
void func_08A7B114(Step *self, StepOwner *owner);
void func_08A7B134(Step7ADD8 *self, unsigned int flags);
void func_08A7B2B8(StepLink *self, unsigned int flags);
void func_08A7B514(StepBtn80 *self);
void func_08A7B5EC(StepBtn80 *self);
void func_08A7BA60(Rows74E80 *self, unsigned int flags);
void func_08A7D0A0(Step7D0A0 *self, StepOwner *owner);
void func_08A7D14C(Step *self, StepOwner *owner);
void func_08A7D16C(Step7D0A0 *self, unsigned int flags);
void func_08A7D1BC(Step7D0A0 *self);
void func_08A7D40C(Step7D0A0 *self);
void func_08A7D4F4(Step7D0A0 *self);
void func_08A7D96C(StepLink *self, unsigned int flags);
void func_08A7D988(StepLink *self);
void func_08A7DEC8(StepBtn80 *self);
void func_08A7DFA0(StepBtn80 *self);
void func_08A7E6F0(Step77C34 *self, StepOwner *owner);
void func_08A7E7CC(Step *self, StepOwner *owner);
void func_08A7E7EC(Step7E7EC *self, unsigned int flags);
void func_08A7E980(StepLink *self, unsigned int flags);
void func_08A7EA20(StepWait *self);
void func_08A7EA60(StepWait *self, StepOwner *owner);
void func_08A7EA90(StepWait *self);
void func_08A7EA9C(int arg1, int arg2);
void func_08A7EAB8(Step *self, unsigned int flags);

/* 0x08A7EEDC..0x08A812FC */
void func_08A7EB48(void *self);
void func_08A7EEDC(Screen7EEDC *self, int hide, int arg);
unsigned int func_08A7F104(Screen7EEDC *self, int arg1, int arg2);
int func_08A7F280(Screen7EEDC *self, int id);
void func_08A7F398(Screen7EEDC *self, int arg1, int arg2);
void func_08A7F474(Screen7EEDC *self, int arg1, int arg2, int arg3);
void func_08A7F96C(Screen7EEDC *self, unsigned int bit, char value, int arg);
int func_08A80A4C(Screen7EEDC *self, int kind);
void func_08A80B54(Screen7EEDC *self, unsigned int arg);
void func_08A80BC8(Screen7EEDC *self, int value);
Obj80D24 *func_08A80D24(Obj80D24 *self);
void func_08A812FC(Screen7EEDC *self);

/* 0x08A85E68..0x08A8A194 */
char *func_08A85E68(void);
char *func_08A85EB4(void);
char *func_08A85EC0(void);
char *func_08A85ECC(void);
char *func_08A85ED8(void);
extern Screen87EAC *D_08A87EAC;
extern int D_09B6CD88;
void func_08A86760(Screen87EAC *self);
void func_08A869AC(Screen87EAC *self);
void func_08A86A2C(Screen87EAC *self, int value);
void func_08A86AF8(Screen87EAC *self);
void func_08A86B30(void);
void func_08A86B50(void);
void func_08A86B70(void);
void func_08A87040(Screen87EAC *self);
void func_08A87060(Screen87EAC *self);
void func_08A870A4(Screen87EAC *self, void *arg);
void func_08A87270(Screen87EAC *self);
void func_08A872F4(Screen87EAC *self);
void func_08A8734C(Screen87EAC *self);
void func_08A8736C(Screen87EAC *self, int value);
void func_08A873A8(Screen87EAC *self, char arg);
void func_08A873E8(Screen87EAC *self);
void func_08A87408(Screen87EAC *self);
Screen87EAC *func_08A87428(Screen87EAC *self);
void func_08A874C8(void *arg);
void func_08A874FC(Screen87EAC *self);
Obj88584 *func_08A88584(Obj88584 *self);
extern Screen8B7A8 *D_08A8B7A8;
void func_08A88A70(Screen8B7A8 *self);
void func_08A88D48(Screen8B7A8 *self);
void func_08A88E5C(Screen8B7A8 *self);
void func_08A88EDC(Screen8B7A8 *self, int arg1, int arg2);
void func_08A89020(Screen8B7A8 *self);
void func_08A8908C(void);
void func_08A890AC(void);
void func_08A890CC(void);
void func_08A89288(void);
void func_08A891E4(void);
void func_08A89200(Screen8B7A8 *self);
void func_08A89F40(Screen8B7A8 *self);
void func_08A8A068(Screen8B7A8 *self);
Screen8B7A8 *func_08A8A194(Screen8B7A8 *self);
void func_08A8AF60(Screen8B7A8 *self);
void func_08A8EF2C(void *object, char arg1, int arg2);

/* ------------------------------------------------------------ 0x08A8A23C..0x08A96D10: View8E3B4 */

/* Element of View8E3B4's work block (two at +0x390; flags at +0x294). */
typedef struct Rec2CC {
    char pad_00[0x294];
    unsigned int flags;
    char pad_298[0x34];
} Rec2CC;
MENU3_SIZE(Rec2CC, 0x2CC);
MENU3_AT(Rec2CC, flags, 0x294);

/* Work block of View8E3B4 (+8). handles end where the messages pointer is
 * (the prompt takes it as its field_1C); texts and prompt_texts are the
 * two runs func_08A8ED84 releases. */
typedef struct Work8E3B4 {
    MenuLayout *layout;
    MenuModel *handles[0x89];
    void *messages;
    void *texts[0x19];
    void *prompt_texts[6];
    unsigned int flags;
    int field_2AC;
    char pad_2B0[0x4];
    int index;
    char pad_2B8[0x4];
    int field_2BC[2];
    int field_2C4;
    char pad_2C8[0x4];
    int field_2CC;
    char pad_2D0[0x8];
    int field_2D8;
    char pad_2DC[0x20];
    unsigned char field_2FC[0xC];
    int field_308[4]; /* record numbers, indexed by index */
    unsigned char field_318[4];
    char pad_31C[0x38];
    short field_354[0x14];
    char pad_37C[0x14];
    Rec2CC records[2];
    char pad_928[0x1B0];
    int field_AD8;
    int field_ADC;
    void *field_AE0;
    MenuPrompt prompt;
} Work8E3B4;
MENU3_AT(Work8E3B4, layout, 0x0);
MENU3_AT(Work8E3B4, handles, 0x4);
MENU3_AT(Work8E3B4, messages, 0x228);
MENU3_AT(Work8E3B4, texts, 0x22C);
MENU3_AT(Work8E3B4, prompt_texts, 0x290);
MENU3_AT(Work8E3B4, flags, 0x2A8);
MENU3_AT(Work8E3B4, field_2AC, 0x2AC);
MENU3_AT(Work8E3B4, index, 0x2B4);
MENU3_AT(Work8E3B4, field_2BC, 0x2BC);
MENU3_AT(Work8E3B4, field_2C4, 0x2C4);
MENU3_AT(Work8E3B4, field_2CC, 0x2CC);
MENU3_AT(Work8E3B4, field_2D8, 0x2D8);
MENU3_AT(Work8E3B4, field_2FC, 0x2FC);
MENU3_AT(Work8E3B4, field_308, 0x308);
MENU3_AT(Work8E3B4, field_318, 0x318);
MENU3_AT(Work8E3B4, field_354, 0x354);
MENU3_AT(Work8E3B4, records, 0x390);
MENU3_AT(Work8E3B4, field_AD8, 0xAD8);
MENU3_AT(Work8E3B4, field_ADC, 0xADC);
MENU3_AT(Work8E3B4, field_AE0, 0xAE0);
MENU3_AT(Work8E3B4, prompt, 0xAE4);

/* Member at +0xC of View8E3B4 (constructor func_08A946F0, destructor func_08A946F8). */
typedef struct Obj946F0 {
    char pad_00[0xC];
} Obj946F0;
MENU3_SIZE(Obj946F0, 0xC);

/* Member at +0x18 of View8E3B4 (constructor func_08A96D10). */
typedef struct Obj96D10 {
    int field_00;
    int field_04;
} Obj96D10;
MENU3_SIZE(Obj96D10, 0x8);
MENU3_AT(Obj96D10, field_00, 0x0);
MENU3_AT(Obj96D10, field_04, 0x4);

/* A view of the screen in D_08A8B7A8 (its field_154); destructor func_08A8EC28. */
typedef struct View8E3B4 {
    int field_00;
    char pad_04[0x4];
    Work8E3B4 *work;
    Obj946F0 field_0C;
    Obj96D10 field_18;
} View8E3B4;
MENU3_AT(View8E3B4, field_00, 0x0);
MENU3_AT(View8E3B4, work, 0x8);
MENU3_AT(View8E3B4, field_0C, 0xC);
MENU3_AT(View8E3B4, field_18, 0x18);

typedef struct Obj96A30 {
    char pad_00[0x4];
    int field_04;
} Obj96A30;
MENU3_AT(Obj96A30, field_04, 0x4);

/* ------------------------------------------------------------ 0x08A980C8..0x08A9D0E0: the screen in D_08AA3F44 */

/* Record filled by func_08B2BB2C. */
typedef struct RecB2BB2C {
    char field_00;
    char pad_01[0xF];
    int field_10;
} RecB2BB2C;
MENU3_AT(RecB2BB2C, field_00, 0x0);
MENU3_AT(RecB2BB2C, field_10, 0x10);

/* Work block of ScreenA3F44 (+0x154). state / next are the screen's step
 * numbers (next is entered when a script ends). */
typedef struct WorkA3F44 {
    char pad_00[0x4];
    MenuLayout layout;
    char pad_404[0x480];
    void *messages;
    char pad_888[0x88];
    int state;
    int next;
    char pad_918[0x60];
    struct ScreenA7ED0 *field_978;
    char pad_97C[0x34];
    int field_9B0;
    char pad_9B4[0x4];
    int field_9B8;
    char pad_9BC[0x8];
    unsigned char field_9C4[4];
    char pad_9C8[0x157C];
    int field_1F44;
    char pad_1F48[0x5CD4];
    int field_7C1C;
    RecB2BB2C field_7C20;
    char pad_7C34[0x118];
    RecB2BB2C field_7D4C;
    char pad_7D60[0x2E4];
    struct ScreenAB7CFC *field_8044;
    int field_8048;
    char pad_804C[0x8];
    int field_8054;
    char pad_8058[0x15C];
    unsigned int flags;
    char pad_81B8[0xC];
    int field_81C4;
    char pad_81C8[0x4];
    int field_81CC;
    unsigned char field_81D0[4];
    char pad_81D4[0x11C];
    int field_82F0;
    int field_82F4;
    char pad_82F8[0x7E8];
    void *field_8AE0;
    char pad_8AE4[0x14];
    void *field_8AF8; /* a layout entry */
    char pad_8AFC[0x3FC];
    void *field_8EF8; /* a model handle */
    char pad_8EFC[0x47C];
    void *texts[6];
    MenuPrompt prompt;
} WorkA3F44;
MENU3_AT(WorkA3F44, layout, 0x4);
MENU3_AT(WorkA3F44, messages, 0x884);
MENU3_AT(WorkA3F44, state, 0x910);
MENU3_AT(WorkA3F44, next, 0x914);
MENU3_AT(WorkA3F44, field_978, 0x978);
MENU3_AT(WorkA3F44, field_9B0, 0x9B0);
MENU3_AT(WorkA3F44, field_9B8, 0x9B8);
MENU3_AT(WorkA3F44, field_9C4, 0x9C4);
MENU3_AT(WorkA3F44, field_1F44, 0x1F44);
MENU3_AT(WorkA3F44, field_7C1C, 0x7C1C);
MENU3_AT(WorkA3F44, field_7C20, 0x7C20);
MENU3_AT(WorkA3F44, field_7D4C, 0x7D4C);
MENU3_AT(WorkA3F44, field_8044, 0x8044);
MENU3_AT(WorkA3F44, field_8048, 0x8048);
MENU3_AT(WorkA3F44, field_8054, 0x8054);
MENU3_AT(WorkA3F44, flags, 0x81B4);
MENU3_AT(WorkA3F44, field_81C4, 0x81C4);
MENU3_AT(WorkA3F44, field_81CC, 0x81CC);
MENU3_AT(WorkA3F44, field_81D0, 0x81D0);
MENU3_AT(WorkA3F44, field_82F0, 0x82F0);
MENU3_AT(WorkA3F44, field_82F4, 0x82F4);
MENU3_AT(WorkA3F44, field_8AE0, 0x8AE0);
MENU3_AT(WorkA3F44, field_8AF8, 0x8AF8);
MENU3_AT(WorkA3F44, field_8EF8, 0x8EF8);
MENU3_AT(WorkA3F44, texts, 0x9378);
MENU3_AT(WorkA3F44, prompt, 0x9390);

/* Member at +0x894 of ScreenA3F44 and pointed to by WorkAB7CFC (constructor
 * func_08AB41E0, destructor func_08AB4228): three callback pairs (a table
 * and the object), flags at +0x2C and +0x30, sixteen values read by
 * func_08AB73A0 and three rows of five (func_08AB6D04). */
typedef struct ObjB41E0 {
    int field_00;
    void *field_04; /* D_08AB7C18 */
    struct ObjB41E0 *field_08;
    void *field_0C; /* D_08AB7C30 */
    struct ObjB41E0 *field_10;
    int field_14;
    char pad_18[0x4];
    void *field_1C; /* D_08AB7C78 */
    struct ObjB41E0 *field_20;
    int field_24;
    int field_28;
    unsigned int flags;
    unsigned int field_30;
    int field_34;
    char pad_38[0x14];
    short field_4C;
    short field_4E;
    short field_50;
    short field_52;
    short field_54;
    short field_56;
    short field_58;
    short field_5A;
    short field_5C;
    short field_5E;
    short field_60;
    short field_62;
    short field_64;
    short field_66;
    short field_68;
    short field_6A;
    int field_6C[5];
    int field_80[5];
    int field_94[5];
    int field_A8;
} ObjB41E0;
MENU3_AT(ObjB41E0, field_00, 0x0);
MENU3_AT(ObjB41E0, field_04, 0x4);
MENU3_AT(ObjB41E0, field_08, 0x8);
MENU3_AT(ObjB41E0, field_0C, 0xC);
MENU3_AT(ObjB41E0, field_10, 0x10);
MENU3_AT(ObjB41E0, field_14, 0x14);
MENU3_AT(ObjB41E0, field_1C, 0x1C);
MENU3_AT(ObjB41E0, field_20, 0x20);
MENU3_AT(ObjB41E0, field_24, 0x24);
MENU3_AT(ObjB41E0, field_28, 0x28);
MENU3_AT(ObjB41E0, flags, 0x2C);
MENU3_AT(ObjB41E0, field_30, 0x30);
MENU3_AT(ObjB41E0, field_34, 0x34);
MENU3_AT(ObjB41E0, field_4C, 0x4C);
MENU3_AT(ObjB41E0, field_4E, 0x4E);
MENU3_AT(ObjB41E0, field_50, 0x50);
MENU3_AT(ObjB41E0, field_52, 0x52);
MENU3_AT(ObjB41E0, field_54, 0x54);
MENU3_AT(ObjB41E0, field_56, 0x56);
MENU3_AT(ObjB41E0, field_58, 0x58);
MENU3_AT(ObjB41E0, field_5A, 0x5A);
MENU3_AT(ObjB41E0, field_5C, 0x5C);
MENU3_AT(ObjB41E0, field_5E, 0x5E);
MENU3_AT(ObjB41E0, field_60, 0x60);
MENU3_AT(ObjB41E0, field_62, 0x62);
MENU3_AT(ObjB41E0, field_64, 0x64);
MENU3_AT(ObjB41E0, field_66, 0x66);
MENU3_AT(ObjB41E0, field_68, 0x68);
MENU3_AT(ObjB41E0, field_6A, 0x6A);
MENU3_AT(ObjB41E0, field_6C, 0x6C);
MENU3_AT(ObjB41E0, field_80, 0x80);
MENU3_AT(ObjB41E0, field_94, 0x94);
MENU3_AT(ObjB41E0, field_A8, 0xA8);

/* Screen in D_08AA3F44: a 0x100-byte block for its task, the task, heaps. */
typedef struct ScreenA3F44 {
    unsigned char block[0x100];
    MenuTask task;
    void *field_134; /* released with func_0888208C */
    void *heap;
    void *field_13C; /* a block from D_089D5EC0's heap */
    void *field_140; /* a heap */
    char pad_144[0x10];
    WorkA3F44 *work;
    unsigned char field_158[4]; /* built by func_0891A810 (size not known) */
    char pad_15C[0x738];
    ObjB41E0 field_894;
} ScreenA3F44;
MENU3_AT(ScreenA3F44, block, 0x0);
MENU3_AT(ScreenA3F44, task, 0x100);
MENU3_AT(ScreenA3F44, field_134, 0x134);
MENU3_AT(ScreenA3F44, heap, 0x138);
MENU3_AT(ScreenA3F44, field_13C, 0x13C);
MENU3_AT(ScreenA3F44, field_140, 0x140);
MENU3_AT(ScreenA3F44, work, 0x154);
MENU3_AT(ScreenA3F44, field_158, 0x158);
MENU3_AT(ScreenA3F44, field_894, 0x894);

/* 0x08A8A23C..0x08A96D10 */
void func_08A89894(Screen8B7A8 *self, void *arg);
void func_08A8A23C(void *arg);
void func_08A8A440(Screen8B7A8 *self, char arg1, int arg2);
void func_08A8A534(Screen8B7A8 *self, int value);
int func_08A8A540(Screen8B7A8 *self);
void func_08A8B894(View8E3B4 *self);
void func_08A8BBC0(View8E3B4 *self);
void func_08A8BFC4(void);
void func_08A8BFCC(void);
void func_08A8C0EC(View8E3B4 *self);
void func_08A8C298(View8E3B4 *self);
void func_08A8C3A0(View8E3B4 *self);
void func_08A8C46C(View8E3B4 *self, int arg);
void func_08A8C540(View8E3B4 *self);
void func_08A8CCE0(View8E3B4 *self, char arg1, int arg2);
void func_08A8CD90(View8E3B4 *self, char arg1, int arg2);
void func_08A8CE88(View8E3B4 *self, unsigned char shown, int kind);
void func_08A8CF34(View8E3B4 *self, unsigned char shown);
void func_08A8D098(View8E3B4 *self, unsigned char shown);
void func_08A8D0D8(View8E3B4 *self, unsigned char shown);
void func_08A8D27C(View8E3B4 *self, char arg);
void func_08A8D2BC(View8E3B4 *self, int arg);
void func_08A8D480(View8E3B4 *self, int id, int arg);
void func_08A8D70C(View8E3B4 *self, int arg1, int arg2, char arg3);
void func_08A8DA6C(View8E3B4 *self, int arg);
void func_08A8DB4C(View8E3B4 *self, int arg1, int arg2, int arg3);
void func_08A8DC60(View8E3B4 *self, int id, int arg);
void func_08A8DE60(View8E3B4 *self, int arg1, int arg2);
void func_08A8DF30(View8E3B4 *self, unsigned int arg);
void func_08A8DFA4(View8E3B4 *self, int index, int value, char arg);
void func_08A8E0C0(View8E3B4 *self, int arg);
void func_08A8E3B4(View8E3B4 *self);
int func_08A8E438(View8E3B4 *self);
void func_08A8E668(View8E3B4 *self);
void func_08A8E6F4(View8E3B4 *self);
void func_08A8E74C(View8E3B4 *self);
void func_08A8E7AC(View8E3B4 *self);
int func_08A8E7E8(View8E3B4 *self);
void func_08A8E854(View8E3B4 *self, int index, int value);
View8E3B4 *func_08A8EBF4(View8E3B4 *self);
void func_08A8EC28(View8E3B4 *self, unsigned int flags);
void func_08A8ED84(View8E3B4 *self);
unsigned char func_08A8EE44(View8E3B4 *self);
int func_08A8EF14(View8E3B4 *self);
void func_08A8EF90(View8E3B4 *self, int arg);
void func_08A91FD4(View8E3B4 *self, char arg1, int arg2, int arg3);
void func_08A924A8(View8E3B4 *self, char arg1, int arg2, int arg3);
void func_08A932D0(View8E3B4 *self, int mode);
Obj946F0 *func_08A946F0(Obj946F0 *self);
void func_08A946F8(Obj946F0 *self, unsigned int flags);
unsigned char func_08A94724(Obj946F0 *self, Rec2CC *record);
void func_08A9482C(Obj946F0 *self, Rec2CC *record, int kind);
void func_08A94A1C(Obj946F0 *self, Rec2CC *record);
void func_08A94C04(Obj946F0 *self, char arg, Rec2CC *record, MenuModel **handles);
void func_08A95A38(Obj946F0 *self, Rec2CC *record, MenuLayout *layout, MenuModel **handles, char arg);
void *func_08A969CC(void *self);
void func_08A969D4(void *arg);
void func_08A96A08(void);
Obj96A30 *func_08A96A30(Obj96A30 *self);
void func_08A96A68(Link8A068 *self);
void func_08A96B14(Link8A068 *self);
int func_08A96D4C(Obj96D10 *self, int arg);
Obj96D10 *func_08A96D10(Obj96D10 *self);
void func_08A96D20(Obj96D10 *self, unsigned int flags);
void func_08A97030(Obj96D10 *self, void *messages);
void func_08A97750(Obj96D10 *self, int arg, void *out);
void func_08933C00(char *text);
int func_089C5FB8(void);
void func_08B2BA5C(int id);
void func_08B2BADC(int id);
void func_08B2BB2C(int id, RecB2BB2C *out);

/* 0x08A980C8..0x08A9D0E0 */
extern ScreenA3F44 *D_08AA3F44;
extern int D_09B6EFB0;
void func_08A97DA8(ScreenA3F44 *self);
void func_08A980C8(ScreenA3F44 *self);
void func_08A98128(void);
void func_08A98148(void);
void func_08A98168(void);
void func_08A9832C(ScreenA3F44 *self);
void func_08A98420(ScreenA3F44 *self);
void func_08A98458(ScreenA3F44 *self);
void func_08A984BC(ScreenA3F44 *self);
void func_08A9869C(ScreenA3F44 *self);
void func_08A988B8(ScreenA3F44 *self);
void func_08A989A8(ScreenA3F44 *self);
void func_08A98A08(ScreenA3F44 *self);
void func_08A98D20(ScreenA3F44 *self);
void func_08A98D88(ScreenA3F44 *self);
void func_08A98E50(ScreenA3F44 *self);
void func_08A98E88(ScreenA3F44 *self);
void func_08A99248(ScreenA3F44 *self);
void func_08A992C0(ScreenA3F44 *self);
void func_08A99364(ScreenA3F44 *self);
void func_08A99624(ScreenA3F44 *self);
void func_08A9990C(ScreenA3F44 *self);
void func_08A99A90(ScreenA3F44 *self);
void func_08A99AC8(ScreenA3F44 *self);
void func_08A99B80(ScreenA3F44 *self);
void func_08A99E40(ScreenA3F44 *self);
int func_08A9A2BC(ScreenA3F44 *self, int arg);
int func_08A9A460(ScreenA3F44 *self);
void func_08A9A4CC(ScreenA3F44 *self);
int func_08A9A52C(ScreenA3F44 *self, void *arg);
void func_08A9A68C(ScreenA3F44 *self);
void func_08A9A6D8(void);
void func_08A9A890(ScreenA3F44 *self);
void func_08A9AA84(ScreenA3F44 *self);
void func_08A9ACAC(ScreenA3F44 *self);
void func_08A9AE4C(ScreenA3F44 *self);
void func_08A9AFB0(ScreenA3F44 *self);
int func_08A9B0DC(ScreenA3F44 *self);
void func_08A9B11C(ScreenA3F44 *self);
void func_08A9B1B0(void);
void func_08A9B2CC(void);
int func_08A9B2D4(ScreenA3F44 *self);
void func_08A9B7E8(ScreenA3F44 *self);
void func_08A9B904(ScreenA3F44 *self);
int func_08A9B950(ScreenA3F44 *self);
void func_08A9B9AC(ScreenA3F44 *self, int value);
void func_08A9B9C0(ScreenA3F44 *self);
void func_08A9BD8C(void);
void func_08A9BEA0(ScreenA3F44 *self);
void func_08A9C140(ScreenA3F44 *self, int arg);
void func_08A9C1FC(ScreenA3F44 *self, unsigned char arg);
void func_08A9C398(ScreenA3F44 *self);
void func_08A9C490(ScreenA3F44 *self);
void func_08A9C538(ScreenA3F44 *self);
void func_08A9C55C(ScreenA3F44 *self, int value);
void func_08A9C5A0(ScreenA3F44 *self, char arg);
void func_08A9C5E4(ScreenA3F44 *self);
void func_08A9C608(ScreenA3F44 *self);
ScreenA3F44 *func_08A9CBDC(ScreenA3F44 *self);
void func_08A9CC28(ScreenA3F44 *self, unsigned int flags);
void func_08A9CEE0(void *arg);
void func_08A9CF04(int arg);
void func_08A9CF28(void);
void func_08A9CF48(void);
void func_08A9CF8C(ScreenA3F44 *self);
void func_08A9CFF4(ScreenA3F44 *self);
void func_08A9D060(void);
void func_08A9D080(void);
int func_08A9D0E0(void);
void func_08A9D0E8(ScreenA3F44 *self);
void func_08A9D72C(ScreenA3F44 *self);
void func_08A9E0E8(ScreenA3F44 *self);
void func_08A9ECB8(ScreenA3F44 *self);
void func_08AA0D48(ScreenA3F44 *self, char arg);
void func_08AA1248(ScreenA3F44 *self);
void func_08AA161C(ScreenA3F44 *self);
void func_08AA2AC4(ScreenA3F44 *self);
void func_08AA2E20(ScreenA3F44 *self);
void func_08AA32F8(ScreenA3F44 *self);
void func_08AA5544(struct ScreenA7ED0 *self, void *out);
int func_08AA55CC(struct ScreenA7ED0 *self);
void func_08AAB5B4(struct ScreenAB7CFC *self, void *out);
int func_08AAB624(struct ScreenAB7CFC *self);
int func_08AAB630(struct ScreenAB7CFC *self);
void func_08AB3908(void *object, int arg);
ObjB41E0 *func_08AB41E0(ObjB41E0 *self);
void func_08AB4228(ObjB41E0 *self, unsigned int flags);
void func_08AB4254(ObjB41E0 *self, unsigned char arg);
int func_08AB43FC(ObjB41E0 *self, int arg);
void func_08AB5790(ObjB41E0 *self, int arg);
void func_08AB58E0(ObjB41E0 *self, int arg1, short arg2);
void *func_0891A810(void *object);
void func_08889ED4(int index, MenuCallback callback, char arg);

/* ------------------------------------------------------------ 0x08AA4164..0x08AB2D24: two screens and their records */

/* Record read by func_08AB1420..func_08AB16A8 (record_pick_08ab16a8.c reads
 * it as PickState). Size from WorkA7ED0, where field_1DEC follows it, and
 * from WorkAB7CFC's five records. */
typedef struct Record16DC {
    char field_00;
    char field_01;
    char field_02;
    char field_03;
    char pad_04[0x16D8];
} Record16DC;
MENU3_SIZE(Record16DC, 0x16DC);
MENU3_AT(Record16DC, field_00, 0x0);
MENU3_AT(Record16DC, field_01, 0x1);
MENU3_AT(Record16DC, field_02, 0x2);
MENU3_AT(Record16DC, field_03, 0x3);

/* Work block of ScreenA7ED0 (+0x1EE4). */
typedef struct WorkA7ED0 {
    char pad_00[0x4];
    int state;
    char pad_08[0x4];
    MenuLayout *layout;
    MenuModel *handles[0x120];
    void *texts[0x21];
    char pad_514[0x44];
    int field_558;
    char pad_55C[0x1B4];
    Record16DC record;
    unsigned short field_1DEC;
    char pad_1DEE[0x6];
    int field_1DF4;
    char pad_1DF8[0x78];
    unsigned int flags;
    char pad_1E74[0x24];
    int field_1E98;
} WorkA7ED0;
MENU3_AT(WorkA7ED0, state, 0x4);
MENU3_AT(WorkA7ED0, layout, 0xC);
MENU3_AT(WorkA7ED0, handles, 0x10);
MENU3_AT(WorkA7ED0, texts, 0x490);
MENU3_AT(WorkA7ED0, field_558, 0x558);
MENU3_AT(WorkA7ED0, record, 0x710);
MENU3_AT(WorkA7ED0, field_1DEC, 0x1DEC);
MENU3_AT(WorkA7ED0, field_1DF4, 0x1DF4);
MENU3_AT(WorkA7ED0, flags, 0x1E70);
MENU3_AT(WorkA7ED0, field_1E98, 0x1E98);

/* Screen with vtable D_08AA7ED0 at +0x1EE8 (deleting destructor func_08AA507C). */
typedef struct ScreenA7ED0 {
    char pad_00[0x1EE4];
    WorkA7ED0 *work;
    void *vtable;
} ScreenA7ED0;
MENU3_AT(ScreenA7ED0, work, 0x1EE4);
MENU3_AT(ScreenA7ED0, vtable, 0x1EE8);

/* Block at D_09B6EFB0 + 0x304 (func_08AABC54 returns it): per-player
 * values indexed by index. */
typedef struct PlayerBlock {
    int field_00[5];
    int field_14[5];
    char pad_28[0x104];
    unsigned int flags[5];
    char pad_140[0x14];
    int index;
} PlayerBlock;
MENU3_AT(PlayerBlock, field_00, 0x0);
MENU3_AT(PlayerBlock, field_14, 0x14);
MENU3_AT(PlayerBlock, flags, 0x12C);
MENU3_AT(PlayerBlock, index, 0x154);

/* Work block of ScreenAB7CFC (+0x7CFC). */
typedef struct WorkAB7CFC {
    char pad_00[0x4];
    int state;
    char pad_08[0x4];
    MenuLayout *layout;
    MenuModel *handles[0x120];
    void *texts[0x45]; /* length not known (the gap to field_5A4) */
    int field_5A4;
    char pad_5A8[0x4B0];
    Record16DC records[5];
    char pad_7CA4[0x14];
    int field_7CB8;
    char pad_7CBC[0x4];
    unsigned int flags;
    int field_7CC4;
    int field_7CC8;
    ObjB41E0 *field_7CCC;
    char pad_7CD0[0x4];
    int field_7CD4;
    char pad_7CD8[0x4];
    int field_7CDC;
    int field_7CE0; /* a countdown */
} WorkAB7CFC;
MENU3_AT(WorkAB7CFC, state, 0x4);
MENU3_AT(WorkAB7CFC, layout, 0xC);
MENU3_AT(WorkAB7CFC, handles, 0x10);
MENU3_AT(WorkAB7CFC, texts, 0x490);
MENU3_AT(WorkAB7CFC, field_5A4, 0x5A4);
MENU3_AT(WorkAB7CFC, records, 0xA58);
MENU3_AT(WorkAB7CFC, field_7CB8, 0x7CB8);
MENU3_AT(WorkAB7CFC, flags, 0x7CC0);
MENU3_AT(WorkAB7CFC, field_7CC4, 0x7CC4);
MENU3_AT(WorkAB7CFC, field_7CC8, 0x7CC8);
MENU3_AT(WorkAB7CFC, field_7CCC, 0x7CCC);
MENU3_AT(WorkAB7CFC, field_7CD4, 0x7CD4);
MENU3_AT(WorkAB7CFC, field_7CDC, 0x7CDC);
MENU3_AT(WorkAB7CFC, field_7CE0, 0x7CE0);

typedef struct ScreenAB7CFC {
    char pad_00[0x7CFC];
    WorkAB7CFC *work;
} ScreenAB7CFC;
MENU3_AT(ScreenAB7CFC, work, 0x7CFC);

/* Work block of PanelAB2D24 (+0x584). */
typedef struct WorkAB2D24 {
    char pad_00[0x4];
    int state;
    char pad_08[0x4];
    MenuLayout *layout;
    MenuModel *handles[0x120];
    void *texts[1]; /* length not known */
    char pad_494[0xD4];
    char field_568;
} WorkAB2D24;
MENU3_AT(WorkAB2D24, state, 0x4);
MENU3_AT(WorkAB2D24, layout, 0xC);
MENU3_AT(WorkAB2D24, handles, 0x10);
MENU3_AT(WorkAB2D24, texts, 0x490);
MENU3_AT(WorkAB2D24, field_568, 0x568);

typedef struct PanelAB2D24 {
    char pad_00[0x584];
    WorkAB2D24 *work;
} PanelAB2D24;
MENU3_AT(PanelAB2D24, work, 0x584);

typedef struct WorkAB35B8 {
    char pad_00[0x488];
    void *texts[1];
    char pad_48C[0xC];
    unsigned int flags;
    char pad_49C[0xC];
    int field_4A8;
} WorkAB35B8;
MENU3_AT(WorkAB35B8, texts, 0x488);
MENU3_AT(WorkAB35B8, flags, 0x498);
MENU3_AT(WorkAB35B8, field_4A8, 0x4A8);

typedef struct ObjAB35B8 {
    WorkAB35B8 *work;
} ObjAB35B8;
MENU3_AT(ObjAB35B8, work, 0x0);

typedef struct ObjAB3664 {
    char pad_00[0x4];
    int field_04;
} ObjAB3664;
MENU3_AT(ObjAB3664, field_04, 0x4);

/* 0x08AA4164..0x08AB2D24 */
extern unsigned char D_08AA7ED0[];
void func_08AA4164(ScreenA7ED0 *self);
void func_08AA46AC(ScreenA7ED0 *self);
int func_08AA4800(void);
void func_08AA4CF0(ScreenA7ED0 *self, int arg);
void func_08AA4E34(ScreenA7ED0 *self);
void func_08AA507C(ScreenA7ED0 *self, unsigned int flags);
void func_08AA5458(ScreenA7ED0 *self);
void func_08AA5478(ScreenA7ED0 *self);
void func_08AA5498(ScreenA7ED0 *self);
void func_08AA5504(ScreenA7ED0 *self);
void func_08AA5524(ScreenA7ED0 *self);
void func_08AA56AC(ScreenA7ED0 *self);
int func_08AA8E38(ScreenAB7CFC *self);
void func_08AA9594(ScreenAB7CFC *self);
void func_08AA9604(ScreenAB7CFC *self);
void func_08AA9800(ScreenAB7CFC *self);
unsigned char func_08AA99D4(ScreenAB7CFC *self);
void func_08AA9EF8(ScreenAB7CFC *self);
void func_08AA9F78(ScreenAB7CFC *self, int index, int arg);
void func_08AAA6C0(ScreenAB7CFC *self, int arg);
int func_08AAA718(ScreenAB7CFC *self, char *buffer, int arg);
void func_08AAA780(ScreenAB7CFC *self, int arg);
void func_08AAA980(ScreenAB7CFC *self);
void func_08AAAA8C(ScreenAB7CFC *self);
void func_08AAAD34(ScreenAB7CFC *self);
void func_08AAB060(ScreenAB7CFC *self, int index);
void func_08AAB170(ScreenAB7CFC *self);
void func_08AAB190(ScreenAB7CFC *self);
int func_08AAB63C(ScreenAB7CFC *self, int which);
void func_08AAB658(ScreenAB7CFC *self);
void func_08AAB8D8(ScreenAB7CFC *self);
void func_08AAB960(ScreenAB7CFC *self);
PlayerBlock *func_08AABC54(void);
void func_08AAC5D0(ScreenAB7CFC *self);
int *func_08AB0F80(Record16DC *self, int kind, int index);
int func_08AB1420(Record16DC *self);
int func_08AB1428(Record16DC *self);
int func_08AB144C(Record16DC *self);
int func_08AB1610(Record16DC *self);
int func_08AB16A8(Record16DC *self);
int func_08AB1748(Record16DC *self);
void func_08AB1B8C(int value, Record16DC *records, int count);
unsigned int func_08AB1CDC(int value);
int func_08AB2838(PanelAB2D24 *self);
void func_08AB2B5C(PanelAB2D24 *self);
void func_08AB2C5C(PanelAB2D24 *self);
void func_08AB2D24(PanelAB2D24 *self);
void func_08AB35B8(ObjAB35B8 *self);
ObjAB3664 *func_08AB3664(ObjAB3664 *self);
void func_08AB3BA0(ObjAB35B8 *self);
void func_088CB9E8(char *buffer);
extern unsigned char D_08AB7C18[];
extern unsigned char D_08AB7C30[];
extern unsigned char D_08AB7C78[];
extern void *D_08AB7C90;
extern void *D_089D8364;
void func_08907300(void *object, int arg);
void func_08AB43D4(int arg1, int arg2);
void func_08AB45CC(ObjB41E0 *self, int arg1, int arg2, int arg3);
int func_08AB47BC(ObjB41E0 *self);
void func_08AB4930(ObjB41E0 *self);
void func_08AB51F4(ObjB41E0 *self, int index, unsigned int value);
void func_08AB5EC8(ObjB41E0 *self);
void func_08AB5F00(ObjB41E0 *self);
void func_08AB5FA8(void);
void func_08AB6080(ObjB41E0 *self);
void func_08AB6D04(ObjB41E0 *self, int index, int arg1, int arg2, int arg3, int arg4);
void func_08AB6D20(ObjB41E0 *self, unsigned char on);
void func_08AB6D50(void);
void func_08AB6E5C(ObjB41E0 *self);
void func_08AB6F94(ObjB41E0 *self);
void func_08AB6FA4(ObjB41E0 *self);
void func_08AB6FB4(ObjB41E0 *self);
void func_08AB6FD8(ObjB41E0 *self);
void func_08AB71C0(ObjB41E0 *self);
void func_08AB72A0(ObjB41E0 *self);
int func_08AB73A0(ObjB41E0 *self, int kind);
void func_08AB737C(void *object, int arg1, int arg2);

/* ------------------------------------------------------------ 0x08AB74E0..0x08AB7C98: ObjB41E0 callbacks */

/* A callback pair of ObjB41E0 (table, object). */
typedef struct LinkB41E0 {
    void *table;
    ObjB41E0 *object;
} LinkB41E0;
MENU3_AT(LinkB41E0, table, 0x0);
MENU3_AT(LinkB41E0, object, 0x4);

typedef struct LinkB41E0At8 {
    char pad_00[0x8];
    ObjB41E0 *object;
} LinkB41E0At8;
MENU3_AT(LinkB41E0At8, object, 0x8);

typedef struct ObjAB7C98 {
    char pad_00[0x4];
    int field_04;
} ObjAB7C98;
MENU3_AT(ObjAB7C98, field_04, 0x4);

/* ------------------------------------------------------------ 0x08AB8128..0x08AB9C18: the screen in D_08ABB9E4 */

typedef struct WorkBB9E4 {
    char pad_00[0x8FC];
    void *field_8FC; /* handed to func_0887AD78 */
    char pad_900[0x42C];
    int state;
    char pad_D30[0x30];
    int field_D60[0x40]; /* length not known (the gap to field_E60) */
    int field_E60;
} WorkBB9E4;
MENU3_AT(WorkBB9E4, field_8FC, 0x8FC);
MENU3_AT(WorkBB9E4, state, 0xD2C);
MENU3_AT(WorkBB9E4, field_D60, 0xD60);
MENU3_AT(WorkBB9E4, field_E60, 0xE60);

/* Screen in D_08ABB9E4 (declared void * in auto_region1.h). */
typedef struct ScreenBB9E4 {
    unsigned char block[0x100];
    MenuTask task;
    void *heap;
    WorkBB9E4 *work;
} ScreenBB9E4;
MENU3_AT(ScreenBB9E4, block, 0x0);
MENU3_AT(ScreenBB9E4, task, 0x100);
MENU3_AT(ScreenBB9E4, heap, 0x134);
MENU3_AT(ScreenBB9E4, work, 0x138);

/* ------------------------------------------------------------ 0x08ABBB30..0x08ABF77C: the screen in D_08AC4560 */

/* Fields of D_089D712C past auto_region1.h's Global9D712C. */
typedef struct Global9D712CView {
    char pad_00[0xEFD];
    unsigned char field_EFD;
    char pad_EFE[0x1EF0A];
    char field_1FE08[0x1A];
    char pad_1FE22[0x1F726];
    unsigned char field_3F548[4];
} Global9D712CView;
MENU3_AT(Global9D712CView, field_EFD, 0xEFD);
MENU3_AT(Global9D712CView, field_1FE08, 0x1FE08);
MENU3_AT(Global9D712CView, field_3F548, 0x3F548);

/* Element of the table D_08AC42B6 (func_08ABC934 looks an id up). */
typedef struct IdEntry8 {
    unsigned short id;
    char pad_02[0x6];
} IdEntry8;
MENU3_SIZE(IdEntry8, 0x8);
MENU3_AT(IdEntry8, id, 0x0);

/* Work block of ScreenC4560 (+0x14C). */
typedef struct WorkC4560 {
    MenuLayout layout;
    MenuModel *handles[0x120];
    MenuCursor cursor;
    char pad_8AC[0x30];
    unsigned char field_8DC[0x14];
    void *messages;
    void *texts[0x35];
    char pad_9C8[0x90];
    unsigned int flags;
    int state;
    char pad_A60[0x28];
    int field_A88[0x20]; /* indexed by field_B08 (length not known) */
    int field_B08;
    int field_B0C;
    char pad_B10[0x1A];
    char field_B2A[0x1A];
    char field_B44;
    char pad_B45[0x2];
    unsigned char field_B47;
    char pad_B48[0x10];
    unsigned char field_B58;
    char pad_B59[0x7];
    void *field_B60; /* blocks from the screen's heap */
    void *field_B64;
    void *field_B68;
    void *field_B6C;
    BlockView *field_B70;
    char pad_B74[0x8];
    void *field_B7C;
    void *field_B80;
    void *field_B84;
} WorkC4560;
MENU3_AT(WorkC4560, layout, 0x0);
MENU3_AT(WorkC4560, handles, 0x400);
MENU3_AT(WorkC4560, cursor, 0x880);
MENU3_AT(WorkC4560, field_8DC, 0x8DC);
MENU3_AT(WorkC4560, messages, 0x8F0);
MENU3_AT(WorkC4560, texts, 0x8F4);
MENU3_AT(WorkC4560, flags, 0xA58);
MENU3_AT(WorkC4560, state, 0xA5C);
MENU3_AT(WorkC4560, field_A88, 0xA88);
MENU3_AT(WorkC4560, field_B08, 0xB08);
MENU3_AT(WorkC4560, field_B0C, 0xB0C);
MENU3_AT(WorkC4560, field_B2A, 0xB2A);
MENU3_AT(WorkC4560, field_B44, 0xB44);
MENU3_AT(WorkC4560, field_B47, 0xB47);
MENU3_AT(WorkC4560, field_B58, 0xB58);
MENU3_AT(WorkC4560, field_B60, 0xB60);
MENU3_AT(WorkC4560, field_B64, 0xB64);
MENU3_AT(WorkC4560, field_B68, 0xB68);
MENU3_AT(WorkC4560, field_B6C, 0xB6C);
MENU3_AT(WorkC4560, field_B70, 0xB70);
MENU3_AT(WorkC4560, field_B7C, 0xB7C);
MENU3_AT(WorkC4560, field_B80, 0xB80);
MENU3_AT(WorkC4560, field_B84, 0xB84);

/* Screen in D_08AC4560. */
typedef struct ScreenC4560 {
    unsigned char block[0x100];
    MenuTask task;
    void *heap;
    void *field_138; /* released with func_0888208C */
    char pad_13C[0x10];
    WorkC4560 *work;
} ScreenC4560;
MENU3_AT(ScreenC4560, block, 0x0);
MENU3_AT(ScreenC4560, task, 0x100);
MENU3_AT(ScreenC4560, heap, 0x134);
MENU3_AT(ScreenC4560, field_138, 0x138);
MENU3_AT(ScreenC4560, work, 0x14C);

/* ------------------------------------------------------------ 0x08AC47C4..: the screen of func_08AC47C4 */

typedef struct WorkC47C4 {
    MenuLayout layout;
    MenuModel *handles[0x120];
    void *messages;
    char pad_884[0xFC];
    void *field_980; /* a page (func_08AC8A00) */
    short field_984[4];
    short field_98C[7][8];
    char pad_9FC[0x8];
    int field_A04;
} WorkC47C4;
MENU3_AT(WorkC47C4, layout, 0x0);
MENU3_AT(WorkC47C4, handles, 0x400);
MENU3_AT(WorkC47C4, messages, 0x880);
MENU3_AT(WorkC47C4, field_980, 0x980);
MENU3_AT(WorkC47C4, field_984, 0x984);
MENU3_AT(WorkC47C4, field_98C, 0x98C);
MENU3_AT(WorkC47C4, field_A04, 0xA04);

typedef struct ScreenC47C4 {
    unsigned char block[0x100];
    MenuTask task;
    void *heap;
    WorkC47C4 *work;
} ScreenC47C4;
MENU3_AT(ScreenC47C4, block, 0x0);
MENU3_AT(ScreenC47C4, task, 0x100);
MENU3_AT(ScreenC47C4, heap, 0x134);
MENU3_AT(ScreenC47C4, work, 0x138);

/* 0x08AB74E0..0x08AB7C98 */
void func_08AB6E70(ObjB41E0 *self);
void func_08AB7030(ObjB41E0 *self);
void func_08AB70BC(void);
void func_08AB70E8(ObjB41E0 *self);
void func_08AB74E0(void);
void func_08AB755C(LinkB41E0 *self);
void func_08AB7578(LinkB41E0 *self);
void func_08AB7594(LinkB41E0 *self);
void func_08AB75B8(LinkB41E0 *self);
void func_08AB75D4(LinkB41E0 *self);
void func_08AB75F0(LinkB41E0 *self);
void func_08AB760C(LinkB41E0 *self);
void func_08AB7628(LinkB41E0At8 *self);
ObjAB7C98 *func_08AB7C98(ObjAB7C98 *self);

/* 0x08AB8128..0x08AB9C18 */
unsigned char func_088F4518(int value, int arg);
void *func_0887AD78(void *object, int arg1, int arg2, int arg3, int arg4, char arg5);
void func_08AB7F40(ScreenBB9E4 *self);
void func_08AB7FE0(ScreenBB9E4 *self);
void func_08AB8128(ScreenBB9E4 *self, int index);
void func_08AB8190(ScreenBB9E4 *self, int arg1, int arg2, int arg3);
void func_08AB8768(unsigned int arg, unsigned short *out);
int func_08AB96B4(void);
void func_08AB9938(ScreenBB9E4 *self);
void func_08AB9A38(ScreenBB9E4 *self);
int func_08AB9B84(ScreenBB9E4 *self, int index);
int func_08AB9B98(ScreenBB9E4 *self);
void func_08AB9BB8(void);
void func_08AB9BD8(void);
void func_08AB9BF8(void);
void func_08AB9CB8(ScreenBB9E4 *self);
ScreenBB9E4 *func_08AB9C18(ScreenBB9E4 *self);

/* 0x08ABBB30..0x08ABF77C */
extern ScreenC4560 *D_08AC4560;
void func_08A2958C(void);
void func_088AC268(Global9D712C *object);
void func_08ABBB30(ScreenC4560 *self);
void func_08ABBC14(ScreenC4560 *self);
void *func_08ABBC4C(ScreenC4560 *self, int arg1, int arg2, int arg3, int arg4);
char *func_08ABBC80(ScreenC4560 *self, int id);
void func_08ABBCA8(ScreenC4560 *self);
void func_08ABBDDC(ScreenC4560 *self);
void func_08ABBE74(ScreenC4560 *self);
void func_08ABC0B8(ScreenC4560 *self);
void func_08ABC37C(ScreenC4560 *self);
void func_08ABC484(ScreenC4560 *self);
void func_08ABC510(ScreenC4560 *self);
void func_08ABC718(ScreenC4560 *self);
void func_08ABC75C(ScreenC4560 *self);
void func_08ABC7C8(ScreenC4560 *self);
void func_08ABC828(ScreenC4560 *self, int id);
int func_08ABC868(ScreenC4560 *self);
int func_08ABC89C(ScreenC4560 *self);
void func_08ABC8C4(ScreenC4560 *self);
int func_08ABC934(ScreenC4560 *self, unsigned int id);
void func_08ABC96C(unsigned int arg, unsigned short *out);
void func_08ABCBAC(ScreenC4560 *self, int value);
void func_08ABCCDC(ScreenC4560 *self);
void func_08ABCD40(ScreenC4560 *self);
void func_08ABCDE8(ScreenC4560 *self);
void func_08ABCE68(ScreenC4560 *self);
void func_08ABD234(ScreenC4560 *self);
void func_08ABD310(ScreenC4560 *self);
void func_08ABD604(ScreenC4560 *self);
void func_08ABE47C(ScreenC4560 *self);
void func_08ABEEA4(ScreenC4560 *self);
void func_08ABF054(ScreenC4560 *self);
void func_08ABF1DC(ScreenC4560 *self, void *arg);
void func_08ABF334(ScreenC4560 *self);
void func_08ABF388(void);
void func_08ABF3A8(void);
void func_08ABF3C8(void);
void func_08ABF3E8(void);
void func_08ABF408(void);
void func_08ABF428(void);
void func_08ABF448(void);
void func_08ABF468(void *arg);
int func_08ABF550(ScreenC4560 *self, void *arg);
ScreenC4560 *func_08ABF6F8(ScreenC4560 *self);
void func_08ABF77C(void *arg);
void func_08ABF7A0(ScreenC4560 *self);

/* 0x08AC47C4.. */
void func_088F10AC(int index, short *out);
char func_088F1094(void);
unsigned char func_08AC4930(ScreenC47C4 *self, short *values);
void func_08AC4990(ScreenC47C4 *self);
void func_08AC4AA4(ScreenC47C4 *self, int size, char *buffer);
void func_08AC4C00(ScreenC47C4 *self, int size, char *buffer);
void func_08AC4E44(ScreenC47C4 *self, int size, char *buffer);
void func_08AC53AC(ScreenC47C4 *self);
int func_08AC5514(ScreenC47C4 *self);
void func_08AC5708(ScreenC47C4 *self);
void func_08AC58D0(ScreenC47C4 *self);
void func_08AC5DB4(ScreenC47C4 *self);
void func_08AC5DDC(ScreenC47C4 *self, int arg1, int arg2, int arg3);
void func_08AC6540(void);
void func_08AC6560(void);
void func_08AC6920(ScreenC47C4 *self);

/* ------------------------------------------------------------ 0x08AC7578..0x08AC8A94: PanelC18 */

/* Data of PanelC18: the layout, handles and messages it is given
 * (func_08AC88B8), its texts and scripts. */
typedef struct WorkC18 {
    MenuLayout *layout;
    MenuModel **handles;
    void *messages;
    void *texts[0x15]; /* length not known */
    char pad_60[0xB0];
    unsigned char field_110[0x20];
    char pad_130[0xA80];
    unsigned short field_BB0[0x14]; /* a text script (length not known) */
    const short *field_BD8; /* layout scripts played when set */
    const short *field_BDC;
    const unsigned short *field_BE0; /* a text script played when set */
    char pad_BE4[0x1C];
    unsigned int flags;
    char pad_C04[0x14];
} WorkC18;
MENU3_SIZE(WorkC18, 0xC18);
MENU3_AT(WorkC18, layout, 0x0);
MENU3_AT(WorkC18, handles, 0x4);
MENU3_AT(WorkC18, messages, 0x8);
MENU3_AT(WorkC18, texts, 0xC);
MENU3_AT(WorkC18, field_110, 0x110);
MENU3_AT(WorkC18, field_BB0, 0xBB0);
MENU3_AT(WorkC18, field_BD8, 0xBD8);
MENU3_AT(WorkC18, field_BDC, 0xBDC);
MENU3_AT(WorkC18, field_BE0, 0xBE0);
MENU3_AT(WorkC18, flags, 0xC00);

/* A panel whose work pointer points back at its own first 0xC18 bytes (constructor func_08AC87F4). */
typedef struct PanelC18 {
    WorkC18 data;
    WorkC18 *work;
} PanelC18;
MENU3_AT(PanelC18, data, 0x0);
MENU3_AT(PanelC18, work, 0xC18);

/* ------------------------------------------------------------ 0x08ACA688..0x08AD11A0: ScreenDB30 */

typedef struct RecC6 {
    char pad_00[0xC6];
} RecC6;
MENU3_SIZE(RecC6, 0xC6);

/* Work block of the screen in D_08B01CF8 (+0x140). */
typedef struct WorkB01CF8 {
    void *field_00;
    char pad_04[0x574];
    MenuModel *field_578;
    char pad_57C[0x320];
    void *messages;
    void *field_8A0; /* a second messages object */
    char pad_8A4[0xE4];
    void *field_988; /* a text */
    void *texts[1]; /* length not known */
    char pad_990[0x1B0];
    unsigned int flags;
    int field_B44;
    char pad_B48[0xB4];
    int field_BFC;
    int field_C00;
    char pad_C04[0x8];
    unsigned int field_C0C[3];
    int field_C18;
    char pad_C1C[0x10];
    int field_C2C;
    int field_C30;
    char pad_C34[0x4C];
    RecC6 records[1]; /* length not known */
    char pad_D46[0x1EB6];
    int field_2BFC;
    char pad_2C00[0x5C];
    int field_2C5C;
    unsigned char field_2C60;
    char pad_2C61[0x3];
    unsigned int field_2C64[1];
    char pad_2C68[0xBC];
    int field_2D24;
    int field_2D28;
    int field_2D2C;
    char pad_2D30[0xA4];
    int field_2DD4;
    MenuPrompt prompt;
    unsigned short field_2E04;
    char pad_2E06[0x8E];
    int field_2E94;
    void *field_2E98; /* blocks from the screen's heap */
    void *field_2E9C;
    void *field_2EA0;
} WorkB01CF8;
MENU3_AT(WorkB01CF8, field_00, 0x0);
MENU3_AT(WorkB01CF8, field_578, 0x578);
MENU3_AT(WorkB01CF8, messages, 0x89C);
MENU3_AT(WorkB01CF8, field_8A0, 0x8A0);
MENU3_AT(WorkB01CF8, field_988, 0x988);
MENU3_AT(WorkB01CF8, texts, 0x98C);
MENU3_AT(WorkB01CF8, flags, 0xB40);
MENU3_AT(WorkB01CF8, field_B44, 0xB44);
MENU3_AT(WorkB01CF8, field_BFC, 0xBFC);
MENU3_AT(WorkB01CF8, field_C00, 0xC00);
MENU3_AT(WorkB01CF8, field_C0C, 0xC0C);
MENU3_AT(WorkB01CF8, field_C18, 0xC18);
MENU3_AT(WorkB01CF8, field_C2C, 0xC2C);
MENU3_AT(WorkB01CF8, field_C30, 0xC30);
MENU3_AT(WorkB01CF8, records, 0xC80);
MENU3_AT(WorkB01CF8, field_2BFC, 0x2BFC);
MENU3_AT(WorkB01CF8, field_2C5C, 0x2C5C);
MENU3_AT(WorkB01CF8, field_2C60, 0x2C60);
MENU3_AT(WorkB01CF8, field_2C64, 0x2C64);
MENU3_AT(WorkB01CF8, field_2D24, 0x2D24);
MENU3_AT(WorkB01CF8, field_2D28, 0x2D28);
MENU3_AT(WorkB01CF8, field_2D2C, 0x2D2C);
MENU3_AT(WorkB01CF8, field_2DD4, 0x2DD4);
MENU3_AT(WorkB01CF8, prompt, 0x2DD8);
MENU3_AT(WorkB01CF8, field_2E04, 0x2E04);
MENU3_AT(WorkB01CF8, field_2E94, 0x2E94);
MENU3_AT(WorkB01CF8, field_2E98, 0x2E98);
MENU3_AT(WorkB01CF8, field_2E9C, 0x2E9C);
MENU3_AT(WorkB01CF8, field_2EA0, 0x2EA0);

/* Screen in D_08B01CF8 (declared void * in auto_region1.h). */
typedef struct ScreenB01CF8 {
    unsigned char block[0x100];
    MenuTask task;
    char pad_134[0x4];
    void *heap;
    char pad_13C[0x4];
    WorkB01CF8 *work;
} ScreenB01CF8;
MENU3_AT(ScreenB01CF8, block, 0x0);
MENU3_AT(ScreenB01CF8, task, 0x100);
MENU3_AT(ScreenB01CF8, heap, 0x138);
MENU3_AT(ScreenB01CF8, work, 0x140);

/* Work block of ScreenDB30 (+0xDB30). */
typedef struct WorkDB30 {
    MenuLayout layout;
    MenuModel *handles[0x120];
    void *texts[1]; /* length not known */
    char pad_884[0x694];
    unsigned int flags;
    char pad_F1C[0xC];
    int field_F28;
    char pad_F2C[0x50];
    MenuCursorView cursor;
    char pad_FA8[0x4];
    int field_FAC;
    char pad_FB0[0x8];
    int field_FB8;
    char pad_FBC[0x50];
    unsigned char field_100C[0xE4]; /* a list for func_088D4788 */
    int field_10F0;
    char pad_10F4[0x4];
    unsigned char field_10F8[0xE4];
    int field_11DC;
    char pad_11E0[0x30];
    int field_1210;
    char pad_1214[0x10];
    int field_1224;
    int field_1228;
    int field_122C;
    char pad_1230[0x1E8];
    int field_1418;
    int field_141C;
    int field_1420;
    char pad_1424[0x96A4];
    unsigned char field_AAC8[4];
    char pad_AACC[0x2000];
    int field_CACC;
    char pad_CAD0[0x1038];
    int field_DB08; /* count of texts loading (func_08AD0934 / func_08AD0990) */
    char pad_DB0C[0x4];
    int field_DB10;
} WorkDB30;
MENU3_AT(WorkDB30, layout, 0x0);
MENU3_AT(WorkDB30, handles, 0x400);
MENU3_AT(WorkDB30, texts, 0x880);
MENU3_AT(WorkDB30, flags, 0xF18);
MENU3_AT(WorkDB30, field_F28, 0xF28);
MENU3_AT(WorkDB30, cursor, 0xF7C);
MENU3_AT(WorkDB30, field_FAC, 0xFAC);
MENU3_AT(WorkDB30, field_FB8, 0xFB8);
MENU3_AT(WorkDB30, field_100C, 0x100C);
MENU3_AT(WorkDB30, field_10F0, 0x10F0);
MENU3_AT(WorkDB30, field_10F8, 0x10F8);
MENU3_AT(WorkDB30, field_11DC, 0x11DC);
MENU3_AT(WorkDB30, field_1210, 0x1210);
MENU3_AT(WorkDB30, field_1224, 0x1224);
MENU3_AT(WorkDB30, field_1228, 0x1228);
MENU3_AT(WorkDB30, field_122C, 0x122C);
MENU3_AT(WorkDB30, field_1418, 0x1418);
MENU3_AT(WorkDB30, field_141C, 0x141C);
MENU3_AT(WorkDB30, field_1420, 0x1420);
MENU3_AT(WorkDB30, field_AAC8, 0xAAC8);
MENU3_AT(WorkDB30, field_CACC, 0xCACC);
MENU3_AT(WorkDB30, field_DB08, 0xDB08);
MENU3_AT(WorkDB30, field_DB10, 0xDB10);

/* Screen with vtable D_08AD8E50 (deleting destructor func_08AD0ED0). */
typedef struct ScreenDB30 {
    void *vtable;
    char pad_04[0xDB2C];
    WorkDB30 *work;
} ScreenDB30;
MENU3_AT(ScreenDB30, vtable, 0x0);
MENU3_AT(ScreenDB30, work, 0xDB30);

/* ------------------------------------------------------------ 0x08ADA9C4..0x08ADCBFC: Screen46AC */

typedef struct Work46AC {
    MenuLayout layout;
    MenuModel *handles[0x120];
    void *texts[1]; /* length not known */
    char pad_884[0x2DAC];
    short field_3630[4]; /* length not known */
} Work46AC;
MENU3_AT(Work46AC, layout, 0x0);
MENU3_AT(Work46AC, handles, 0x400);
MENU3_AT(Work46AC, texts, 0x880);
MENU3_AT(Work46AC, field_3630, 0x3630);

/* Screen with vtable D_08ADF498 (deleting destructor func_08ADCBFC). */
typedef struct Screen46AC {
    void *vtable;
    char pad_04[0x46A8];
    Work46AC *work;
} Screen46AC;
MENU3_AT(Screen46AC, vtable, 0x0);
MENU3_AT(Screen46AC, work, 0x46AC);

/* ------------------------------------------------------------ 0x08ADFD8C..: Screen850 */

typedef struct Work850 {
    MenuLayout layout;
    MenuModel *handles[0x32];
    char pad_4C8[0x88];
    void *texts[0x11];
    char pad_594[0x4];
    int state;
    int next;
    char pad_5A0[0x20];
    int field_5C0;
    char pad_5C4[0x4];
    int field_5C8;
    char pad_5CC[0x8];
    short field_5D4[0x28]; /* indexed by func_08AE150C's result */
    short field_624[0x28]; /* length not known */
} Work850;
MENU3_AT(Work850, layout, 0x0);
MENU3_AT(Work850, handles, 0x400);
MENU3_AT(Work850, texts, 0x550);
MENU3_AT(Work850, state, 0x598);
MENU3_AT(Work850, next, 0x59C);
MENU3_AT(Work850, field_5C0, 0x5C0);
MENU3_AT(Work850, field_5C8, 0x5C8);
MENU3_AT(Work850, field_5D4, 0x5D4);
MENU3_AT(Work850, field_624, 0x624);

/* Screen with vtable D_08AE4550 (deleting destructor func_08AE29B0). */
typedef struct Screen850 {
    void *vtable;
    char pad_04[0x84C];
    Work850 *work;
} Screen850;
MENU3_AT(Screen850, vtable, 0x0);
MENU3_AT(Screen850, work, 0x850);

/* 0x08AC60B4..0x08AC65A0 (ScreenC47C4) */
void func_08AC4688(ScreenC47C4 *self);
void func_08AC4724(ScreenC47C4 *self);
void func_08AC593C(ScreenC47C4 *self);
void func_08AC60B4(ScreenC47C4 *self);
void func_08AC63D4(ScreenC47C4 *self);
void func_08AC647C(ScreenC47C4 *self);
void func_08AC64A8(ScreenC47C4 *self);
void func_08AC6500(void);
void func_08AC6520(void);
void func_08AC6580(void);
ScreenC47C4 *func_08AC65A0(ScreenC47C4 *self);
void func_08AC6640(ScreenC47C4 *self);
unsigned char func_08AC8A00(void *page);

/* 0x08AC7578..0x08AC8A94 (PanelC18) */
void func_08AC7578(PanelC18 *self);
void func_08AC7620(PanelC18 *self);
void func_08AC7700(unsigned int arg, unsigned short *out);
void func_08AC77B4(PanelC18 *self, int arg1, int arg2, int arg3);
void func_08AC7884(PanelC18 *self, int id);
void func_08AC78AC(PanelC18 *self, int id);
void func_08AC8474(PanelC18 *self);
PanelC18 *func_08AC87F4(PanelC18 *self);
void func_08AC888C(void *block);
void func_08AC88B8(PanelC18 *self, MenuLayout *layout, MenuModel **handles, void *messages);
void func_08AC8A94(PanelC18 *self, int arg1, int arg2, int arg3);

/* 0x08ACA688..0x08AD11A0 (ScreenDB30) */
void func_088D436C(void *handles, void *texts, const unsigned short *script);
void func_088D442C(void *handles, void *texts, const unsigned short *script);
unsigned char func_088D4788(void *cursor, void *list, int count);
void func_088B745C(void *record);
int func_0887BA80(void *text, void *callback, void *arg);
void func_08ACA564(ScreenDB30 *self);
void func_08ACA688(ScreenDB30 *self);
void func_08ACA704(ScreenDB30 *self);
void func_08ACA768(ScreenDB30 *self);
void func_08ACAC94(ScreenDB30 *self);
void func_08ACADD4(ScreenDB30 *self);
void func_08ACB188(ScreenDB30 *self);
void func_08ACB1FC(ScreenDB30 *self);
void func_08ACB360(ScreenDB30 *self);
void func_08ACB3AC(ScreenDB30 *self, int index, int arg);
void func_08ACB474(ScreenDB30 *self);
void func_08ACB4E4(ScreenDB30 *self);
void func_08ACB680(ScreenDB30 *self);
void func_08ACBFCC(ScreenDB30 *self);
void func_08ACC020(ScreenDB30 *self);
void func_08ACC05C(ScreenDB30 *self);
void func_08ACC098(ScreenDB30 *self);
void func_08ACCB88(ScreenDB30 *self);
void func_08ACCD48(ScreenDB30 *self);
void func_08ACD078(ScreenDB30 *self);
void func_08ACD184(ScreenDB30 *self);
int func_08ACD768(ScreenDB30 *self, int index);
int func_08ACD8C4(ScreenDB30 *self, int index);
void func_08ACE24C(ScreenDB30 *self);
void func_08ACE27C(ScreenDB30 *self, char arg);
int func_08ACE360(ScreenDB30 *self);
void func_08ACE3D0(ScreenDB30 *self);
int func_08ACE74C(ScreenDB30 *self, char arg1, short *arg2);
void func_08ACEE3C(void);
void func_08ACEEDC(ScreenDB30 *self);
void func_08ACEF7C(void);
void func_08ACEFA4(ScreenDB30 *self);
void func_08ACF008(ScreenDB30 *self);
void func_08ACF0BC(ScreenDB30 *self);
void func_08ACF434(ScreenDB30 *self);
void func_08ACF548(ScreenDB30 *self);
int func_08ACF738(ScreenDB30 *self);
int func_08ACF7C4(ScreenDB30 *self);
int func_08ACF81C(ScreenDB30 *self, int value, int arg);
int func_08ACFFCC(ScreenDB30 *self);
void func_08AD00F8(ScreenDB30 *self);
void func_08AD0240(ScreenDB30 *self);
void func_08AD0584(ScreenDB30 *self);
void func_08AD062C(ScreenDB30 *self);
void func_08AD0934(ScreenDB30 *self, void *text);
void func_08AD0990(ScreenDB30 *self, void *arg);
void func_08AD09E8(ScreenDB30 *self);
void func_08AD0A58(ScreenDB30 *self);
void func_08AD0B2C(ScreenDB30 *self);
void func_08AD0BA4(ScreenDB30 *self);
void func_08AD0ED0(ScreenDB30 *self, unsigned int flags);
void func_08AD11A0(ScreenDB30 *self);
void func_08AD1514(ScreenDB30 *self, char arg);
void func_08AD40C0(ScreenDB30 *self);
void func_08AD6698(ScreenDB30 *self);
void func_08AD6D24(ScreenDB30 *self);
void func_08AF1244(void *self, unsigned int flags);
void func_08AF12B0(void *block);
void func_08AF3ECC(void *object);
void func_08AF4B2C(void *object);
void func_08AF4B6C(void *object, void *out, int arg);
void func_08AF4E1C(ScreenB01CF8 *self);
void func_08AFD194(void *object, char arg1, unsigned int arg2);

/* 0x08ADA9C4..0x08ADCBFC (Screen46AC) */
void func_08ADA9C4(Screen46AC *self);
void func_08ADAA1C(Screen46AC *self);
void func_08ADAA5C(Screen46AC *self);
void func_08ADAA9C(Screen46AC *self);
void func_08ADAAF0(Screen46AC *self);
void func_08ADAB38(Screen46AC *self);
void func_08ADAED4(Screen46AC *self);
void func_08ADB5C0(Screen46AC *self);
void func_08ADB610(Screen46AC *self);
void func_08ADB65C(Screen46AC *self);
void func_08ADB6F8(Screen46AC *self);
void func_08ADB734(Screen46AC *self);
void func_08ADB75C(Screen46AC *self);
void func_08ADB784(Screen46AC *self);
void func_08ADC9D4(Screen46AC *self);
void func_08ADCAB8(Screen46AC *self);
void func_08ADCBFC(Screen46AC *self, unsigned int flags);
void func_08ADCFD8(Screen46AC *self, char arg);
void func_08ADD910(Screen46AC *self);
void func_08AF55F0(void *object, short *values);
void func_08AF569C(void *object, short *values);
void func_08AF5858(void *object, short *values);

/* 0x08ADFD8C.. (Screen850) */
int func_08ADFD8C(Screen850 *self);
void func_08ADFF7C(Screen850 *self);
void func_08AE0F70(Screen850 *self, int arg1, int arg2);
void func_08AE20B0(Screen850 *self);
const short *func_08AE46B0(void);
void func_08AF6DB8(ScreenB01CF8 *self);

/* ------------------------------------------------------------ 0x08AE4D70..0x08AE7F94: ScreenA690 */

typedef struct WorkA690 {
    MenuLayout layout;
    MenuModel *handles[0x120];
    void *texts[1]; /* length not known */
    char pad_884[0x1CD4];
    unsigned int flags;
    char pad_255C[0x110];
    int field_266C;
    char pad_2670[0x6F54];
    short field_95C4[4]; /* length not known */
    char pad_95CC[0x70];
    unsigned char field_963C[4];
} WorkA690;
MENU3_AT(WorkA690, layout, 0x0);
MENU3_AT(WorkA690, handles, 0x400);
MENU3_AT(WorkA690, texts, 0x880);
MENU3_AT(WorkA690, flags, 0x2558);
MENU3_AT(WorkA690, field_266C, 0x266C);
MENU3_AT(WorkA690, field_95C4, 0x95C4);
MENU3_AT(WorkA690, field_963C, 0x963C);

/* Screen with vtable D_08AEB118 (deleting destructor func_08AE7F94). */
typedef struct ScreenA690 {
    void *vtable;
    char pad_04[0xA68C];
    WorkA690 *work;
} ScreenA690;
MENU3_AT(ScreenA690, vtable, 0x0);
MENU3_AT(ScreenA690, work, 0xA690);

/* ------------------------------------------------------------ 0x08AEBEFC..0x08AED3D4: Screen23F0 */

typedef struct Work23F0 {
    MenuLayout layout;
    MenuModel *handles[0x120];
    char pad_880[0x10A4];
    unsigned int flags;
    char pad_1928[0x34];
    MenuCursor cursor;
    char pad_1988[0x30];
    int field_19B8;
} Work23F0;
MENU3_AT(Work23F0, layout, 0x0);
MENU3_AT(Work23F0, handles, 0x400);
MENU3_AT(Work23F0, flags, 0x1924);
MENU3_AT(Work23F0, cursor, 0x195C);
MENU3_AT(Work23F0, field_19B8, 0x19B8);

/* Screen with vtable D_08AEF718 (deleting destructor func_08AED3D4). */
typedef struct Screen23F0 {
    void *vtable;
    char pad_04[0x23EC];
    Work23F0 *work;
} Screen23F0;
MENU3_AT(Screen23F0, vtable, 0x0);
MENU3_AT(Screen23F0, work, 0x23F0);

/* ------------------------------------------------------------ 0x08AEFDE0..0x08AF12B0 */

/* Constructor func_08AEFDE0 (vtable D_08AF00A0). */
typedef struct ObjEFDE0 {
    void *vtable;
    int field_04;
    int field_08;
} ObjEFDE0;
MENU3_SIZE(ObjEFDE0, 0xC);
MENU3_AT(ObjEFDE0, vtable, 0x0);
MENU3_AT(ObjEFDE0, field_04, 0x4);
MENU3_AT(ObjEFDE0, field_08, 0x8);

typedef struct Point8 {
    int x;
    int y;
} Point8;
MENU3_SIZE(Point8, 0x8);
MENU3_AT(Point8, x, 0x0);
MENU3_AT(Point8, y, 0x4);

typedef struct PanelAF08A4 {
    char pad_00[0x70];
    void **texts; /* three texts */
    char pad_74[0xEC];
    MenuModel *field_160;
} PanelAF08A4;
MENU3_AT(PanelAF08A4, texts, 0x70);
MENU3_AT(PanelAF08A4, field_160, 0x160);

/* Base of the panels with a vtable at 0 (constructor func_08AF1230,
 * destructor func_08AF1244; func_08AF12B0 frees them). */
typedef struct PanelBase {
    void *vtable;
} PanelBase;
MENU3_AT(PanelBase, vtable, 0x0);

/* ------------------------------------------------------------ 0x08AF50E0..0x08AF63CC: panel callbacks */

/* Second argument of func_08AF50E0. */
typedef struct PanelCtx50E0 {
    char pad_00[0x10];
    void *texts;
    char pad_14[0x4];
    void *messages;
} PanelCtx50E0;
MENU3_AT(PanelCtx50E0, texts, 0x10);
MENU3_AT(PanelCtx50E0, messages, 0x18);

/* Second argument of func_08AF55B8 / func_08AF55C4 / func_08AF58E8 / func_08AF5C28. */
typedef struct PanelCtx55C4 {
    char pad_00[0x8];
    MenuLayout *layout;
    MenuModel **handles;
    char pad_10[0x10];
    const short *script;
    char pad_24[0x4];
    int state;
} PanelCtx55C4;
MENU3_AT(PanelCtx55C4, layout, 0x8);
MENU3_AT(PanelCtx55C4, handles, 0xC);
MENU3_AT(PanelCtx55C4, script, 0x20);
MENU3_AT(PanelCtx55C4, state, 0x28);

/* Second argument of func_08AF5D20..func_08AF63CC. */
typedef struct PanelCtx5D20 {
    char pad_00[0xC];
    MenuLayout *layout;
    MenuModel **handles;
    void **texts;
    char pad_18[0x2C];
    const short *field_44; /* closing layout script */
    char pad_48[0xC];
    const unsigned short *field_54; /* closing text script */
} PanelCtx5D20;
MENU3_AT(PanelCtx5D20, layout, 0xC);
MENU3_AT(PanelCtx5D20, handles, 0x10);
MENU3_AT(PanelCtx5D20, texts, 0x14);
MENU3_AT(PanelCtx5D20, field_44, 0x44);
MENU3_AT(PanelCtx5D20, field_54, 0x54);

/* 0x08ADFD8C..0x08AE29B0 (Screen850) */
void func_08AE0638(Screen850 *self);
void func_08AE09D8(Screen850 *self, int index, int value);
void func_08AE12AC(Screen850 *self, int arg1, int arg2, int arg3);
int func_08AE150C(Screen850 *self);
int func_08AE1544(Screen850 *self);
int func_08AE1578(Screen850 *self);
void func_08AE1748(Screen850 *self, int arg1, int arg2);
void func_08AE1BA8(Screen850 *self, int arg);
void func_08AE2018(Screen850 *self, int kind);
void func_08AE283C(Screen850 *self);
void func_08AE29B0(Screen850 *self, unsigned int flags);
const short *func_08AE46BC(void);
const short *func_08AE4708(void);
const short *func_08AE4714(void);

/* 0x08AE4D70..0x08AE7F94 (ScreenA690) */
void func_08AE4CC0(ScreenA690 *self);
void func_08AE4D70(ScreenA690 *self);
void func_08AE4DD0(ScreenA690 *self);
void func_08AE4E18(ScreenA690 *self);
void func_08AE4E60(ScreenA690 *self);
void func_08AE4EBC(ScreenA690 *self);
void func_08AE4F0C(ScreenA690 *self);
void func_08AE5228(ScreenA690 *self, char arg);
void func_08AE5E7C(ScreenA690 *self);
void func_08AE5ED0(ScreenA690 *self);
void func_08AE5F20(ScreenA690 *self);
void func_08AE5F70(ScreenA690 *self);
void func_08AE6028(ScreenA690 *self);
void func_08AE6070(ScreenA690 *self);
void func_08AE60A4(ScreenA690 *self);
void func_08AE60D8(ScreenA690 *self);
void func_08AE61A8(ScreenA690 *self);
void func_08AE638C(ScreenA690 *self);
void func_08AE656C(ScreenA690 *self);
void func_08AE65C4(ScreenA690 *self);
void func_08AE6828(ScreenA690 *self);
void func_08AE6A20(ScreenA690 *self);
void func_08AE797C(ScreenA690 *self);
void func_08AE7A8C(ScreenA690 *self);
void func_08AE7E38(ScreenA690 *self);
void func_08AE7E6C(ScreenA690 *self);
void func_08AE7EA0(ScreenA690 *self);
void func_08AE7F94(ScreenA690 *self, unsigned int flags);
void func_08AE8758(ScreenA690 *self);
void func_08AF51B8(void *object, short *values);

/* 0x08AEBEFC..0x08AED3D4 (Screen23F0) */
void func_08AEBEFC(Screen23F0 *self);
void func_08AEBF6C(Screen23F0 *self);
void func_08AEBFDC(Screen23F0 *self);
void func_08AEC0C8(Screen23F0 *self);
void func_08AEC3EC(Screen23F0 *self);
void func_08AEC700(Screen23F0 *self, int value);
void func_08AECBCC(Screen23F0 *self);
void func_08AECD10(Screen23F0 *self);
void func_08AED204(Screen23F0 *self);
void func_08AED2E8(Screen23F0 *self);
void func_08AED3D4(Screen23F0 *self, unsigned int flags);
void func_08AEDBE8(Screen23F0 *self);

/* 0x08AEFDE0..0x08AF12B0 */
ObjEFDE0 *func_08AEFDE0(ObjEFDE0 *self, int arg1, int arg2);
void func_08AF02F8(Point8 *out, int column, int row);
void func_08AF0878(void *block);
void func_08AF08A4(MenuLayout *layout, MenuModel **handles);
void func_08AF08CC(PanelAF08A4 *self);
void func_08AF08F0(PanelAF08A4 *self);
void func_08AF091C(PanelAF08A4 *self);
PanelBase *func_08AF1230(PanelBase *self);

/* 0x08AF1750..0x08AF7124 (ScreenB01CF8) */
void func_088D5638(void *object, MenuLayout *layout, MenuModel **handles, unsigned char arg3, const unsigned short *script,
                   unsigned int arg5, unsigned int arg6, int arg7, unsigned int arg8);
unsigned int func_088D5CE8(void);
int func_089203BC(void *object);
void func_088CBCB4(char *text);
void func_08AF1750(ScreenB01CF8 *self);
void func_08AF17EC(ScreenB01CF8 *self);
void func_08AF2400(ScreenB01CF8 *self);
void func_08AF2CB0(ScreenB01CF8 *self);
void func_08AF2F80(void);
void func_08AF3698(ScreenB01CF8 *self, MenuLayout *layout, MenuModel **handles);
void func_08AF371C(ScreenB01CF8 *self, MenuLayout *layout, MenuModel **handles);
void func_08AF37A4(ScreenB01CF8 *self, MenuLayout *layout, MenuModel **handles, int arg, const unsigned short *script);
void func_08AF37F4(ScreenB01CF8 *self, MenuLayout *layout, MenuModel **handles);
void func_08AF3824(ScreenB01CF8 *self, MenuLayout *layout, MenuModel **handles, unsigned char arg,
                   const unsigned short *script);
void func_08AF385C(ScreenB01CF8 *self, MenuLayout *layout, MenuModel **handles, int arg);
void func_08AF3930(ScreenB01CF8 *self, MenuLayout *layout, MenuModel **handles);
void func_08AF39B4(ScreenB01CF8 *self, int arg1, int arg2, int arg3, int arg4);
void func_08AF39E8(ScreenB01CF8 *self, int kind, int arg1, int arg2, int arg3, int arg4);
void func_08AF3A08(ScreenB01CF8 *self, int id);
void func_08AF3A30(ScreenB01CF8 *self, int kind, int id);
void func_08AF3A50(ScreenB01CF8 *self, int kind, int arg1, int arg2, int arg3, int arg4);
void func_08AF3A70(ScreenB01CF8 *self, int kind, int id);
void func_08AF3A90(ScreenB01CF8 *self, int value);
void func_08AF3BE8(ScreenB01CF8 *self);
int func_08AF4054(ScreenB01CF8 *self);
void func_08AF4224(void);
void func_08AF488C(ScreenB01CF8 *self);
void func_08AF48CC(ScreenB01CF8 *self);
void func_08AF4A94(ScreenB01CF8 *self);
void func_08AF4E08(ScreenB01CF8 *self, int arg1, int arg2);
void func_08AF4E64(ScreenB01CF8 *self, unsigned int *arg1, unsigned int *arg2);
void func_08AF4E98(ScreenB01CF8 *self, unsigned char value);
void func_08AF50E0(void *self, PanelCtx50E0 *ctx);
void func_08AF55B8(void *self, PanelCtx55C4 *ctx);
void func_08AF55C4(void *self, PanelCtx55C4 *ctx);
void func_08AF55E8(void);
void func_08AF58E8(void *self, PanelCtx55C4 *ctx);
void func_08AF58F4(void);
void func_08AF5C28(void *self, PanelCtx55C4 *ctx);
void func_08AF5D20(void *self, PanelCtx5D20 *ctx, int motion);
void func_08AF5D48(void *self, PanelCtx5D20 *ctx, int motion);
void func_08AF5EC8(void *self, PanelCtx5D20 *ctx);
void func_08AF5FFC(void *self, PanelCtx5D20 *ctx);
void func_08AF6060(void *self, PanelCtx5D20 *ctx);
void func_08AF61CC(void *self, PanelCtx5D20 *ctx);
void func_08AF63CC(void *self, PanelCtx5D20 *ctx);
void func_08AF6D3C(ScreenB01CF8 *self, void *entry, void *model, int arg);
void func_08AF7084(ScreenB01CF8 *self);
void func_08AF70A4(ScreenB01CF8 *self);
void func_08AF70C4(ScreenB01CF8 *self);
void func_08AF70E4(ScreenB01CF8 *self);
void func_08AF7104(ScreenB01CF8 *self);
void func_08AF7124(ScreenB01CF8 *self);
unsigned int func_08AF96B0(void);
char *func_08AF9720(char *buffer, void *object);
int func_08AF9F1C(ScreenB01CF8 *self);
unsigned char func_08AF9F94(ScreenB01CF8 *self);
unsigned char func_08AF9FB4(ScreenB01CF8 *self);
void func_08AFA790(void);
void func_08AFA7B0(void);

#ifdef __cplusplus
}
#endif

#endif /* DISSIDIA_MENU_AUTO_REGION3_H */
