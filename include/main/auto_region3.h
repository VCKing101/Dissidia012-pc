#ifndef DISSIDIA_MAIN_AUTO_REGION3_H
#define DISSIDIA_MAIN_AUTO_REGION3_H

/* Types and prototypes behind the automatically matched functions in main
 * 0x0888B080..0x088D96AC (readability pass, main region 3). Builds on
 * auto_region2.h (and through it auto_region1.h) and follows their naming:
 * classes with a vtable pointer are named after the vtable (Obj9CA758 stores
 * D_089CA758), others after the first function that shows their layout
 * (Task888B8BC), objects reached through a global pointer after the global
 * (Mem9D6AC0 is *D_089D6AC0). Fields whose use is not plain keep their offset
 * as their name. Every layout carries compile-time size/offset assertions
 * (MAIN3_SIZE / MAIN3_AT).
 *
 * Functions that auto_region2.h already declares keep that declaration; where
 * it takes `void *self`, the definition here takes `void *` too and works
 * through a typed local. */

#include "main/auto_region2.h"

#define MAIN3_OFFSET(type, field) ((unsigned int)&((type *)0)->field)
#define MAIN3_ASSERT(name, cond) typedef char main3_assert_##name[(cond) ? 1 : -1]
#define MAIN3_SIZE(type, size) MAIN3_ASSERT(size_##type, sizeof(type) == (size))
#define MAIN3_AT(type, field, offset) \
    MAIN3_ASSERT(at_##type##_##field, MAIN3_OFFSET(type, field) == (offset))

/* ------------------------------------------------------------ small shared shapes */

/* An object whose only visible field is its vtable pointer, at +0. */
typedef struct VtableOnly {
    void *vtable;
} VtableOnly;
MAIN3_SIZE(VtableOnly, 0x4);
MAIN3_AT(VtableOnly, vtable, 0x0);

/* A callback block (0x34 bytes): built by func_08864FF0, its callbacks set by
 * func_0886503C, its argument by func_0886504C, started by func_088650A4 and
 * waited for by func_08865158 (see also include/main/callback_runtime.h). */
typedef void (*BlockCallback)(void);
typedef struct CallbackBlock {
    int field_00[6];
    BlockCallback callbacks[3];
    char field_24;
    unsigned char field_25;
    unsigned char field_26;
    char pad_27;
    int field_28;
    int field_2C;
    const void *field_30;
} CallbackBlock;
MAIN3_SIZE(CallbackBlock, 0x34);
MAIN3_AT(CallbackBlock, field_00, 0x0);
MAIN3_AT(CallbackBlock, callbacks, 0x18);
MAIN3_AT(CallbackBlock, field_24, 0x24);
MAIN3_AT(CallbackBlock, field_28, 0x28);
MAIN3_AT(CallbackBlock, field_2C, 0x2C);
MAIN3_AT(CallbackBlock, field_30, 0x30);

CallbackBlock *func_08864FF0(CallbackBlock *self, int arg1, int arg2, char arg3);
void func_0886503C(CallbackBlock *self, BlockCallback first, BlockCallback second, BlockCallback third);
void func_0886504C(CallbackBlock *self, void *arg, unsigned int size);
void func_088650A4(CallbackBlock *self);

/* ------------------------------------------------------------ 0x0888B080..0x08892584 */

unsigned char func_0888B080(void);
void func_0888B3A4(void);

/* A task with a callback block at +0x100 (D_089D6A14 points at one). */
typedef struct Task888B8BC {
    char pad_00[0x100];
    CallbackBlock block;
    unsigned char field_134;
} Task888B8BC;
MAIN3_AT(Task888B8BC, block, 0x100);
MAIN3_AT(Task888B8BC, field_134, 0x134);

void func_08889808(Flow9D6A10 *self);
void func_0888B804(Task888B8BC *self);
void func_0888B89C(void);
Task888B8BC *func_0888B8BC(Task888B8BC *self);
void func_0888B858(void);
void func_0888B894(void);

/* Overlay calls on the object D_08B54330. */
extern void *D_08B54330;
int func_08B53454(void *self);
int func_08B5354C(void *self);
int func_08B535C4(void *self);
int func_08B5363C(void *self);
void func_0888B98C(void);
void func_0888B9AC(void);
void func_0888B9CC(void);
void func_0888B9EC(void);

/* Globals released by func_0888E5D0. */
extern int D_089D6A90;
extern int D_089D6A94;          /* -1 when unset */
extern void *D_089D6AE8, *D_089D6AF0, *D_089D6C80;
extern Holder D_09B491E0;       /* a static object */
extern int D_089D6A98;          /* its destructor entry */
int func_088A0DFC(void *self, int handle);
void func_08896AB0(void *self);
void func_08895F18(void *self);
void func_088A0D1C(int arg);
void func_0888E5D0(void);
void func_0888E65C(void);

struct Task888E6A8;

/* Vtable D_089CA5B0; a member of Task888E6A8 that points back at it. */
typedef struct Obj9CA5B0 {
    void *vtable;
    struct Task888E6A8 *owner;
} Obj9CA5B0;
MAIN3_SIZE(Obj9CA5B0, 0x8);
MAIN3_AT(Obj9CA5B0, vtable, 0x0);
MAIN3_AT(Obj9CA5B0, owner, 0x4);

/* A task with a callback block at +0x100 (D_089D6AA8 points at one). */
typedef struct Task888E6A8 {
    char pad_00[0x100];
    CallbackBlock block;
    int field_134;
    int field_138;
    Obj9CA5B0 link;
} Task888E6A8;
MAIN3_SIZE(Task888E6A8, 0x144);
MAIN3_AT(Task888E6A8, block, 0x100);
MAIN3_AT(Task888E6A8, field_134, 0x134);
MAIN3_AT(Task888E6A8, field_138, 0x138);
MAIN3_AT(Task888E6A8, link, 0x13C);

extern int D_089CA5B0;
Task888E6A8 *func_0888E6A8(Task888E6A8 *self);
void func_0888E6F8(Task888E6A8 *self);
void func_0888E72C(void *self);
void func_0888E79C(Task888E6A8 *self);
void func_0888E9A8(Task888E6A8 *self);
void func_0888E9B4(Task888E6A8 *self);
void func_0888E9C0(void);
void func_0888E9E0(void);
void func_0888EA00(void);
void func_0888EA30(Obj9CA5B0 *self);
void func_0888EA4C(Obj9CA5B0 *self);

/* operator new / delete on the heaps of *D_089D6AC0. */
void *func_08890900(unsigned int size);
void *func_0888EA68(unsigned int size);
void func_0888EA84(void *block);

extern int D_089CA600, D_089CA638;
VtableOnly *func_0888EAA8(VtableOnly *self);
VtableOnly *func_0888ED88(VtableOnly *self);

extern void *D_09B491E8;
void *func_088DAA40(void *self, int arg1, char arg2);
void func_0888EBF0(void);

extern Obj87A61C D_09B491F8;    /* a static object */
extern int D_089D6AB0;          /* its destructor entry */
void func_0888EC44(void);
void func_0888F000(void);
void func_0888F390(void);
void func_0888F66C(void);
void func_0888F8D4(void);
void func_0888FE08(void);
void func_08890780(void);

typedef struct Obj88F9EC {
    char pad_00[0xC];
    int field_0C;
    char pad_10[0x14];
    int field_24;
} Obj88F9EC;
MAIN3_AT(Obj88F9EC, field_0C, 0xC);
MAIN3_AT(Obj88F9EC, field_24, 0x24);

void func_0888F9EC(Obj88F9EC *self);
void func_0888FC34(Obj88F9EC *self);

/* The memory object D_089D6AC0 points at: a block, two heaps and a block
 * taken from the first heap. */
typedef struct Mem9D6AC0 {
    void *block;
    void *heap;
    void *heap2;
    void *heap_block;
} Mem9D6AC0;
MAIN3_SIZE(Mem9D6AC0, 0x10);
MAIN3_AT(Mem9D6AC0, block, 0x0);
MAIN3_AT(Mem9D6AC0, heap, 0x4);
MAIN3_AT(Mem9D6AC0, heap2, 0x8);
MAIN3_AT(Mem9D6AC0, heap_block, 0xC);

extern Mem9D6AC0 *D_089D6AC0;
Mem9D6AC0 *func_088907AC(Mem9D6AC0 *self);
void func_0889081C(Mem9D6AC0 *self);
void *func_088908F0(void);
void func_08890970(void *block);
void func_0889099C(void *block);
void func_08890A00(void *block);

/* Vtable D_089CA748 at +0x24. */
typedef struct Obj9CA748 {
    char pad_00[0x24];
    void *vtable;
} Obj9CA748;
MAIN3_SIZE(Obj9CA748, 0x28);
MAIN3_AT(Obj9CA748, vtable, 0x24);

extern int D_089CA748;
Obj9CA748 *func_08890ACC(Obj9CA748 *self);
void func_08890B04(Obj9CA748 *self, unsigned int flags);
void func_08890B5C(Obj9CA748 *self);
void func_08890B9C(Obj9CA748 *self);

/* One 0xC-byte entry of the table a model keeps at +0x108. */
typedef struct ModelEntry {
    int field_00;
    int field_04;
    unsigned int flags;
} ModelEntry;
MAIN3_SIZE(ModelEntry, 0xC);
MAIN3_AT(ModelEntry, flags, 0x8);

/* The model object (auto_region2.h's ModelObj) seen at +0x108. */
typedef struct ModelView {
    char pad_00[0x108];
    ModelEntry *entries;
} ModelView;
MAIN3_AT(ModelView, entries, 0x108);

/* A 0x130-byte object built by func_08894D34, destroyed by func_08894E28
 * (vtable D_089CA868 at +0x120) and drawn by func_08895388. The members at
 * +0x10, +0x60 and +0x78 are destroyed by func_0886EE90, func_0886F048 and
 * func_08864C48. The colour at +0x100 is placed here by address only
 * (func_088953F0 sits among this class's functions). */
typedef struct Obj894D34 {
    char pad_00[0x10];
    char field_10[0x44];        /* passed to func_0886ACCC with the model */
    int field_54;
    char pad_58[0x8];
    char field_60[0x14];
    ModelView *model;
    char field_78[0x88];
    unsigned int color;         /* 0x00BBGGRR */
    char pad_104[0x1C];
    void *vtable;
    char pad_124[0xC];
} Obj894D34;
MAIN3_SIZE(Obj894D34, 0x130);
MAIN3_AT(Obj894D34, field_10, 0x10);
MAIN3_AT(Obj894D34, field_54, 0x54);
MAIN3_AT(Obj894D34, field_60, 0x60);
MAIN3_AT(Obj894D34, model, 0x74);
MAIN3_AT(Obj894D34, field_78, 0x78);
MAIN3_AT(Obj894D34, color, 0x100);
MAIN3_AT(Obj894D34, vtable, 0x120);

/* Derives from Obj894D34 (base vtable D_089CA768); own vtable D_089CA758 at
 * +0x734; three parts of the same class at +0x140 and one more at +0x4E0. */
typedef struct Obj9CA758 {
    Obj894D34 base;
    int field_130;
    char pad_134[0xC];
    Obj894D34 parts[3];
    int part_index;             /* -1: none */
    char pad_4D4[0xC];
    Obj894D34 field_4E0;
    unsigned char field_610;
    char pad_611[0xA1];
    unsigned char field_6B2;
    char pad_6B3;
    unsigned char field_6B4;
    char pad_6B5[0x3];
    int field_6B8;
    int field_6BC;
    int field_6C0;
    char pad_6C4[0x4C];
    int field_710;
    int field_714;
    int field_718;
    int field_71C;
    int field_720;
    int field_724;
    int field_728;
    int field_72C;
    int field_730;
    void *vtable;
} Obj9CA758;
MAIN3_SIZE(Obj9CA758, 0x738);
MAIN3_AT(Obj9CA758, field_130, 0x130);
MAIN3_AT(Obj9CA758, parts, 0x140);
MAIN3_AT(Obj9CA758, part_index, 0x4D0);
MAIN3_AT(Obj9CA758, field_4E0, 0x4E0);
MAIN3_AT(Obj9CA758, field_610, 0x610);
MAIN3_AT(Obj9CA758, field_6B2, 0x6B2);
MAIN3_AT(Obj9CA758, field_6B4, 0x6B4);
MAIN3_AT(Obj9CA758, field_6B8, 0x6B8);
MAIN3_AT(Obj9CA758, field_6C0, 0x6C0);
MAIN3_AT(Obj9CA758, field_710, 0x710);
MAIN3_AT(Obj9CA758, field_730, 0x730);
MAIN3_AT(Obj9CA758, vtable, 0x734);

extern int D_089CA758, D_089CA768;
Obj894D34 *func_08894D34(Obj894D34 *self);
void func_08894E28(Obj894D34 *self, unsigned int flags);
void func_08895388(Obj894D34 *self);
void func_08893CEC(Obj9CA758 *self);
void func_0889245C(Obj9CA758 *self, int arg);
Obj9CA758 *func_08892198(Obj9CA758 *self);
void func_08892234(Obj9CA758 *self, unsigned int flags);
void func_08892584(Obj9CA758 *self);
void func_0886ACCC(void *model, void *matrix, int arg);
void func_08866BA8(void *self);
void func_089A0C20(unsigned int color);
void func_089A0E20(unsigned int arg1, int arg2);
void func_089A0E60(unsigned int arg1, int arg2);
void func_089A25C0(void);


/* ------------------------------------------------------------ 0x088934E0..0x0889716C */

extern Obj9CA758 *D_089D6AC8;
void func_08892A78(Obj9CA758 *self, int arg1, int arg2, int arg3);
void func_08892AC0(Obj9CA758 *self, int arg1, int arg2);
void func_08892C50(Obj9CA758 *self, int arg);
void func_08892760(Obj9CA758 *self);
void func_088934E0(int arg1, int arg2, int arg3);
void func_0889350C(int arg1, int arg2);
void func_08893534(int arg);
void func_08893710(void);
void func_08893730(void);
int func_0889381C(Obj9CA758 *self);
int func_08893828(Obj9CA758 *self);
int func_08893834(Obj9CA758 *self);
int func_08893840(Obj9CA758 *self);
void func_08893C94(Obj9CA758 *self, int arg);
void func_08893CC0(Obj9CA758 *self, int arg);
unsigned char func_08893CF8(Obj9CA758 *self);

void func_08893750(Holder *self);
void func_08893910(Holder *self);

/* Calls on the model of an Obj894D34. */
void func_0886BB44(ModelView *model, unsigned int arg1, unsigned char arg2, unsigned char arg3, int arg4);
void func_0886BD70(ModelView *model, int arg1, int arg2, void *arg3, char arg4);
void func_0886BF28(ModelView *model, int arg1, char arg2);
void func_0886C050(ModelView *model, int arg1, int arg2, char arg3);
void func_0886C218(ModelView *model, int arg1, char arg2);
void func_08893938(Obj894D34 *self, unsigned int arg1, unsigned char arg2);
void func_08893968(Obj894D34 *self, int arg1, int arg2);
void func_0889398C(Obj894D34 *self, int arg);
void func_088939AC(Obj894D34 *self, int arg1, int arg2);
void func_088939CC(Obj894D34 *self, int arg);
unsigned char func_088939EC(Obj894D34 *self, int index);
int func_08893A18(Obj894D34 *self);
extern int D_089CA868;
void func_0886EE90(void *self, unsigned int flags);
void func_08864C48(void *self, unsigned int flags);
void func_08894E9C(Holder *self, void *value);
void func_08866AA8(unsigned char red, unsigned char green, unsigned char blue);
void func_088953F0(Obj894D34 *self);
ModelView *func_08895420(Obj894D34 *self);
void *func_08895428(Obj894D34 *self);

/* Vtable D_089CA890 at +0x8 (D_089CA8B0 for the derived class func_088956FC destroys). */
typedef struct Obj9CA890 {
    char active;
    char pad_01[0x3];
    int field_04;
    void *vtable;
} Obj9CA890;
MAIN3_SIZE(Obj9CA890, 0xC);
MAIN3_AT(Obj9CA890, active, 0x0);
MAIN3_AT(Obj9CA890, vtable, 0x8);

extern int D_089CA890, D_089CA8B0;
Obj9CA890 *func_08895430(Obj9CA890 *self);
void func_08895448(Obj9CA890 *self, unsigned int flags);
void func_088956FC(Obj9CA890 *self, unsigned int flags);

/* The object D_089D6D80 points at; the handle at +0x8 is passed to the
 * 0x089406xx..0x089419xx calls. */
typedef struct Sys9D6D80 {
    int field_00;
    int field_04;
    int handle;
} Sys9D6D80;
MAIN3_AT(Sys9D6D80, handle, 0x8);

extern Sys9D6D80 *D_089D6D80;
int func_08941250(int arg);
int func_0894125C(int arg);
void func_0889547C(void);
void func_088954A0(void);

/* The pool D_089D6AD0 points at; blocks are freed to its heap at +0x8. */
typedef struct Pool9D6AD0 {
    int field_00;
    int field_04;
    void *heap;
} Pool9D6AD0;
MAIN3_SIZE(Pool9D6AD0, 0xC);
MAIN3_AT(Pool9D6AD0, field_00, 0x0);
MAIN3_AT(Pool9D6AD0, field_04, 0x4);
MAIN3_AT(Pool9D6AD0, heap, 0x8);

extern Pool9D6AD0 *D_089D6AD0;
void func_088954FC(void *block);
Pool9D6AD0 *func_08895758(Pool9D6AD0 *self);
void func_08895768(Pool9D6AD0 *self);
void func_0889586C(Pool9D6AD0 *self);
void func_088958EC(Pool9D6AD0 *self, void *heap);
void func_088959E8(void);
void func_08895A08(void);
Words3 *func_08895A40(Words3 *self);

/* An object holding a handle at +0x8 (D_089D6AE0 points at one). */
typedef struct Obj895BE4 {
    int field_00;
    int field_04;
    void *handle;
} Obj895BE4;
MAIN3_AT(Obj895BE4, handle, 0x8);

extern Obj895BE4 *D_089D6AE0;
void func_08895DC0(void);
void func_08895E00(void);
void func_088A9E60(void *self, void *buffer, int count, void (*callback)(void), int arg4, int arg5,
                   int arg6, int arg7);
void func_088AA130(void *self, void (*callback)(void), int arg);
void func_08895BDC(Obj895BE4 *self);
void func_08895BE4(Obj895BE4 *self);
void func_08895C28(Obj895BE4 *self);
void func_08895CF8(Obj895BE4 *self, int arg);
void func_08895DA0(void);

/* Five words, a member at +0x14 (built by func_0889F63C, released by
 * func_0889F704) and a state at +0x38 (D_089D6AE8 points at one). */
typedef struct Obj8966BC {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    char field_14[0x24];
    int state;
    int field_3C;
    int field_40;
} Obj8966BC;
MAIN3_SIZE(Obj8966BC, 0x44);
MAIN3_AT(Obj8966BC, field_14, 0x14);
MAIN3_AT(Obj8966BC, state, 0x38);
MAIN3_AT(Obj8966BC, field_3C, 0x3C);
MAIN3_AT(Obj8966BC, field_40, 0x40);

void *func_0889F63C(void *self);
int func_0889F704(void *self);
void func_0889F898(void *self, unsigned char arg);
void func_0889F910(void *self);
int func_088A24D4(int arg1, int arg2, int arg3, int arg4);
void func_088963B8(Obj8966BC *self);
void func_088963E4(Obj8966BC *self, int arg1, int arg2, int arg3, int arg4);
void func_088965F4(Obj8966BC *self, unsigned char arg);
Obj8966BC *func_088966BC(Obj8966BC *self);
void func_088967B4(Obj8966BC *self);

extern void *D_089D6C10;
Holder *func_08897118(Holder *self);
int func_08897138(void *self, int arg);


/* ------------------------------------------------------------ 0x08897244..0x088A3224 */

/* A 0x108-byte object built by func_08897244 and destroyed by func_0889726C:
 * a block copied in by func_08897518 from the heap at +0xF0. */
typedef struct Obj897244 {
    void *data;
    char pad_04[0xA8];
    int field_AC;
    int field_B0;
    int field_B4;
    int field_B8;
    int field_BC;
    char field_C0;
    char pad_C1[0x3];
    int field_C4;
    char pad_C8[0x28];
    void *heap;
    int field_F4;
    int field_F8;
    int field_FC;
    int field_100;
    char pad_104[0x4];
} Obj897244;
MAIN3_SIZE(Obj897244, 0x108);
MAIN3_AT(Obj897244, data, 0x0);
MAIN3_AT(Obj897244, field_AC, 0xAC);
MAIN3_AT(Obj897244, field_C0, 0xC0);
MAIN3_AT(Obj897244, field_C4, 0xC4);
MAIN3_AT(Obj897244, heap, 0xF0);
MAIN3_AT(Obj897244, field_F4, 0xF4);
MAIN3_AT(Obj897244, field_100, 0x100);

/* Two Obj897244 and a word: the static object D_09AEF8D8 that
 * func_088992B0 builds on first use. */
typedef struct Obj8991D0 {
    Obj897244 base;
    Obj897244 field_108[1];
    int field_210;
} Obj8991D0;
MAIN3_SIZE(Obj8991D0, 0x214);
MAIN3_AT(Obj8991D0, field_108, 0x108);
MAIN3_AT(Obj8991D0, field_210, 0x210);

Obj897244 *func_08897244(Obj897244 *self);
void func_0889726C(Obj897244 *self, unsigned int flags);
int func_08935970(const char *a, const char *b);
int func_088973EC(const char **a, const char **b);
void *func_0885C6CC(void *heap, int size, int align, int arg3, int arg4, char arg5);
void *func_08897518(Obj897244 *self, const void *src, int size);
char func_0889CF04(int arg1, int *arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8,
                   int arg9);
void func_08897960(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6);
void func_08898F84(Obj897244 *self);
void func_08898F5C(Obj897244 *self);
void func_08899178(Obj897244 *self, int arg1, int arg2);
void func_08899240(Obj8991D0 *self, unsigned int flags);
extern int D_09AEF8D4;            /* set once D_09AEF8D8 is built */
extern Obj8991D0 D_09AEF8D8;
extern int D_089D6C20;            /* its destructor entry */
Obj8991D0 *func_088991D0(Obj8991D0 *self);
Obj8991D0 *func_088992B0(void);
void func_0889991C(int value);
unsigned char func_08899944(void);

/* Three words, an Obj87A61C at +0xC and a 0x24-byte member at +0x1C (built by
 * func_0889F63C, released by func_0889F704). */
typedef struct Obj89F4F4 {
    int field_00;
    int field_04;
    int field_08;
    Obj87A61C field_0C;
    char field_1C[0x24];
} Obj89F4F4;
MAIN3_SIZE(Obj89F4F4, 0x40);
MAIN3_AT(Obj89F4F4, field_0C, 0xC);
MAIN3_AT(Obj89F4F4, field_1C, 0x1C);

int func_0889F3E4(void);
int func_0889F534(Obj89F4F4 *self, int arg);
void func_0889FF8C(int arg);
int func_0889F6A0(void *self, int arg1, int arg2, int arg3);
void func_0889F830();           /* called with one argument and with two */
int func_0889F980(void *self);
int func_0889FABC(void);
void func_0889F234(Obj89F4F4 *self);
void func_0889F264(Obj89F4F4 *self, int arg);
void func_0889F290(Obj89F4F4 *self, int arg1, int arg2);
void func_0889F2B8(Obj89F4F4 *self);
void func_0889F340(Obj89F4F4 *self);
unsigned char func_0889F35C(Obj89F4F4 *self);
Obj89F4F4 *func_0889F4F4(Obj89F4F4 *self);
void func_0889FA64(void);
void func_0889FA80(void *self);

extern void *D_089D6C38;
extern int D_089D6C58;
void func_0889FCE8(void);
int func_089906F8(void);
void func_088A040C(void);
void func_08990680(void);
void func_088A074C(void);

/* The static object D_09B4CF80. */
typedef struct Obj8672F4 Obj8672F4;
extern Obj8672F4 D_09B4CF80;
extern void *D_089D6C78;
Obj8672F4 *func_088672F4(Obj8672F4 *self);
int func_08867338(Obj8672F4 *self);
void func_088674AC(Obj8672F4 *self, int arg);
void func_08867590(Obj8672F4 *self);
int func_088676B4(Obj8672F4 *self, int arg);
void func_08867718(Obj8672F4 *self, int arg);
void func_088A0C6C(void);
void func_088A0CD4(int arg);
void func_088A0CF8(int arg);
void func_088A0D40(void);
void func_088A0D60(void);
void func_088A0D80(void);
void func_088A0D88(void);

/* 32 elements of 0x30 bytes, each built by func_088A0E94. */
typedef struct Obj8A0EB4 {
    char elems[0x20][0x30];
} Obj8A0EB4;
MAIN3_SIZE(Obj8A0EB4, 0x600);
void func_088A0E94(void);
Obj8A0EB4 *func_088A0EB4(Obj8A0EB4 *self);

/* The manager D_089D6C88 points at: a heap, a block, a member built by
 * func_088A077C, 20 one-byte elements and three 4-float vectors. */
typedef struct Mgr9D6C88 {
    void *heap;
    void *block;
    char field_08[0xC];
    char field_14[0x54];
    char field_68[0x14];
    float vectors[3][4];
} Mgr9D6C88;
MAIN3_SIZE(Mgr9D6C88, 0xAC);
MAIN3_AT(Mgr9D6C88, heap, 0x0);
MAIN3_AT(Mgr9D6C88, block, 0x4);
MAIN3_AT(Mgr9D6C88, field_08, 0x8);
MAIN3_AT(Mgr9D6C88, field_14, 0x14);
MAIN3_AT(Mgr9D6C88, field_68, 0x68);
MAIN3_AT(Mgr9D6C88, vectors, 0x7C);

extern Mgr9D6C88 *D_089D6C88;
void *func_088A077C(void *self);
void func_088A1908(void);
void func_088A1930(void);
void func_08990024(void);
void func_08990078(void);
void func_088A1330(Mgr9D6C88 *self);
void func_088A1ADC(Mgr9D6C88 *self);
unsigned int func_089908D8(int arg1, unsigned char arg2);
void func_08990E48(int arg1, int arg2);
void func_08990D18(int arg);
void func_08990D34(void);
char func_08990DE0(int arg);
void func_088A1BEC(float x, float y, float *out);
unsigned char func_088A1758(Mgr9D6C88 *self, int id);
void func_088A17A4(Mgr9D6C88 *self, unsigned int id, unsigned char arg);
void func_088A17C4(Mgr9D6C88 *self, int id, int arg);
void func_088A183C(Mgr9D6C88 *self, int id, int arg);
void func_088A185C(Mgr9D6C88 *self, int arg);
void func_088A1878(void);
void func_088A1894(Mgr9D6C88 *self, int id);
void func_088A18DC(Mgr9D6C88 *self, int x, int y);
Mgr9D6C88 *func_088A193C(Mgr9D6C88 *self);
void func_088A19B0(Mgr9D6C88 *self);
void func_088A1AE4(void);
void func_088A1B3C(void *block);

/* Flags at +0x34 and a callback with its argument at +0x44 / +0x48. */
typedef void (*Callback8A3024)(void *self, int arg);
typedef struct Obj8A1F6C {
    char pad_00[0x34];
    unsigned int flags;
    char pad_38[0xC];
    Callback8A3024 callback;
    int arg;
} Obj8A1F6C;
MAIN3_AT(Obj8A1F6C, flags, 0x34);
MAIN3_AT(Obj8A1F6C, callback, 0x44);
MAIN3_AT(Obj8A1F6C, arg, 0x48);

void func_088A1F6C(Obj8A1F6C *self, unsigned char on);
void func_088A1F94(Obj8A1F6C *self, unsigned char on);
void func_088A3024(Obj8A1F6C *self);

/* A reference to an entry of the manager: id at +0x8, argument at +0xC. */
typedef struct Ref8A3188 {
    int field_00;
    int field_04;
    int id;
    int arg;
} Ref8A3188;
MAIN3_AT(Ref8A3188, id, 0x8);
MAIN3_AT(Ref8A3188, arg, 0xC);

void func_088A3188(Ref8A3188 *self);
void func_088A31AC(Ref8A3188 *self, unsigned char arg);
void func_088A31D4(Ref8A3188 *self);
void func_088A31FC(Ref8A3188 *self, int arg);
void func_088A3224(Ref8A3188 *self);

/* ------------------------------------------------------------ 0x088A32D8..0x088A7350 */

/* A 0x34-byte object: a word, a 0x24-byte member at +0x4 (the class
 * func_0889F63C builds), a callback and its argument, two bytes. Two of them
 * make up the static array D_09B4D3B0. */
typedef void (*Callback8A3E4C)(int arg);
typedef struct Obj8A3E08 {
    int field_00;
    char field_04[0x24];
    Callback8A3E4C callback;
    int arg;
    unsigned char field_30;
    unsigned char field_31;
    char pad_32[0x2];
} Obj8A3E08;
MAIN3_SIZE(Obj8A3E08, 0x34);
MAIN3_AT(Obj8A3E08, field_04, 0x4);
MAIN3_AT(Obj8A3E08, callback, 0x28);
MAIN3_AT(Obj8A3E08, arg, 0x2C);
MAIN3_AT(Obj8A3E08, field_30, 0x30);
MAIN3_AT(Obj8A3E08, field_31, 0x31);

extern Obj8A3E08 D_09B4D3B0[2];
extern int D_089D6D3C;          /* its destructor entry */
Obj8A3E08 *func_088A3E08(Obj8A3E08 *self);
void func_088A3E4C(Obj8A3E08 *self);
void func_088A40C4(Obj8A3E08 *self);
void func_088A40E0(Obj8A3E08 *self, unsigned int flags);
void func_088A4130(void);
void func_088A4168(void);

typedef struct Ref8A32D8 {
    int field_00;
    int field_04;
    Obj8A3E08 *target;
} Ref8A32D8;
MAIN3_AT(Ref8A32D8, target, 0x8);

void func_088A32D8(Ref8A32D8 *self);
Words3 *func_088A3900(Words3 *self);
void func_088A402C(int arg);
void func_088A3D14(void *self, int arg);

void func_088A48EC(void);
void func_088A44F8(void *self, int arg);
void func_088A4540(void *self, int arg);

/* An entry of the list func_088A6478 walks: key at +0x10, a callback with
 * its argument at +0x38 / +0x3C, next at +0x40. */
typedef void (*Callback8A4C10)(int arg1, int arg2, int arg3);
typedef struct Obj8A4C10 {
    char pad_00[0x10];
    int key;
    char pad_14[0x24];
    Callback8A4C10 callback;
    int arg;
    struct Obj8A4C10 *next;
} Obj8A4C10;
MAIN3_AT(Obj8A4C10, key, 0x10);
MAIN3_AT(Obj8A4C10, callback, 0x38);
MAIN3_AT(Obj8A4C10, arg, 0x3C);
MAIN3_AT(Obj8A4C10, next, 0x40);

void func_088A4C10(Obj8A4C10 *self, int arg1, int arg2);
void func_089419C8(int handle, void *object);

extern Obj8672F4 D_09B4D420;    /* a static object */
void func_088A51E4(int arg);
void func_088A5208(int arg);
void func_088A522C(int arg);
void func_088A5250(void);
void func_088A52CC(void);
extern void *D_089D6D68;        /* a heap */
void func_088A52A4(void *block);

void func_088A56A8(void);
void func_088A57B0(void *self);
void func_088A534C(void *self);

/* An id (-1: none) registered with a handle, and a value. */
typedef struct Obj8A6510 {
    int id;
    int handle;
    int value;
} Obj8A6510;
MAIN3_SIZE(Obj8A6510, 0xC);
MAIN3_AT(Obj8A6510, id, 0x0);
MAIN3_AT(Obj8A6510, handle, 0x4);
MAIN3_AT(Obj8A6510, value, 0x8);

/* A 0x38-byte object built by func_088A9974 on the base Obj8A6510: a
 * handler at +0xC, the first of its Obj8A6A78 children at +0x1C (linked
 * through their +0x27C) and the next object of its list at +0x34. */
typedef struct Obj8A9974 {
    Obj8A6510 base;
    int field_0C;
    int field_10;
    int field_14;
    int field_18;
    struct Obj8A6A78 *children;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    unsigned char field_30;
    char pad_31[0x3];
    struct Obj8A9974 *next;
} Obj8A9974;
MAIN3_SIZE(Obj8A9974, 0x38);
MAIN3_AT(Obj8A9974, field_0C, 0xC);
MAIN3_AT(Obj8A9974, field_10, 0x10);
MAIN3_AT(Obj8A9974, children, 0x1C);
MAIN3_AT(Obj8A9974, field_20, 0x20);
MAIN3_AT(Obj8A9974, field_24, 0x24);
MAIN3_AT(Obj8A9974, field_30, 0x30);
MAIN3_AT(Obj8A9974, next, 0x34);

/* A 0x68-byte element built by func_088A5884 on the base Obj8A9974. */
typedef struct Obj8A5884 {
    Obj8A9974 base;
    char field_38;
    char pad_39[0x1F];
    int field_58;
    Holder field_5C;
    int field_60;
    char field_64;
    char pad_65[0x3];
} Obj8A5884;
MAIN3_SIZE(Obj8A5884, 0x68);
MAIN3_AT(Obj8A5884, field_38, 0x38);
MAIN3_AT(Obj8A5884, field_58, 0x58);
MAIN3_AT(Obj8A5884, field_5C, 0x5C);
MAIN3_AT(Obj8A5884, field_60, 0x60);
MAIN3_AT(Obj8A5884, field_64, 0x64);

/* 45 of them and two words. */
typedef struct Obj8A58C4 {
    Obj8A5884 elems[45];
    int field_1248;
    int field_124C;
} Obj8A58C4;
MAIN3_SIZE(Obj8A58C4, 0x1250);
MAIN3_AT(Obj8A58C4, field_1248, 0x1248);
MAIN3_AT(Obj8A58C4, field_124C, 0x124C);

Obj8A9974 *func_088A9974(Obj8A9974 *self);
Obj8A5884 *func_088A5884(Obj8A5884 *self);
Obj8A58C4 *func_088A58C4(Obj8A58C4 *self);
void func_088A5904(Obj8A58C4 *self);

/* Calls on *D_089D6D80. */
void func_088A6028(Sys9D6D80 *self);
void func_088A6084(Sys9D6D80 *self);
void func_088A60B8(Sys9D6D80 *self);
void func_088A6120(Sys9D6D80 *self);
void func_088A6180(Sys9D6D80 *self);
void func_088A61E0(Sys9D6D80 *self);
void func_088A62A8(Sys9D6D80 *self);
void func_088A6248(Sys9D6D80 *self);
void func_088A6278(Sys9D6D80 *self);
void func_088A5E6C(Sys9D6D80 *self);
void func_088A5C34(void);
void func_088A5C54(void);
void func_088A5C74(void);
void func_088A5C94(void);
void func_088A5CB4(void);
void func_088A5CD4(void);
void func_088A5CF8(void);
void func_088A5D18(void);
void func_088A5D38(void);
void func_088A5D58(void);
unsigned char func_0885C3BC(void (*callback)(void), int arg);
extern void *D_089D6D78;
void func_088A5EBC(void);
void func_088A5830(void *self);
void func_088A4D04(void);
void func_08940610(int handle, int arg);
void func_0894065C(int handle);
void func_088A63CC(void);
void func_089408DC(int handle);
void func_08940914(int handle);
void func_08940C70(int handle);
void func_08940C8C(int handle);
void func_08940CA8(int handle);
void func_08940CC4(int handle);

/* A list whose first entry is at +0. */
typedef struct List8A6478 {
    Obj8A4C10 *first;
} List8A6478;
MAIN3_SIZE(List8A6478, 0x4);

void func_088A6478(List8A6478 *self, int key, int arg1, int arg2);

/* Vtable D_089CAF68; keeps the list it forwards to. */
typedef struct Obj9CAF68 {
    void *vtable;
    List8A6478 *list;
} Obj9CAF68;
MAIN3_SIZE(Obj9CAF68, 0x8);
MAIN3_AT(Obj9CAF68, list, 0x4);

extern int D_089CAF68;
Obj9CAF68 *func_088A64DC(Obj9CAF68 *self, List8A6478 *list);
void func_088A64F4(Obj9CAF68 *self, int key, int arg1, int arg2);

void func_089418F8(int handle, int id);
Obj8A6510 *func_088A6510(Obj8A6510 *self);
void func_088A6528(Obj8A6510 *self, unsigned int flags);
int func_088A65E4(Obj8A6510 *self);

/* Three 3-float vectors, members at +0x30 and +0xB0, an object at +0xCC, a
 * state at +0xD4 (3 while paused, the previous one kept at +0xD8) and flags
 * at +0xDC (one bit per member that has been reset). */
typedef struct Obj8A6A78 {
    float field_00[3];
    float field_0C[3];
    float field_18[3];
    char pad_24[0xC];
    char field_30[0x80];
    char field_B0[0x1C];
    void *field_CC;
    int field_D0;
    int state;
    int saved_state;
    unsigned int flags;
    char pad_E0[0x19C];
    struct Obj8A6A78 *next;
} Obj8A6A78;
MAIN3_AT(Obj8A6A78, field_0C, 0xC);
MAIN3_AT(Obj8A6A78, field_18, 0x18);
MAIN3_AT(Obj8A6A78, field_30, 0x30);
MAIN3_AT(Obj8A6A78, field_B0, 0xB0);
MAIN3_AT(Obj8A6A78, field_CC, 0xCC);
MAIN3_AT(Obj8A6A78, state, 0xD4);
MAIN3_AT(Obj8A6A78, saved_state, 0xD8);
MAIN3_AT(Obj8A6A78, flags, 0xDC);
MAIN3_AT(Obj8A6A78, field_D0, 0xD0);
MAIN3_AT(Obj8A6A78, next, 0x27C);

void func_088A7890(void);
int func_088A751C(Obj8A6A78 *self);
void func_088A49EC(void *object);
void func_088A4A30(void *object);
void *func_089A34AC(float *v);
void *func_089A3EFC(void *m);
void *func_089A383C(void *v);
void func_088A6A78(Obj8A6A78 *self);
void func_088A6C70(Obj8A6A78 *self);
void func_088A71D8(Obj8A6A78 *self);
void func_088A7234(Obj8A6A78 *self);
void func_088A7284(Obj8A6A78 *self);
void func_088A72B4(Obj8A6A78 *self);
void func_088A72E8(Obj8A6A78 *self);
void func_088A731C(Obj8A6A78 *self);
void func_088A7350(Obj8A6A78 *self);

/* ------------------------------------------------------------ 0x088A807C..0x088AD0E8 */

/* Three 4x4 matrices at +0x10, +0x50 and +0x90. */
typedef struct Mat8A807C {
    char pad_00[0x10];
    float field_10[16];
    float field_50[16];
    float field_90[16];
} Mat8A807C;
MAIN3_SIZE(Mat8A807C, 0xD0);
MAIN3_AT(Mat8A807C, field_10, 0x10);
MAIN3_AT(Mat8A807C, field_50, 0x50);
MAIN3_AT(Mat8A807C, field_90, 0x90);

void func_088A807C(Mat8A807C *self, float *out);
void func_088A80A4(Mat8A807C *self, float *out);
void func_088A80CC(Mat8A807C *self, float *out);

/* Vtable D_089CB1B0 at +0x2C. */
typedef struct Obj9CB1B0 {
    char pad_00[0x2C];
    void *vtable;
} Obj9CB1B0;
MAIN3_SIZE(Obj9CB1B0, 0x30);
MAIN3_AT(Obj9CB1B0, vtable, 0x2C);

extern int D_089CB1B0;
void func_088A9880(Obj9CB1B0 *self, unsigned int flags);
void func_088A814C(Obj9CB1B0 *self, unsigned int flags);

/* A list of Obj8A9974 (D_089D6DC8 points at one). */
typedef struct List8A972C {
    Obj8A9974 *first;
    int field_04;
} List8A972C;
MAIN3_SIZE(List8A972C, 0x8);
MAIN3_AT(List8A972C, first, 0x0);
MAIN3_AT(List8A972C, field_04, 0x4);

extern List8A972C *D_089D6DC8;
void func_088A7FB8(void);
void func_088A9B18(Obj8A9974 *self);
void func_088A9D28(Obj8A9974 *self);
void func_088AA248(Obj8A9974 *self);
void func_088AA29C(Obj8A9974 *self);
void func_088A952C(List8A972C *self);
void func_088A9630(List8A972C *self);
void func_088A9684(List8A972C *self);
void func_088A96D8(List8A972C *self);
List8A972C *func_088A972C(List8A972C *self);
void func_088A973C(List8A972C *self);
void func_088A983C(void);

/* The object func_088A98B4 / func_088A990C / func_088A9944 are called on:
 * passed on as the source of an Obj8A9974 call. */
typedef struct Src8A98B4 {
    char pad_00[0x24];
    unsigned char field_24;
} Src8A98B4;
MAIN3_AT(Src8A98B4, field_24, 0x24);

void func_088A9E90(Obj8A9974 *self, Src8A98B4 *source, unsigned int arg1, unsigned int arg2,
                   unsigned int arg3);
void func_088AA3B0(Obj8A9974 *self, Src8A98B4 *source, int arg1, int arg2);
void func_088A7118(Obj8A6A78 *self, Src8A98B4 *source, int arg1, int arg2);
void func_088A98B4(Src8A98B4 *self, Obj8A9974 *target, unsigned int arg1, unsigned int arg2,
                   unsigned int arg3);
void func_088A990C(Src8A98B4 *self, Obj8A9974 *target, int arg);
void func_088A9944(Src8A98B4 *self, Obj8A9974 *target);

void func_088AA4B8(Obj8A9974 *self);
void func_088AA4C0(Obj8A9974 *self);
void func_088AA4D0(Obj8A9974 *self);
void func_088A9A4C(Obj8A9974 *self);
unsigned int func_088AA30C(Obj8A9974 *self, int key);
int func_088AA434(Obj8A9974 *self, int value);

/* Calls that return one result through a local. */
int func_088A9E24(void *self, void (*callback)(void), int arg1, int arg2, int arg3, int arg4);
int func_088AA9EC(void *self, int *out, int count, int arg1, int arg2, int arg3, int arg4);
int func_088AABE0(void *self, int *out, int count, int arg1, int arg2, int arg3, int arg4);
int func_088AADAC(void *self, int *out, int count, int arg1, int arg2, int arg3, int arg4);
void func_088AA050(void *self, int *out, int count, int arg1, int arg2, int arg3, int arg4);
void func_088AA0A4(void *self, int *out, int count, int arg1, int arg2, int arg3, int arg4);
void func_088AA0F8(void *self, int *out, int count, int arg1, int arg2, int arg3, int arg4);
int func_088AA018(void *self, int arg1, int arg2, int arg3, int arg4);
int func_088AA06C(void *self, int arg1, int arg2, int arg3, int arg4);
int func_088AA0C0(void *self, int arg1, int arg2, int arg3, int arg4);
void func_088AAF08(void);
void func_088AB028(void);
void func_088AB140(void);
void func_088AA114(void);
void func_088AA22C(void);
void func_088AA2F0(void);

/* 16 bytes cleared with memset and two words. */
typedef struct Obj8AB6BC {
    char field_00[0x10];
    int field_10;
    int field_14;
} Obj8AB6BC;
MAIN3_SIZE(Obj8AB6BC, 0x18);
MAIN3_AT(Obj8AB6BC, field_10, 0x10);
MAIN3_AT(Obj8AB6BC, field_14, 0x14);

Obj8AB6BC *func_088AB6BC(Obj8AB6BC *self);

/* A large object (0x3F600 bytes) with a state at +0x210, a 13-character
 * name at +0x21C, a data pointer and two sizes at +0x254..+0x25C, flags at
 * +0xEF8 and +0x3F5FC, and a buffer at +0xF08 whose header func_088AB7C0
 * checks (type 10, size 0x3ECC8). */
typedef struct Save8AB7A4 {
    char pad_000[0x210];
    int state;
    char pad_214[0x8];
    char name[0x38];
    void *data;
    int size_258;
    int size_25C;
    char pad_260[0x501];
    char field_761;
    char pad_762[0x2];
    int field_764[4];
    char pad_774[0x34];
    int field_7A8;
    char pad_7AC[0x710];
    int field_EBC;
    char field_EC0;
    char pad_EC1[0x7];
    int field_EC8;
    int field_ECC;
    char pad_ED0[0x10];
    Obj87A61C *field_EE0;
    char pad_EE4[0x10];
    int field_EF4;
    unsigned int flags;
    char pad_EFC;
    unsigned char field_EFD;
    char pad_EFE[0xA];
    int field_F08;
    int field_F0C;
    char pad_F10[0x1D078];
    char field_1DF88[0x280];    /* the extents of these three members are only */
    char field_1E208[0x21340];  /* the distance to the next known offset */
    char field_3F548[0xB4];
    unsigned int flags_3F5FC;
} Save8AB7A4;
MAIN3_AT(Save8AB7A4, state, 0x210);
MAIN3_AT(Save8AB7A4, name, 0x21C);
MAIN3_AT(Save8AB7A4, data, 0x254);
MAIN3_AT(Save8AB7A4, size_258, 0x258);
MAIN3_AT(Save8AB7A4, size_25C, 0x25C);
MAIN3_AT(Save8AB7A4, field_761, 0x761);
MAIN3_AT(Save8AB7A4, field_764, 0x764);
MAIN3_AT(Save8AB7A4, field_7A8, 0x7A8);
MAIN3_AT(Save8AB7A4, field_EBC, 0xEBC);
MAIN3_AT(Save8AB7A4, field_EC0, 0xEC0);
MAIN3_AT(Save8AB7A4, field_EC8, 0xEC8);
MAIN3_AT(Save8AB7A4, field_ECC, 0xECC);
MAIN3_AT(Save8AB7A4, field_EE0, 0xEE0);
MAIN3_AT(Save8AB7A4, field_EF4, 0xEF4);
MAIN3_AT(Save8AB7A4, flags, 0xEF8);
MAIN3_AT(Save8AB7A4, field_EFD, 0xEFD);
MAIN3_AT(Save8AB7A4, field_F08, 0xF08);
MAIN3_AT(Save8AB7A4, field_F0C, 0xF0C);
MAIN3_AT(Save8AB7A4, field_1DF88, 0x1DF88);
MAIN3_AT(Save8AB7A4, field_1E208, 0x1E208);
MAIN3_AT(Save8AB7A4, field_3F548, 0x3F548);
MAIN3_AT(Save8AB7A4, flags_3F5FC, 0x3F5FC);

char *func_08935BB4(char *dst, const char *src, unsigned int size);   /* strncpy */
extern const char *D_089D6010;
const char *func_08867C70(void);
const char *func_08867C80(void);
const char *func_08867C90(void);
void func_088F1B8C(void);
void func_088AB9F8(Save8AB7A4 *self);
void func_088AC034(Save8AB7A4 *self, int state);
unsigned int func_088AC07C(Save8AB7A4 *self, unsigned char arg);
void func_088AC82C(Save8AB7A4 *self, char arg);
void func_088ADF68(Save8AB7A4 *self, int *out, int arg1, int arg2);
int func_088AB7A4(Save8AB7A4 *self, int arg1, int arg2, int arg3);
unsigned char func_088AB7C0(Save8AB7A4 *self);
void func_088AC1AC(Save8AB7A4 *self, int state, unsigned char on);
void func_088AC294(Save8AB7A4 *self, int state);
void func_088AC2F8(Save8AB7A4 *self, void *data, int size, const char *name, int kind);
void func_088AC37C(Save8AB7A4 *self, void *data, int size);
void func_088AC3D0(Save8AB7A4 *self, void *data, int size);
void func_088AC424(Save8AB7A4 *self, void *data, int size);
void func_088AC478(Save8AB7A4 *self);
void func_088AC760(Save8AB7A4 *self);
void func_088AC7C4(Save8AB7A4 *self);
void func_088AC7EC(Save8AB7A4 *self);

extern Save8AB7A4 *D_089D712C;
void func_088ACAFC(void *self, int arg1, int arg2);
void func_088AE9B8(void *self, int arg);
void func_088ACDD0(int arg1, int arg2);
void func_088AD0E8(int arg);

/* ------------------------------------------------------------ 0x088AD1E8..0x088B7060 */

void func_088AD20C(Save8AB7A4 *self, int arg);
void func_088AD1E8(int arg);
void func_088AD994(Save8AB7A4 *self);
void func_088ADF28(Save8AB7A4 *self, char *out, int index, int arg3);
void func_088ADF7C(int arg1, int arg2, int arg3);
void func_088AE010(unsigned int packed, unsigned int *year, unsigned int *month, unsigned int *day,
                   unsigned int *hour, unsigned int *minute);
void func_088AE048(unsigned int packed, unsigned int *year, unsigned int *month, unsigned int *day,
                   unsigned int *hour, unsigned int *minute);

/* A large work area reached through Obj8AEFA0 (+0xC) and Mgr9D7260 (+0x14). */
typedef struct Work8AEFA0 {
    char pad_0000[0x814];
    int field_814;
    char pad_0818[0x18];
    char field_830[0x14];
    char pad_0844[0x20];
    char field_864[0x80];
    char field_8E4[0x80];
    char pad_0964[0x448];
    int field_DAC;
    char pad_0DB0[0x24];
    int field_DD4;
    char pad_0DD8[0x34];
    char field_E0C[0x40];
    char pad_0E4C[0x1C48];
    char field_2A94[0x14];
    char pad_2AA8[0x88];
    int field_2B30;
    int field_2B34;
    int field_2B38;
    int field_2B3C;
    char field_2B40[0x14];
    char pad_2B54[0x1C];
    int field_2B70;
    int field_2B74;
    int field_2B78;
    char pad_2B7C[0x3];
    char field_2B7F;
    char pad_2B80[0x4];
    int field_2B84;
    int field_2B88;
} Work8AEFA0;
MAIN3_AT(Work8AEFA0, field_814, 0x814);
MAIN3_AT(Work8AEFA0, field_830, 0x830);
MAIN3_AT(Work8AEFA0, field_864, 0x864);
MAIN3_AT(Work8AEFA0, field_8E4, 0x8E4);
MAIN3_AT(Work8AEFA0, field_DAC, 0xDAC);
MAIN3_AT(Work8AEFA0, field_DD4, 0xDD4);
MAIN3_AT(Work8AEFA0, field_E0C, 0xE0C);
MAIN3_AT(Work8AEFA0, field_2A94, 0x2A94);
MAIN3_AT(Work8AEFA0, field_2B30, 0x2B30);
MAIN3_AT(Work8AEFA0, field_2B3C, 0x2B3C);
MAIN3_AT(Work8AEFA0, field_2B40, 0x2B40);
MAIN3_AT(Work8AEFA0, field_2B70, 0x2B70);
MAIN3_AT(Work8AEFA0, field_2B78, 0x2B78);
MAIN3_AT(Work8AEFA0, field_2B7F, 0x2B7F);
MAIN3_AT(Work8AEFA0, field_2B84, 0x2B84);
MAIN3_AT(Work8AEFA0, field_2B88, 0x2B88);

typedef struct Obj8AEFA0 {
    int field_00;
    unsigned int flags;
    Words2 *field_08;
    Work8AEFA0 *work;
} Obj8AEFA0;
MAIN3_SIZE(Obj8AEFA0, 0x10);
MAIN3_AT(Obj8AEFA0, flags, 0x4);
MAIN3_AT(Obj8AEFA0, field_08, 0x8);
MAIN3_AT(Obj8AEFA0, work, 0xC);

void func_088AEF98(Obj8AEFA0 *self);
void func_088AEFA0(Obj8AEFA0 *self);
void func_088AF020(Obj8AEFA0 *self, int value);
unsigned char func_088AEF54(void);
void func_088AF0D0(void);
void func_088AF3E0(void);
void func_088AF348(Obj8AEFA0 *self);
void func_088AF75C(Obj8AEFA0 *self, int value);

/* The object D_089D7260 points at; its work area is at +0x14. */
typedef struct Mgr9D7260 {
    char pad_00[0x14];
    Work8AEFA0 *work;
} Mgr9D7260;
MAIN3_AT(Mgr9D7260, work, 0x14);

extern Mgr9D7260 *D_089D7260;
void func_088B01C0(Mgr9D7260 *self, int arg);
void func_088AF424(Mgr9D7260 *self, int arg1, int arg2);
void func_088AFB30(Mgr9D7260 *self);
void func_088AFA70(int arg);
void func_088AFB64(int arg1, int arg2);
int func_088AFACC(int value);
void func_088AFB00(Words2 *self);
int func_088AFBAC(void);
int func_088B0754(int value);
extern void *D_089D726C;
void func_088B0AD8(void);
void func_088ADE90(Save8AB7A4 *self, char *arg1, char *arg2, int arg3);
void func_088B0CC8(char *arg1, char *arg2);
void func_088B0C08(void);
void func_088B0CAC(void);
void func_089C5880(void);
void func_088B1440(void);
int func_088B15D0(int value);
extern void *D_089D727C;
void func_088B145C(void *self, int arg);
void func_088B1604(int arg);

/* Four bytes, a word at +0x4, +0x8, a state at +0x10 and +0x14. */
typedef struct Obj8B1998 {
    char field_00;
    char field_01;
    char field_02;
    char field_03;
    int field_04;
    int field_08;
    int field_0C;
    int state;
    int field_14;
} Obj8B1998;
MAIN3_SIZE(Obj8B1998, 0x18);
MAIN3_AT(Obj8B1998, field_04, 0x4);
MAIN3_AT(Obj8B1998, field_08, 0x8);
MAIN3_AT(Obj8B1998, state, 0x10);
MAIN3_AT(Obj8B1998, field_14, 0x14);

void func_088B1764(Obj8B1998 *self);
unsigned char func_088B178C(Obj8B1998 *self);
Obj8B1998 *func_088B1998(Obj8B1998 *self);
extern void *D_089D7280;
void func_088B1798(void *self, int arg);
void func_088B19E0(int arg);

extern Obj87A61C D_09B49108, D_09B49118, D_09B4D478;    /* static objects */
extern int D_089D7298, D_089D72C4, D_089D72D0;          /* their destructor entries */
void func_088B1DF4(void);
void func_088B2AD0(void);

typedef struct Words6 {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    int field_14;
} Words6;
MAIN3_SIZE(Words6, 0x18);
MAIN3_AT(Words6, field_14, 0x14);

void func_088B2B14(Words3 *self);
void func_088B2CFC(Words6 *self);

/* Copied member by member by func_088B2F80. */
typedef struct Obj8B2F80 {
    char field_00[0x24];
    Words6 field_24;
    Words6 field_3C;
    Words3 field_54;
    char field_60[0x4];
} Obj8B2F80;
MAIN3_AT(Obj8B2F80, field_24, 0x24);
MAIN3_AT(Obj8B2F80, field_3C, 0x3C);
MAIN3_AT(Obj8B2F80, field_54, 0x54);
MAIN3_AT(Obj8B2F80, field_60, 0x60);

void func_088B2C28(Obj8B2F80 *self, const Obj8B2F80 *other);
void func_088B2C98(Obj8B2F80 *self);
void func_088B2D18(Words6 *self, const Words6 *other);
void func_088B2DCC(Words3 *self, const Words3 *other);
void func_088B2E4C(void *self, const void *other);
void func_088B2F80(Obj8B2F80 *self, const Obj8B2F80 *other);
void func_088B2FE8(Obj8B2F80 *self);

void func_088B3224(void *self);
void *func_088B31FC(void *self);
void func_088B327C(void *self);

typedef struct Obj8B3298 {
    char pad_00[0x4C];
    char field_4C[0x4];
} Obj8B3298;
MAIN3_AT(Obj8B3298, field_4C, 0x4C);

void func_088B3060(void *self);
void func_088B3298(Obj8B3298 *self);

/* Seven words and the vtable D_089CB7C8 at +0x1C (D_089D72FC points at one). */
typedef struct Obj9CB7C8 {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    int field_14;
    Words2 *field_18;
    void *vtable;
} Obj9CB7C8;
MAIN3_SIZE(Obj9CB7C8, 0x20);
MAIN3_AT(Obj9CB7C8, field_14, 0x14);
MAIN3_AT(Obj9CB7C8, field_18, 0x18);
MAIN3_AT(Obj9CB7C8, vtable, 0x1C);

extern int D_089CB7C8;
extern Obj9CB7C8 *D_089D72FC;
Obj9CB7C8 *func_088B3830(Obj9CB7C8 *self);
void func_088B3868(Obj9CB7C8 *self, unsigned int flags);
void func_088B38C0(Obj9CB7C8 *self);
void func_088B38E0(Obj9CB7C8 *self);
int func_088B39E0(Obj9CB7C8 *self, int arg1, int arg2);
void func_088B3B20(Obj9CB7C8 *self, int arg);
int func_088B3B54(Obj9CB7C8 *self);
unsigned int func_088B3B60(Obj9CB7C8 *self, unsigned int arg);
int func_088B3B90(Obj9CB7C8 *self);
void func_088B3930(int arg1, int arg2);
void func_088B3958(int arg);
void func_088B397C(void);
void func_088B399C(int arg);
void func_088B39C0(void);

/* A pair of bytes reset to (-1, 0) by func_088B75C0. */
typedef struct BytePair {
    char first;
    char second;
} BytePair;
MAIN3_SIZE(BytePair, 0x2);

/* A 0xC6-byte record, reset by func_088B73FC; unset entries are -1. */
typedef struct Rec8B3DFC {
    char pad_00[0x5A];
    unsigned char field_5A[0x40];
    short field_9A[4];
    short field_A2[10];
    char field_B6;
    char field_B7;
    char field_B8;
    char field_B9;
    char field_BA;
    BytePair field_BB[5];
    char field_C5;
} Rec8B3DFC;
MAIN3_SIZE(Rec8B3DFC, 0xC6);
MAIN3_AT(Rec8B3DFC, field_5A, 0x5A);
MAIN3_AT(Rec8B3DFC, field_9A, 0x9A);
MAIN3_AT(Rec8B3DFC, field_A2, 0xA2);
MAIN3_AT(Rec8B3DFC, field_B6, 0xB6);
MAIN3_AT(Rec8B3DFC, field_BA, 0xBA);
MAIN3_AT(Rec8B3DFC, field_BB, 0xBB);
MAIN3_AT(Rec8B3DFC, field_C5, 0xC5);

typedef struct Party8B3DFC {
    Rec8B3DFC records[9];
    char pad_6F6[0x6D];
    char current;
} Party8B3DFC;
MAIN3_AT(Party8B3DFC, current, 0x763);

void func_088B74A4(Rec8B3DFC *record);
void func_088B3EDC(void *self, Party8B3DFC *party, int arg1, int arg2, int arg3);
int func_088B5254(void *self, char *buffer, int count, Party8B3DFC *party, int arg1, int arg2);
void func_088B5670(void *self, Rec8B3DFC *record, char *buffer, int count, Party8B3DFC *party,
                   int arg1, int arg2, int arg3);
void func_088B3DFC(void *self, Party8B3DFC *party, int arg1, int arg2, int arg3, int arg4);
Holder *func_088B7060(Holder *self);

/* ------------------------------------------------------------ 0x088B7080..0x088C1B3C */

/* A table of 0x90-byte entries sorted for func_08934944 (bsearch) with the
 * compare function func_088B7054; reached through D_089D7368. */
typedef struct Table8B7080 {
    int field_00;
    int count;
    char entries[1][0x90];
} Table8B7080;
MAIN3_AT(Table8B7080, count, 0x4);
MAIN3_AT(Table8B7080, entries, 0x8);

typedef struct Db8B7080 {
    Table8B7080 *table;
} Db8B7080;
MAIN3_SIZE(Db8B7080, 0x4);

extern Db8B7080 *D_089D7368;
int func_088B7054(const void *a, const void *b);
void *func_088B7080(Db8B7080 *self, int key);
void func_088B70C8(int key);

void func_088B73FC(Rec8B3DFC *self);
void func_088B745C(Rec8B3DFC *self);
void func_088B7480(Rec8B3DFC *self);
void func_088B74D0(Rec8B3DFC *self);
void func_088B74FC(Rec8B3DFC *self);
void func_088B7538(Rec8B3DFC *self);
void func_088B758C(Rec8B3DFC *self);
void func_088B75B4(Rec8B3DFC *self);
void func_088B75C0(Rec8B3DFC *self);
void func_088B764C(Rec8B3DFC *self, int index, unsigned char value);

/* Bytes at +0 and rows of three bytes at +0xC. */
typedef struct Obj8B7D94 {
    unsigned char field_00[0xC];
    unsigned char field_0C[1][3];
} Obj8B7D94;
MAIN3_AT(Obj8B7D94, field_0C, 0xC);

void func_088B7D30(void *self);
void func_088B7D58(void *self);
void func_088B7D04(void *self);
void func_088B7D94(Obj8B7D94 *self, int index, unsigned char value);
void func_088B7DA4(Obj8B7D94 *self, int row, int column, unsigned char value);
void func_088B87AC(void);
void func_088BB76C(short *self);

extern Obj87A61C D_09B4E838, D_09B4E848, D_09B68B48, D_09B4E828, D_09B69260;  /* static objects */
extern int D_089D7390, D_089D739C, D_089D73C0, D_089D7418, D_089D7424;   /* their destructor entries */
void func_088BBA64(void);
void func_088BD2C4(void);
void func_088C14B0(void);

Holder *func_088BBD38(Holder *self);
extern Holder *D_089D73A8;
void func_088BBD58(Holder *self, int arg);
void func_088BBD8C(int arg);
void func_088BBE78(void);
void func_088BBE5C(void);

extern void *D_089D73B0;
void func_088BC764(void *self, int arg);
void func_088BC798(void *self, int arg);
void func_088BC7CC(void *self, int arg);
unsigned char *func_088BC800(void *self, unsigned int arg1, unsigned int arg2, unsigned int arg3,
                             unsigned char arg4, unsigned char arg5, unsigned int arg6);
void func_088BC9F8(void *self, int arg);
void func_088BCA2C(int arg);
void func_088BCA50(int arg);
void func_088BCA74(int arg);
void func_088BCA98(unsigned int arg1, unsigned int arg2, unsigned int arg3, unsigned char arg4,
                   unsigned int arg5);
void func_088BCAD0(unsigned int arg1, unsigned int arg2, unsigned int arg3, unsigned char arg4,
                   unsigned int arg5);
void func_088BCB08(int arg);

/* Two words, twelve words and two words. */
typedef struct Obj8BD4E4 {
    int field_00;
    int field_04;
    int field_08[12];
    int field_38;
    int field_3C;
} Obj8BD4E4;
MAIN3_SIZE(Obj8BD4E4, 0x40);
MAIN3_AT(Obj8BD4E4, field_08, 0x8);
MAIN3_AT(Obj8BD4E4, field_38, 0x38);
MAIN3_AT(Obj8BD4E4, field_3C, 0x3C);

/* A word and an Obj8BD4E4. */
typedef struct Obj8BF08C {
    int field_00;
    Obj8BD4E4 field_04;
} Obj8BF08C;
MAIN3_SIZE(Obj8BF08C, 0x44);
MAIN3_AT(Obj8BF08C, field_04, 0x4);

/* Two Obj8BD4E4, two arrays of ten Obj8BF08C and three words. */
typedef struct Obj8BF108 {
    Obj8BD4E4 field_00;
    Obj8BD4E4 field_40;
    Obj8BF08C field_80[10];
    char pad_328[0x4];
    Obj8BF08C field_32C[10];
    char pad_5D4[0x4];
    int field_5D8;
    int field_5DC;
    int field_5E0;
} Obj8BF108;
MAIN3_SIZE(Obj8BF108, 0x5E4);
MAIN3_AT(Obj8BF108, field_40, 0x40);
MAIN3_AT(Obj8BF108, field_80, 0x80);
MAIN3_AT(Obj8BF108, field_32C, 0x32C);
MAIN3_AT(Obj8BF108, field_5D8, 0x5D8);
MAIN3_AT(Obj8BF108, field_5E0, 0x5E0);

Obj8BD4E4 *func_088BD4E4(Obj8BD4E4 *self);
void func_088BD518(Obj8BD4E4 *self, unsigned int flags);
void func_088BD564(Obj8BD4E4 *self);
Obj8BF08C *func_088BF08C(Obj8BF08C *self);
void func_088BF0B8(Obj8BF08C *self, unsigned int flags);

typedef struct Obj8BF590 {
    int field_00;
    int *field_04;
} Obj8BF590;
MAIN3_SIZE(Obj8BF590, 0x8);
MAIN3_AT(Obj8BF590, field_04, 0x4);

int func_088BE63C(int arg1, int arg2, int arg3, int arg4, int *arg5);
int func_088BD3F4(int arg1, int arg2, int arg3, int arg4);
void func_088BF590(Obj8BF590 *self, int arg1, int arg2, int arg3, int arg4);
void func_088BF6A0(void *self, int arg1, int arg2, int arg3, int arg4);

typedef struct Obj8BFF94 {
    short field_00;
    short field_02;
    short field_04[4];
} Obj8BFF94;
MAIN3_SIZE(Obj8BFF94, 0xC);
MAIN3_AT(Obj8BFF94, field_04, 0x4);
void func_088BFF94(Obj8BFF94 *self);

typedef struct Obj8C01C0 {
    char field_00[0x8];
    char field_08[0x4];
    char field_0C[0x100];
    char field_10C[0x4];
    short field_110;
    short field_112;
    int field_114;
} Obj8C01C0;
MAIN3_SIZE(Obj8C01C0, 0x118);
MAIN3_AT(Obj8C01C0, field_08, 0x8);
MAIN3_AT(Obj8C01C0, field_0C, 0xC);
MAIN3_AT(Obj8C01C0, field_10C, 0x10C);
MAIN3_AT(Obj8C01C0, field_110, 0x110);
MAIN3_AT(Obj8C01C0, field_114, 0x114);
void func_088C01C0(Obj8C01C0 *self);
void func_088C0450(void);
void func_088C0498(void);

/* A value that grows with the level: base + rate * level / 100. */
typedef struct Growth {
    short base;
    short rate;
} Growth;
MAIN3_SIZE(Growth, 0x4);

/* Six growth pairs from +0x4. */
typedef struct Stats8C07B4 {
    int field_00;
    Growth field_04;
    Growth field_08;
    Growth field_0C;
    Growth field_10;
    Growth field_14;
    Growth field_18;
} Stats8C07B4;
MAIN3_SIZE(Stats8C07B4, 0x1C);
MAIN3_AT(Stats8C07B4, field_04, 0x4);
MAIN3_AT(Stats8C07B4, field_18, 0x18);

float func_088C08F0(int base, int rate, int level);
float func_088C0920(int base, int rate, int level);
int func_088C07B4(Stats8C07B4 *self, int level);
int func_088C07E0(Stats8C07B4 *self, int level);
int func_088C080C(Stats8C07B4 *self, int level);
int func_088C0838(Stats8C07B4 *self, int level);
int func_088C0864(Stats8C07B4 *self, int level);
int func_088C0890(Stats8C07B4 *self, int level);
int func_088C08BC(Stats8C07B4 *self, int level);

extern int D_089D73F8;
Holder *func_088C0950(Holder *self);
extern Holder *D_089D73FC;
void func_088C0978(Holder *self, int arg);
int func_088C09AC(Holder *self, unsigned int arg);
void func_088C09FC(int arg);
void func_088C0A20(int arg);
Holder *func_088C1B3C(Holder *self);

/* ------------------------------------------------------------ 0x088C1CD4..0x088CAF64 */

int func_088C1C50(int arg1, int arg2, Save8AB7A4 *save, int arg3, int arg4);
void func_088C1CD4(int arg1, int arg2, int arg3, int arg4);

/* A pair passed to the handlers at 0x088C2604 / 0x088C2640: the value is
 * read from the argument and written back to the result. */
typedef struct ValuePair {
    int field_00;
    int value;
} ValuePair;
MAIN3_SIZE(ValuePair, 0x8);
MAIN3_AT(ValuePair, value, 0x4);

void func_088CC440(int value);
void func_089200F0(void *self, int value);
int func_088C2604(void *context, ValuePair *result, int arg2, int arg3, ValuePair *args);
int func_088C2640(void *context, ValuePair *result, int arg2, int arg3, ValuePair *args);

/* 100 two-byte elements, each built by func_088C2838. */
typedef struct Obj8C284C {
    short elems[100];
} Obj8C284C;
void func_088C2838(void);
void func_088C288C(Obj8C284C *self);
Obj8C284C *func_088C284C(Obj8C284C *self);

void func_088C29C0(void *out, int arg1, int arg2, int arg3);
void func_088C2998(int arg1, int arg2, int arg3);

/* What the condition checks at 0x088C2C5C..0x088C4878 fill in: the current
 * value and the required one; they return required <= current. */
typedef struct Check {
    int current;
    int required;
} Check;
MAIN3_SIZE(Check, 0x8);
MAIN3_AT(Check, required, 0x4);

/* The context func_088C3A20 reads (+0xE788). */
typedef struct Ctx8C3A20 {
    char pad_00[0xE788];
    char field_E788[0x4];
} Ctx8C3A20;
MAIN3_AT(Ctx8C3A20, field_E788, 0xE788);

int func_088ABA9C(Save8AB7A4 *self);
int func_088B91C8(void *self);
int func_088F1D7C(void);
char func_088F27E4(void);
int func_089203CC(void *self);
short func_08920454(void *self);
short func_0892044C(void *self);
short func_089204A4(void *self);
char func_08920544(void *self);
short func_089204F4(void *self);
int func_088F2BD4(void *self);
int func_088F2BEC(void *self);
int func_088CC22C(void);
int func_088CC2B0(void);
short func_089203C4(void *self);
unsigned char func_088C2C5C(Check *out, int arg, const int *required);
unsigned char func_088C3A20(Check *out, Ctx8C3A20 *context, const int *required);
unsigned char func_088C3C70(Check *out, int arg, const int *required);
unsigned char func_088C3D30(Check *out, int arg, const int *required);
unsigned char func_088C4098(Check *out, int arg, const int *required);
unsigned char func_088C40F0(Check *out, int arg, const int *required);
unsigned char func_088C4148(Check *out, int arg, const int *required);
unsigned char func_088C41A0(Check *out, int arg, const int *required);
unsigned char func_088C41F8(Check *out, int arg, const int *required);
unsigned char func_088C4250(Check *out, int arg, const int *required);
unsigned char func_088C4540(Check *out, int arg, const int *required);
unsigned char func_088C4628(Check *out, int arg, const int *required);
unsigned char func_088C483C(Check *out);
unsigned char func_088C4878(Check *out, int arg, const int *required);

/* A table of 0x58-byte entries after two words. */
typedef struct Table8C6F20 {
    int field_00;
    int field_04;
    char entries[1][0x58];
} Table8C6F20;
MAIN3_AT(Table8C6F20, field_04, 0x4);
MAIN3_AT(Table8C6F20, entries, 0x8);

/* Vtable D_089CBC38 at +0x28: a block (freed with func_08882158), 0x40-byte
 * items, a table (deleted) and an Obj87A61C at +0x18. D_089D773C points at one. */
typedef struct Obj9CBC38 {
    void *block;
    int field_04;
    int field_08;
    int field_0C;
    char (*items)[0x40];
    Table8C6F20 *table;
    Obj87A61C field_18;
    void *vtable;
} Obj9CBC38;
MAIN3_SIZE(Obj9CBC38, 0x2C);
MAIN3_AT(Obj9CBC38, items, 0x10);
MAIN3_AT(Obj9CBC38, table, 0x14);
MAIN3_AT(Obj9CBC38, field_18, 0x18);
MAIN3_AT(Obj9CBC38, vtable, 0x28);

extern int D_089CBC38;
extern Obj9CBC38 *D_089D773C;
Obj9CBC38 *func_088C6B64(Obj9CBC38 *self);
void func_088C6BA4(Obj9CBC38 *self, unsigned int flags);
void func_088C6C08(Obj9CBC38 *self);
void func_088C6C24(Obj9CBC38 *self);
int func_088C6D40(Obj9CBC38 *self, int arg1, int arg2);
void func_088C6EE0(Obj9CBC38 *self, int arg);
int func_088C6F14(Obj9CBC38 *self);
char *func_088C6F20(Obj9CBC38 *self, int index);
char *func_088C6F44(Obj9CBC38 *self, int index);
void func_088C6C7C(int arg1, int arg2);
void func_088C6CA4(int arg);
void func_088C6CC8(void);
void func_088C6CE8(int index);
void func_088C6D0C(int index);
Obj87A61C *func_088C6D30(void);

typedef struct Obj8C6F54 {
    short field_00;
    short field_02;
} Obj8C6F54;
MAIN3_SIZE(Obj8C6F54, 0x4);
void func_088C6F54(Obj8C6F54 *self);

/* Four shorts and two members built by func_088C6F68. */
typedef struct Obj8C6FD4 {
    short field_00;
    short field_02;
    short field_04;
    short field_06;
    char field_08[0x3C];
    char field_44[0x3C];
} Obj8C6FD4;
MAIN3_AT(Obj8C6FD4, field_08, 0x8);
MAIN3_AT(Obj8C6FD4, field_44, 0x44);
void func_088C6F68(void *self);
void func_088C6FD4(Obj8C6FD4 *self);

void func_088C7350(void);
void func_088C7674(void);
void func_088C7690(void);
unsigned int func_088C7F58(unsigned int value);
int func_088C7F78(void);
int func_088C7F9C(unsigned int value);

/* A 0x778-byte object built by func_088CB9E8 (31 of them start Obj8CB110). */
typedef struct Obj8CB9E8 {
    char data[0x778];
} Obj8CB9E8;
MAIN3_SIZE(Obj8CB9E8, 0x778);

/* A 0x1060-byte object built by func_088C823C on the base Obj8CB9E8. */
typedef struct Obj8C823C {
    Obj8CB9E8 base;
    char pad_778[0x56C];
    int field_CE4[20];
    int field_D34[128];
    char pad_F34[0x12C];
} Obj8C823C;
MAIN3_SIZE(Obj8C823C, 0x1060);
MAIN3_AT(Obj8C823C, field_CE4, 0xCE4);
MAIN3_AT(Obj8C823C, field_D34, 0xD34);

/* A word and two Obj8C823C. */
typedef struct Obj8C829C {
    int field_00;
    Obj8C823C field_04[2];
} Obj8C829C;
MAIN3_AT(Obj8C829C, field_04, 0x4);

/* Derives from Obj8C829C (D_089D6A10->field_7C8 points at one). */
typedef struct Obj8C9564 {
    Obj8C829C base;
    char pad_20C4[0x9CF0];
    int field_BDB4;
    char pad_BDB8[0x2C];
    int field_BDE4;
    unsigned int flags;
} Obj8C9564;
MAIN3_AT(Obj8C9564, field_BDB4, 0xBDB4);
MAIN3_AT(Obj8C9564, field_BDE4, 0xBDE4);
MAIN3_AT(Obj8C9564, flags, 0xBDE8);

Obj8CB9E8 *func_088CB9E8(Obj8CB9E8 *self);
Obj8C823C *func_088C823C(Obj8C823C *self);
void func_088C82E0(Obj8C829C *self);
Obj8C829C *func_088C829C(Obj8C829C *self);
void func_088C95A0(Obj8C9564 *self);
Obj8C9564 *func_088C9564(Obj8C9564 *self);
void func_088FB028(Obj8C823C *self);
void func_088FB1A4(Obj8C823C *self);
void func_088C98F0(Obj8C9564 *self);
void func_088C990C(Obj8C9564 *self);
void func_088C9928(Obj8C9564 *self, int value);
int func_088C854C(Obj8C9564 *self, int arg);
int func_088C88D4(Obj8C9564 *self, int arg);
int func_088C948C(Obj8C9564 *self, int arg);
void func_088CA4C8(Obj8C9564 *self);
void func_088CA4EC(Obj8C9564 *self);
void func_088CA540(Obj8C9564 *self);
int func_088CA658(int high, int low);
Words2 *func_088CAF64(Words2 *self);

/* ------------------------------------------------------------ 0x088CAF74..0x088D3174 */

/* A block and its data 0x10 bytes in (func_088CAF64 clears both). */
typedef struct Span8CAF74 {
    char *block;
    char *data;
} Span8CAF74;
MAIN3_SIZE(Span8CAF74, 0x8);
MAIN3_AT(Span8CAF74, data, 0x4);

void func_088CAF74(Span8CAF74 *self, char *block);
void func_088CAF90(Span8CAF74 *self);

/* 31 Obj8CB9E8, a member at +0xFD94 and an Obj8C284C at +0x123D8. */
typedef struct Obj8CB110 {
    Obj8CB9E8 elems[31];
    char pad_E708[0x168C];
    char field_FD94[0x2644];
    Obj8C284C field_123D8;
} Obj8CB110;
MAIN3_AT(Obj8CB110, field_FD94, 0xFD94);
MAIN3_AT(Obj8CB110, field_123D8, 0x123D8);
Obj8CB110 *func_088CB110(Obj8CB110 *self);

Words2 *func_088CB840(Words2 *self);
void func_088CB864(Words2 *self);
extern Words2 *D_089D77F4;
void func_088CB870(Words2 *self, int arg);
void func_088CB8A4(int arg);
void func_088CB9C8(void);

typedef struct Obj8CBB58 {
    unsigned char field_00;
    unsigned char field_01;
    unsigned char field_02;
} Obj8CBB58;
MAIN3_AT(Obj8CBB58, field_01, 0x1);
MAIN3_AT(Obj8CBB58, field_02, 0x2);
unsigned char func_088CBB58(Obj8CBB58 *self);
void func_088CBB3C(Obj8CBB58 *self);
void func_088CBD24(Obj8CBB58 *self, unsigned int arg1, unsigned int arg2);
void func_088CBDDC(Obj8CBB58 *self);

/* A handle from func_088DAA40 and a word. */
typedef struct Obj8CC4D8 {
    void *handle;
    int field_04;
} Obj8CC4D8;
MAIN3_SIZE(Obj8CC4D8, 0x8);
MAIN3_AT(Obj8CC4D8, field_04, 0x4);

void func_088D9CD8(void *handle);
void func_088D9CFC(void *handle);
void func_088D9B84(void *handle, float *matrix);
void func_088D9AF4(void *handle, int arg);
void func_088D9B48(void *handle, int value);
void func_088CC4D8(Obj8CC4D8 *self, void *source, int arg);
void func_088CC63C(Obj8CC4D8 *self);
void func_088CC664(Obj8CC4D8 *self);
void func_088CC68C(Obj8CC4D8 *self, float *matrix);
void func_088CC6C0(Obj8CC4D8 *self, int arg);
void func_088CC77C(void);
void func_088CC820(void);

/* Vtable D_089CBCE0 at +0x34: a byte and two words, a byte at +0x10 and
 * two Obj87A61C. */
typedef struct Obj9CBCE0 {
    char field_00;
    char pad_01[0x3];
    int field_04;
    int field_08;
    char pad_0C[0x4];
    char field_10;
    char pad_11[0x3];
    Obj87A61C field_14;
    Obj87A61C field_24;
    void *vtable;
} Obj9CBCE0;
MAIN3_SIZE(Obj9CBCE0, 0x38);
MAIN3_AT(Obj9CBCE0, field_10, 0x10);
MAIN3_AT(Obj9CBCE0, field_14, 0x14);
MAIN3_AT(Obj9CBCE0, field_24, 0x24);
MAIN3_AT(Obj9CBCE0, vtable, 0x34);

extern int D_089CBCE0;
Obj9CBCE0 *func_088CC83C(Obj9CBCE0 *self);
void func_088CC88C(Obj9CBCE0 *self, unsigned int flags);
void func_088CC904(Obj9CBCE0 *self);
void func_088CC914(Obj9CBCE0 *self);
void func_088CD278(Obj9CBCE0 *self);
void func_088CD280(Obj9CBCE0 *self);

/* Vtable D_089CBD10 at +0x40: an Obj9C9D30 at +0xC, a handle at +0x1C (-1
 * when unset) and an Obj87A61C at +0x2C. */
typedef struct Obj9CBD10 {
    char field_00;
    char pad_01[0x3];
    int field_04;
    int field_08;
    Obj9C9D30 field_0C;
    int field_1C;
    char field_20;
    char pad_21[0x3];
    int field_24;
    char field_28;
    char pad_29[0x3];
    Obj87A61C field_2C;
    char field_3C;
    char pad_3D[0x3];
    void *vtable;
} Obj9CBD10;
MAIN3_SIZE(Obj9CBD10, 0x44);
MAIN3_AT(Obj9CBD10, field_0C, 0xC);
MAIN3_AT(Obj9CBD10, field_1C, 0x1C);
MAIN3_AT(Obj9CBD10, field_20, 0x20);
MAIN3_AT(Obj9CBD10, field_24, 0x24);
MAIN3_AT(Obj9CBD10, field_28, 0x28);
MAIN3_AT(Obj9CBD10, field_2C, 0x2C);
MAIN3_AT(Obj9CBD10, field_3C, 0x3C);
MAIN3_AT(Obj9CBD10, vtable, 0x40);

extern int D_089CBD10;
extern int D_089D7810;
Obj9CBD10 *func_088CD3D4(Obj9CBD10 *self);
void func_088CD424(Obj9CBD10 *self, unsigned int flags);
void func_088CD49C(Obj9CBD10 *self);
void func_088CD4C8(Obj9CBD10 *self);
void func_088CDC58(Obj9CBD10 *self);
void func_088CDC60(Obj9CBD10 *self);
void func_088CDED8(Obj9CBD10 *self);

/* Vtable D_089CBD50 at +0xC: the static object D_09AF0278 that
 * func_088CE2A8 builds on first use. */
typedef struct Obj9CBD50 {
    unsigned char field_00;
    char pad_01[0x3];
    int field_04;
    char field_08;
    char pad_09[0x3];
    void *vtable;
} Obj9CBD50;
MAIN3_SIZE(Obj9CBD50, 0x10);
MAIN3_AT(Obj9CBD50, field_04, 0x4);
MAIN3_AT(Obj9CBD50, field_08, 0x8);
MAIN3_AT(Obj9CBD50, vtable, 0xC);

extern int D_089CBD50;
extern int D_09AF0274;          /* set once D_09AF0278 is built */
extern Obj9CBD50 D_09AF0278;
extern int D_089D7820;          /* its destructor entry */
Obj9CBD50 *func_088CDEE0(Obj9CBD50 *self);
void func_088CDF18(Obj9CBD50 *self, unsigned int flags);
void func_088CDF70(Obj9CBD50 *self);
void func_088CDF80(Obj9CBD50 *self);
int func_088CE0E4(Obj9CBD50 *self);
void func_088CE0C0(void);
unsigned char func_088CE2A0(Obj9CBD50 *self);
Obj9CBD50 *func_088CE2A8(void);

/* Vtable D_089CBD90 at +0x10. */
typedef struct Obj9CBD90 {
    unsigned char field_00;
    char pad_01[0x3];
    int field_04;
    int field_08;
    int field_0C;
    void *vtable;
} Obj9CBD90;
MAIN3_SIZE(Obj9CBD90, 0x14);
MAIN3_AT(Obj9CBD90, field_0C, 0xC);
MAIN3_AT(Obj9CBD90, vtable, 0x10);

extern int D_089CBD90;
Obj9CBD90 *func_088CE334(Obj9CBD90 *self);
void func_088CE3C4(Obj9CBD90 *self);
void func_088CE3D8(Obj9CBD90 *self);
unsigned char func_088CE71C(Obj9CBD90 *self);
int func_088CE724(Obj9CBD90 *self);

void func_088CF550(void *self, int arg);
void func_088CED34(void *self, int arg);
void func_088CED50(void *self, int arg);
void func_088CEE0C(void *self, int arg);
void func_088CF330(void);
void func_088CF314(void);
void func_088D1598(int arg);
void func_088D15BC(int arg);
void func_088D15E0(int arg);

int func_088D0684(unsigned int arg1, unsigned char arg2, unsigned int arg3);
void func_088DB10C(unsigned int arg);
void func_088DA924(unsigned int arg1, int arg2);
void func_088D0984(unsigned int arg1, unsigned int arg2, unsigned char arg3, unsigned short arg4);
unsigned int func_088D09D8(unsigned int arg1, unsigned char arg2, int arg3);

typedef struct Obj8D16A4 {
    char pad_00[0x1C];
    int handle;
} Obj8D16A4;
MAIN3_AT(Obj8D16A4, handle, 0x1C);
void func_088D16A4(Obj8D16A4 *self);

/* A handle released by func_088D2F30. */
typedef struct Obj8D2F30 {
    void *handle;
} Obj8D2F30;
MAIN3_SIZE(Obj8D2F30, 0x4);

void func_088D2F30(Obj8D2F30 *self);
void func_088D9F5C(void *handle, int arg, char flag);
void func_088DA0A8(void *handle, int arg, float *matrix, int *value);
void func_088D2E7C(Obj8D2F30 *self, unsigned int flags);
void func_088D2F68(Obj8D2F30 *self, int arg);
void func_088D2FB8(Obj8D2F30 *self, Obj8D2F30 *source, int arg, unsigned int flags);
void func_088D3174(Obj8D2F30 *self);

#endif /* DISSIDIA_MAIN_AUTO_REGION3_H */
