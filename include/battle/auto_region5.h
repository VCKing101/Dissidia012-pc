#ifndef DISSIDIA_BATTLE_AUTO_REGION5_H
#define DISSIDIA_BATTLE_AUTO_REGION5_H

/* Classes behind the automatically matched functions in battle
 * 0x08AF4D18..0x08B61C8C, plus two step functions of region 2's units
 * (readability pass, region 5).
 *
 * Same conventions as auto_region1.h .. auto_region4.h, which this header
 * builds on: "Part" structs keep their vtable pointer after their data and
 * are named after the address of their vtable (or, without one, of their
 * first function in this region); "Obj" structs keep the vtable at offset 0.
 * A part's first byte is cleared by its reset and tested first by its
 * getters, so it is called `active`. Objects of classes outside the region
 * are byte arrays named after the function that builds or releases them.
 *
 * Most classes here are only partly visible: the region is the tail of the
 * overlay, and many of their other functions were matched by hand or are
 * still unmatched. Where only a few fields are known the rest is padding.
 *
 * Functions that an earlier header already declares keep that declaration
 * (func_08AF614C and func_08AFCB44 take `void *`; func_08AFE308 takes a
 * ManagerE7CA0). */

#include "battle/auto_region4.h"

#ifdef __cplusplus
extern "C" {
#endif

#define REGION5_OFFSET(type, field) ((unsigned int)&((type *)0)->field)
#define REGION5_ASSERT(name, cond) typedef char region5_assert_##name[(cond) ? 1 : -1]
#define REGION5_SIZE(type, size) REGION5_ASSERT(size_##type, sizeof(type) == (size))
#define REGION5_AT(type, field, offset) \
    REGION5_ASSERT(at_##type##_##field, REGION5_OFFSET(type, field) == (offset))

/* ------------------------------------------------------------ main module */

void func_088D9AF4(void *handle, const void *value);
void func_088D9B48(void *handle, int value);
void func_088D9B84(void *handle, const void *matrix);
void func_088D9C60(void *handle, int arg1, int arg2);
void func_088D9CD8(void *handle);                   /* show */
void func_088D9CFC(void *handle);                   /* hide */
void *func_088DAA40(void *source, int arg1, int arg2);
void *func_088DAA68(void *source);
void func_088DAB44(void *object);
void func_0887B820(void *object, int value);
void func_0887B944(void *model);
void func_0887B970(void *model);
void func_0887B9C4(void *model);
void func_0887BA5C(void *model);                    /* release a model */
void func_0887E684(void *object);
unsigned char func_0887E718(void *object);
void *func_08885CC8(int arg0, int arg1, int arg2);
void func_08890A00(void *object);
void func_088A28E8(int arg0, char arg1);
void func_088A96D8(void *object);
void *func_088A9DE8(void *model, int id, int arg2, int arg3, int arg4);
char func_088CBAD0(void *object);
void func_088CC51C(void *object);
void func_088EC2F8(void *object);
void func_088EC544(void *object);
void func_0885EAFC(void *object);
void func_0899B6B0(void *begin, void *end);
void func_0899B6F4(void *begin, void *end);
void func_0899B728(void *begin, void *end);

/* Address ranges handed to func_0899B6B0 .. func_0899B728. */
extern unsigned char D_088664C4[];
extern unsigned char D_088664FC[];
extern unsigned char D_08866528[];
extern unsigned char D_08866560[];

/* ------------------------------------------------------------ globals */

typedef struct Global9D5EC0 {
    char pad_0000[0x8B0];
    unsigned char field_8B0[0x2421];
    unsigned char field_2CD1;
} Global9D5EC0;
REGION5_AT(Global9D5EC0, field_2CD1, 0x2CD1);

extern Global9D5EC0 *D_089D5EC0;
extern void *D_089D6998;
extern void *D_089D6DC8;
extern unsigned char D_09B6C288[];      /* two 0x10-byte objects */
extern int D_09B6C2A8;

/* ------------------------------------------------------------ PartF4458 */

void func_08AF4D18(PartF4458 *self);
void func_08AF54D0(PartF4458 *self);
void func_08AF5758(PartF4458 *self);
void func_08AF5794(PartF4458 *self);
void func_08AF57B8(PartF4458 *self);
void func_08AF57F4(PartF4458 *self);

/* ------------------------------------------------------------ PartF3D40 */

PartF3D40 *func_08AF3B90(PartF3D40 *self);
void func_08AF3CFC(PartF3D40 *self);
int func_08AF3DD4(PartF3D40 *self);

/* ------------------------------------------------------------ BufferF58B0 */

/* Constructor func_08AF58B0 clears it. */
typedef struct BufferF58B0 {
    unsigned char field_00[0xC];
    char pad_0C[8];
    int field_14;
    unsigned char field_18[0x400];
    unsigned char field_418[0x400];
    int field_818;
    int field_81C;
} BufferF58B0;
REGION5_SIZE(BufferF58B0, 0x820);

BufferF58B0 *func_08AF58B0(BufferF58B0 *self);

/* ------------------------------------------------------------ PartF5A80 */

/* Three handles shown by func_08AF5A80 before func_08AF5AE8 runs. */
typedef struct PartF5A80 {
    void *field_00;
    void *field_04;
    void *field_08;
} PartF5A80;

void func_08AF5A80(PartF5A80 *self);
void func_08AF5AE8(PartF5A80 *self);
void func_08AF5C6C(PartF5A80 *self);

/* ------------------------------------------------------------ Handle4C22C */

/* 0xC bytes: a handle and two words. Constructor func_08B4C22C, set up by
 * func_08B4C264, released by func_08B4C2B8. */
typedef struct Handle4C22C {
    void *handle;
    int field_04;
    int field_08;
} Handle4C22C;
REGION5_SIZE(Handle4C22C, 0xC);

void func_08B4C2B8(Handle4C22C *self);

/* ------------------------------------------------------------ PartF6008 */

/* Released by func_08AF605C: nine resources and five models, then its
 * members. field_F54 is a mode (0 and 7 have no field_6AC member to
 * release); field_F64 is freed by func_08AF6008. */
typedef struct PartF6008 {
    void *handles[9];
    void *models[5];
    char pad_38[0x18];
    unsigned char field_50[0x60];       /* released by func_08AF42C0 */
    unsigned char field_B0[0x94];       /* released by func_08AF49BC */
    unsigned char field_144[0x19C];     /* released by func_08AF522C */
    unsigned char field_2E0[0x148];     /* released by func_08B4CC24 */
    unsigned char field_428[0x250];     /* released by func_08B4B430 */
    unsigned char field_678[0x10];      /* released by func_088CC51C */
    Handle4C22C field_688;
    char pad_694[0xC];
    Handle4C22C field_6A0;
    unsigned char field_6AC[0x888];     /* released by func_08AF5A04 */
    int field_F34;
    char pad_F38[0x1C];
    int field_F54;
    char pad_F58[0xC];
    void *field_F64;
    float field_F68;
    unsigned char field_F6C;
} PartF6008;
REGION5_AT(PartF6008, field_50, 0x50);
REGION5_AT(PartF6008, field_2E0, 0x2E0);
REGION5_AT(PartF6008, field_678, 0x678);
REGION5_AT(PartF6008, field_6AC, 0x6AC);
REGION5_AT(PartF6008, field_F34, 0xF34);
REGION5_AT(PartF6008, field_F54, 0xF54);
REGION5_AT(PartF6008, field_F64, 0xF64);
REGION5_AT(PartF6008, field_F6C, 0xF6C);

void func_08AF42C0(void *object);
void func_08AF49BC(void *object);
void func_08AF522C(void *object);
void func_08AF5A04(void *object);
void func_08B4B430(void *object);
void func_08B4CC24(void *object);

void func_08AF6008(PartF6008 *self);
void func_08AF6050(PartF6008 *self);
void func_08AF605C(PartF6008 *self);
void func_08AF615C(PartF6008 *self);
void func_08AF62B8(PartF6008 *self);
void func_08AF62EC(PartF6008 *self);
void func_08AF64E4(PartF6008 *self, void *target, int index);
void func_08AF654C(PartF6008 *self, void *target, int index);

/* ------------------------------------------------------------ PartF66C0 */

/* field_54 is a phase: 1 set up, 3 hiding, 4 hidden. */
typedef struct PartF66C0 {
    char pad_00[0x10];
    void *field_10;
    char pad_14[0x40];
    int phase;
    char pad_58[0x1C];
    PartF3D40 field_74;
} PartF66C0;
REGION5_AT(PartF66C0, phase, 0x54);
REGION5_AT(PartF66C0, field_74, 0x74);

void func_08AF66C0(PartF66C0 *self);
void func_08AF66CC(PartF66C0 *self);
void func_08AF683C(PartF66C0 *self);

/* ------------------------------------------------------------ PartF6A34 */

/* Constructor func_08AF6A34. field_16C is a phase like PartF66C0's. */
typedef struct PartF6A34 {
    unsigned char field_00[0x2C];
    void *handles[0x42];
    unsigned char field_134[0x28];
    int field_15C;
    PartF3D40 field_160;
    int phase;
    unsigned char field_170;
} PartF6A34;
REGION5_AT(PartF6A34, field_134, 0x134);
REGION5_AT(PartF6A34, field_160, 0x160);
REGION5_AT(PartF6A34, phase, 0x16C);
REGION5_AT(PartF6A34, field_170, 0x170);

PartF6A34 *func_08AF6A34(PartF6A34 *self);
void func_08AF70C0(PartF6A34 *self);
void func_08AF70CC(PartF6A34 *self);
void func_08AF7258(PartF6A34 *self);
void func_08AF7294(void *self, void *target, void *handle, int index);
void func_08AF72F0(void *self, void *target, void *handle, int index);

/* ------------------------------------------------------------ PartF820C */

/* Starts with a PartF3D40; field_40 is a phase (2 hiding, 4 done). */
typedef struct PartF820C {
    PartF3D40 field_00;
    char pad_0C[0x18];
    int field_24;
    char pad_28[0x18];
    unsigned int phase;
    unsigned char field_44;
    unsigned char field_45;
} PartF820C;
REGION5_AT(PartF820C, field_24, 0x24);
REGION5_AT(PartF820C, phase, 0x40);

unsigned int func_08AF839C(PartF820C *self, int arg);
void func_08AF820C(PartF820C *self);
void func_08AF835C(PartF820C *self);

void func_08AF84D0(void);

typedef struct PartF8578 {
    char pad_00[4];
    void *field_04;
} PartF8578;

void func_08AF8578(PartF8578 *self);

/* ------------------------------------------------------------ PartFBE40 */

/* field_04 and field_18 are objects of main-module classes (released by
 * func_088EC544 and func_088EC2F8); field_28 is a phase. */
typedef struct PartFBE40 {
    char pad_00[4];
    unsigned char field_04[0x14];
    unsigned char field_18[0xC];
    int field_24;
    int phase;
    unsigned char field_2C;
    unsigned char field_2D;
    char pad_2E[2];
    void *field_30;
    void *handles[2];
    void *field_3C;
    int field_40;
} PartFBE40;
REGION5_AT(PartFBE40, field_24, 0x24);
REGION5_AT(PartFBE40, handles, 0x34);
REGION5_AT(PartFBE40, field_40, 0x40);

/* What func_08AFBF20 copies from. */
typedef struct SetupFBF20 {
    char pad_00[8];
    int field_08;
    char pad_0C[0xC];
    void *field_18;
    int field_1C;
} SetupFBF20;

void func_08AFBE40(PartFBE40 *self, int value);
void func_08AFBF20(PartFBE40 *self, SetupFBF20 *setup);
void func_08AFBF70(PartFBE40 *self);
void func_08AFC090(PartFBE40 *self);
void func_08AFC2E4(void);
void func_08AFC3A0(void);
void func_08AFC414(PartFBE40 *self);
void func_08AFC47C(void);
void func_08AFC4DC(void);
void func_08AFC4F8(PartFBE40 *self);

/* ------------------------------------------------------------ ObjFC570 */

/* Vtable at offset 0, destroyed through the main-module base destructor
 * func_088EC234. */
typedef struct ObjFC570 {
    void *vtable;
} ObjFC570;

extern unsigned char D_08AFC570[];
void func_08AFC514(ObjFC570 *self, int flags);

/* ------------------------------------------------------------ PartFC870 */

typedef struct PartFC870 {
    void *model;
    char pad_04[8];
    void *field_0C;
} PartFC870;

void func_08AFC870(PartFC870 *self, int id);
void *func_08AFC640(void *self);
void func_08AFCAA4(void);

/* ------------------------------------------------------------ PartFCB68 */

/* Constructor func_08AFCB68. field_08 is a 0xC0-byte block cleared as a
 * whole; field_B4 lies inside it. */
typedef struct PartFCB68 {
    int field_00;
    int field_04;
    unsigned char field_08[0xAC];
    int field_B4;
    char pad_B8[0x10];
    int field_C8;
    int field_CC;
    int field_D0;
    int field_D4;
    char pad_D8[0xC];
    unsigned char field_E4[4];          /* built by func_0885EAFC */
} PartFCB68;
REGION5_AT(PartFCB68, field_B4, 0xB4);
REGION5_AT(PartFCB68, field_C8, 0xC8);
REGION5_AT(PartFCB68, field_E4, 0xE4);

PartFCB68 *func_08AFCB68(PartFCB68 *self);

/* Deleting destructor, release (empty) and allocator of one class. */
void func_08AFCBCC(void *self, int flags);
void func_08AFCC78(void *self);
void *func_08AFCD74(unsigned int size);
void func_08AFCD90(void *object);
void *func_08AFE2AC(unsigned int size);
void func_08AFE2C8(void *object);

/* ------------------------------------------------------------ TimerFF1FC */

/* An integer value; func_08AFF33C reports it with a kind (0..6). */
typedef struct TimerFF1FC {
    int value;
} TimerFF1FC;

void func_08AFF33C(TimerFF1FC *self, int kind, int value);
void func_08AFF1FC(TimerFF1FC *self, int value);
void func_08AFF21C(TimerFF1FC *self);
void func_08AFF24C(TimerFF1FC *self);
void func_08AFF27C(TimerFF1FC *self);
void func_08AFF2AC(TimerFF1FC *self);
void func_08AFF2DC(TimerFF1FC *self);
void func_08AFF30C(TimerFF1FC *self);

/* ------------------------------------------------------------ PartFF60C */

typedef struct PartFF60C {
    char pad_00[8];
    int field_08;
    int field_0C;
} PartFF60C;

void func_08AE3B54(float arg0, unsigned int arg1, int arg2, int arg3, int arg4);
void func_08AFF60C(PartFF60C *self, int value);

void func_08B00008(void);

/* ------------------------------------------------------------ 0x08B009A0 */

int func_08B00B2C(int kind, int arg1, int arg2, int arg3, unsigned char flag, int arg5);
void func_08B009A0(int arg1, int arg2, int arg3, unsigned char flag, int arg5);
void func_08B009D4(int arg1, int arg2, int arg3, int arg5);
void func_08B00A04(int arg1, int arg2, int arg3, int arg5);

void *func_088CB164(int arg0, int arg1);
void func_08B01498(void *self, void *data, int arg);
void func_08B01450(void *self, int arg1, int arg2, int arg3);

/* ------------------------------------------------------------ Part014D8 */

/* Constructor func_08B014D8 (no vtable store), reset func_08B01558. */
typedef struct Part014D8 {
    unsigned char active;
    char pad_01[3];
    int field_04;
} Part014D8;

void func_08B01558(Part014D8 *self);
Part014D8 *func_08B014D8(Part014D8 *self);
void func_08B0150C(Part014D8 *self, int flags);
void func_08B015DC(Part014D8 *self);
unsigned char func_08B019E4(Part014D8 *self);

/* ------------------------------------------------------------ Part05B88 */

/* 0x24 bytes, held twice by Part05240. Release func_08B0532C. */
typedef struct Part05B88 {
    unsigned char active;
    char pad_01[7];
    void *field_08;
    char pad_0C[0x14];
    void *vtable;
} Part05B88;
REGION5_SIZE(Part05B88, 0x24);

extern unsigned char D_08B05B88[];
void *func_08B05260(void *self);
void func_08B052F0(Part05B88 *self);
void func_08B05370(Part05B88 *self);
void func_08B05298(Part05B88 *self, int flags);
void func_08B0532C(Part05B88 *self);
unsigned char func_08B056BC(Part05B88 *self);

/* ------------------------------------------------------------ Part05240 */

/* 0x68 bytes. The battle keeps one in D_08B05258. */
typedef struct Part05240 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    unsigned char field_0C[0x10];       /* built by func_0887A61C */
    Part05B88 field_1C[2];
    void *vtable;
} Part05240;
REGION5_SIZE(Part05240, 0x68);
REGION5_AT(Part05240, field_1C, 0x1C);

extern unsigned char D_08B05240[];
extern Part05240 *D_08B05258;
int func_08B05008(Part05240 *self);
void func_08B051DC(Part05240 *self);
void func_08B04CA0(Part05240 *self);
Part05240 *func_08B04BA8(Part05240 *self);
void func_08B04C00(Part05240 *self, int flags);
void func_08B04C8C(Part05240 *self);
void func_08B04D10(Part05240 *self);

void func_08B05EBC(void);
void func_08B05C78(void);

/* ------------------------------------------------------------ Part16D98 */

/* Three sources, three handles loaded from them; field_28 is a phase (1
 * playing, 2 done) and field_2C says which handle is playing. */
typedef struct Part16D98 {
    void *sources[5];
    void *handles[3];
    void *field_20[2];
    int phase;
    int step;
    unsigned char field_30;
} Part16D98;
REGION5_SIZE(Part16D98, 0x34);

Part16D98 *func_08B16D98(Part16D98 *self);
void func_08B17078(Part16D98 *self);
void func_08B170C0(Part16D98 *self);
void func_08B17104(Part16D98 *self);
void func_08B17270(Part16D98 *self);

/* ------------------------------------------------------------ Part06984 */

/* A state machine; func_08B081D4 moves it to another state. field_2C counts
 * up by D_089D69E0's frame time. */
typedef struct Part06984 {
    char pad_00[0x2C];
    float field_2C;
    char pad_30[0x30];
    Part16D98 field_60;
    char pad_94[4];
    int field_98;
    char field_9C;
    char pad_9D[3];
    int field_A0;
    char pad_A4[0x37E4];
    unsigned char field_3888;
} Part06984;
REGION5_AT(Part06984, field_60, 0x60);
REGION5_AT(Part06984, field_98, 0x98);
REGION5_AT(Part06984, field_A0, 0xA0);
REGION5_AT(Part06984, field_3888, 0x3888);

typedef struct Global9D69E0 {
    char pad_00[0x18];
    float field_18;                     /* frame time */
} Global9D69E0;

typedef struct Global9D6A00 {
    char pad_00[4];
    int field_04;
    char pad_08[0x10];
    unsigned char field_18;
} Global9D6A00;

/* More of the object D_089D6A10 points to than auto_region4.h's
 * Global9D6A10 shows (that struct names only field_7A4). */
typedef struct Global9D6A10Node {
    char pad_000[0x5A0];
    int field_5A0;
} Global9D6A10Node;

typedef struct Global9D6A10View {
    char pad_000[0x38];
    Global9D6A10Node *field_38;
    char pad_03C[0x30];
    unsigned char field_6C[0x7C];
    unsigned char field_E8[0x640];
    unsigned char field_728[4];
} Global9D6A10View;
REGION5_AT(Global9D6A10View, field_6C, 0x6C);
REGION5_AT(Global9D6A10View, field_E8, 0xE8);
REGION5_AT(Global9D6A10View, field_728, 0x728);

#define GLOBAL_9D6A10 ((Global9D6A10View *)D_089D6A10)

extern Global9D69E0 *D_089D69E0;
extern Global9D6A00 *D_089D6A00;
extern void *D_089D727C;

void func_08B081D4(Part06984 *self, int state);
unsigned char func_08B0AA58(void);
int func_08B0C5F4(Part06984 *self, int arg);
void func_08889060(void *object);
void func_088B1354(void *object, int arg1, void *callback, void *arg, int arg4);
void func_088CDA34(void *object);
void func_088CDB04(void *object);
void func_088CE72C(void *object);
void func_088DE6D4(void *object, int arg1, int arg2, unsigned int arg3);

int func_08B06984(Part06984 *self);
int func_08B06D8C(Part06984 *self);
void func_08B078A4(void);
void func_08B07B44(Part06984 *self);
void func_08B07D94(Part06984 *self);
int func_08B07DB0(Part06984 *self);
void func_08B07DF0(Part06984 *self);
void func_08B07FC8(Part06984 *self);
void func_08B0817C(Part06984 *self);
void func_08B0825C(Part06984 *self);
int func_08B08BEC(void);
void func_08B0940C(Part06984 *self);
void func_08B0944C(void *arg, Part06984 *self);
int func_08B09468(Part06984 *self, int arg1, int arg2);

/* ------------------------------------------------------------ Part096AC */

/* Three loaded objects: field_10 (freed with func_08890A00), the model
 * field_28 with its motion field_2C, and field_20. */
typedef struct Part096AC {
    char pad_00[0x10];
    void *field_10;
    char pad_14[4];
    int field_18;
    char pad_1C[4];
    void *field_20;
    int field_24;
    void *field_28;
    int field_2C;
} Part096AC;
REGION5_AT(Part096AC, field_20, 0x20);

extern void *D_089D6C30;
void func_0889F2B8(void *object);
void func_088A0CD4(void *object);
void func_088A0D1C(void *object);
void func_088A3D14(void *model, int motion);
void func_088A3EF8(void *model);
void func_08B096AC(Part096AC *self);

/* ------------------------------------------------------------ 0x08B09CAC */

typedef struct Part09CAC {
    char pad_00[0xC];
    int field_0C;
} Part09CAC;

/* Two words: a vtable and the object the calls are forwarded to. */
typedef struct Delegate {
    void *vtable;
    void *owner;
} Delegate;

void func_08B09B04(void *self, void *arg);
void func_08B09324(void *owner);
void func_08B09358(void *owner);
void func_08B0938C(void *owner);
void func_08B093C0(void *owner);
void func_08B09CAC(Part09CAC *self, void *arg);
void func_08B09CB8(void *arg, void *self);
void func_08B09CDC(void *arg, Part09CAC *self);
void func_08B09D00(Delegate *self);
void func_08B09D1C(Delegate *self);
void func_08B09D38(Delegate *self);
void func_08B09D54(Delegate *self);

/* ------------------------------------------------------------ 0x08B0D0E0 */

typedef struct Slot0D0E0 {
    int value;
} Slot0D0E0;

Slot0D0E0 *func_08B0D0E0(Slot0D0E0 *self);
void func_08B0D0EC(Slot0D0E0 *self, int value);

/* A loaded object (field_00) and a model with its motion. */
typedef struct Part0D8D8 {
    void *field_00;
    void *model;
    int motion;
    int field_0C;
} Part0D8D8;

void func_08B0D8D8(Part0D8D8 *self);

typedef struct Node0D9B0 {
    char pad_00[4];
    unsigned char field_04[4];
} Node0D9B0;

typedef struct Part0D9B0 {
    char pad_00[4];
    Node0D9B0 *field_04;
} Part0D9B0;

void func_0889F830(void *object, int arg);
void func_08B0D9B0(Part0D9B0 *self);

void func_08B0E2E0(void);
void func_08B0DABC(void);

/* ------------------------------------------------------------ Part0F410 */

/* field_30 names the object func_08B11718 looks up. */
typedef struct Part0F410 {
    char pad_00[0x30];
    int field_30;
} Part0F410;

void *func_08B11718(int id);
int func_08A80B90(void *object, char flag);
int func_08A80EBC(void *object);
int func_08A81000(void *object);
int func_08A81028(void *object);
int func_08A81050(void *object);
int func_08A81078(void *object);
unsigned int func_08B0F32C(Part0F410 *self);
void func_08B0F410(Part0F410 *self);
void func_08B0F688(Part0F410 *self);
void func_08B0F744(Part0F410 *self);
void func_08B0F788(Part0F410 *self);
void func_08B0F7CC(Part0F410 *self);
void func_08B0F810(Part0F410 *self);

/* ------------------------------------------------------------ Part0E63C */

/* Four buffers, an object of the class func_08B10A6C destroys, two
 * Part0F410 at field_04 and field_08 (the earlier words are their
 * pointers too), and a table set up by func_08B0E6E4. */
typedef struct Part0E63C {
    void *field_00;
    Part0F410 *field_04;
    Part0F410 *field_08;
    void *field_0C;
    void *field_10;
    char pad_14[4];
    void *table;
    int field_1C;
    void *field_20;
} Part0E63C;
REGION5_AT(Part0E63C, field_20, 0x20);

void *func_08B0D0F4(void *table, int index);
void func_08B0E804(Part0E63C *self);
unsigned int func_08B0EA68(void *object);
void func_08B0FD08(void *object);
void func_08B10A6C(void *object, int flags);
unsigned int func_08B10BAC(void *object);
void func_08B0E63C(Part0E63C *self, int flags);
void func_08B0E6E4(Part0E63C *self, void *table, int index);
unsigned int func_08B0E724(Part0E63C *self);

typedef struct Part0DB40 {
    char pad_00[8];
    unsigned char field_08[0x88];       /* released by func_08B14484 */
    Part0E63C *field_90;
    char pad_94[0xC];
    unsigned char field_A0[4];          /* released by func_08B121DC */
} Part0DB40;
REGION5_AT(Part0DB40, field_90, 0x90);
REGION5_AT(Part0DB40, field_A0, 0xA0);

void func_08B0E4D4(Part0DB40 *self);
void func_08B121DC(void *object);
void func_08B14484(void *object);
void func_08B0DB40(Part0DB40 *self);

typedef struct Part0E8F0 {
    unsigned char field_00[0x40];
    int field_40;
    unsigned char field_44[8];
} Part0E8F0;

Part0E8F0 *func_08B0E8F0(Part0E8F0 *self);

/* ------------------------------------------------------------ 0x08B110F4 */

void func_0888A064(int arg);
int func_08B110F4(void);

typedef struct Node113C8b {
    char pad_00[0x9A];
    unsigned char field_9A;
} Node113C8b;

typedef struct Node113C8 {
    char pad_00[0x14];
    Node113C8b *field_14;
} Node113C8;

typedef struct Part113C8 {
    char pad_00[0x34];
    Node113C8 *field_34;
} Part113C8;

void func_08B113C8(Part113C8 *self);
void func_08B113DC(Part113C8 *self);

typedef struct Part114A0 {
    unsigned char *field_00;
} Part114A0;

void func_08B114A0(Part114A0 *self, int flags);

typedef struct Part12060 {
    int field_00;
    unsigned char field_04[0x14];
    int field_18;
} Part12060;

Part12060 *func_08B12060(Part12060 *self);

extern void *D_08B125C8;
extern unsigned char D_09B6C2B0[];      /* block built by func_0887A61C */
void func_08B12550(void);
void func_08B12630(void);

typedef struct Node126DC {
    char pad_000[0x88C];
    int field_88C;
} Node126DC;

typedef struct Part126DC {
    char pad_00[8];
    Node126DC *field_08;
} Part126DC;

extern void *D_089D5FB4;
unsigned char func_0887066C(void *object);
void func_08B126DC(Part126DC *self);
void *func_08B12CF4(void *self);

/* ------------------------------------------------------------ Part13234 */

/* Two delegates that forward to this object, then eight 0x78-byte entries
 * (built by func_08B131F8). */
typedef struct Part13234 {
    char pad_00[4];
    Delegate field_04;
    Delegate field_0C;
    int field_14;
    unsigned char entries[8][0x78];
    int field_3D8;
    int field_3DC;
    unsigned char field_3E0;
} Part13234;
REGION5_AT(Part13234, entries, 0x18);
REGION5_AT(Part13234, field_3D8, 0x3D8);
REGION5_AT(Part13234, field_3E0, 0x3E0);

/* A forwarding object whose target is its third word. */
typedef struct Delegate13AA4 {
    char pad_00[8];
    void *owner;
} Delegate13AA4;

extern unsigned char D_08B13AF8[];
extern unsigned char D_08B13B08[];
void *func_08B131F8(void *self);
void func_08B13624(void *owner);
void func_08B13678(void *owner);
Part13234 *func_08B13234(Part13234 *self);
void func_08B13670(void *owner);
void func_08B13AA4(Delegate13AA4 *self);
void func_08B13AC0(Delegate *self);
void func_08B13ADC(Delegate *self);

void func_08B13CA0(void *self);
void func_08B13EB4(void *self);
void func_08B13B70(void *self);

extern void *D_089D6AE0;
void func_08895B1C(void *object, void *arg);
void func_08895B84(void *object);
int func_0888E4D4(void *arg);
void func_0888E5D0(void);
void func_08B13C34(void *arg);
void func_08B13C6C(void);

/* ------------------------------------------------------------ Part143B0 */

typedef struct Part143B0 {
    unsigned char field_00[0xC];
    unsigned char field_0C[0x28];
    unsigned char field_34[0x18];
    unsigned char field_4C[0x10];       /* built by func_0887A61C */
    int field_5C;
    unsigned char field_60;
    unsigned char field_61;
} Part143B0;
REGION5_AT(Part143B0, field_4C, 0x4C);
REGION5_AT(Part143B0, field_5C, 0x5C);

Part143B0 *func_08B143B0(Part143B0 *self);
void func_08B14468(Part143B0 *self);

void func_08B14A08(void *self);
void func_08B14B7C(void *self);
void func_08B14768(void *self);

typedef struct Part148B4 {
    char pad_00[0xC];
    void *field_0C;
    void *field_10;
    char pad_14[0x1C];
    void *field_30;
} Part148B4;

void func_08B148B4(Part148B4 *self);

extern void *D_089D6DD0;
void func_088AB378(void *object, unsigned int arg);
void func_088AB500(void *object);
int func_08B177B4(void);
void func_08B177DC(void);

/* ------------------------------------------------------------ Part18810 */

/* 0x10 bytes. Reset func_08B18580, release func_08B18590. */
typedef struct Part18810 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    void *vtable;
} Part18810;
REGION5_SIZE(Part18810, 0x10);

extern unsigned char D_08B18810[];
void func_08B18590(Part18810 *self);
Part18810 *func_08B184F0(Part18810 *self);
void func_08B18528(Part18810 *self, int flags);
void func_08B18580(Part18810 *self);
unsigned char func_08B18804(Part18810 *self);

/* Two small classes with the vtable at offset 0 and one word of data. */
typedef struct Obj18FF0 {
    void *vtable;
    int field_04;
} Obj18FF0;

typedef struct Obj19060 {
    void *vtable;
    int field_04;
} Obj19060;

extern unsigned char D_08B18FF0[];
extern unsigned char D_08B19060[];
Obj18FF0 *func_08B1898C(Obj18FF0 *self, int value);
Obj19060 *func_08B18BBC(Obj19060 *self, int value);

/* ------------------------------------------------------------ Part19618 */

/* 0x14 bytes. Reset func_08B193B0, release func_08B193C4. */
typedef struct Part19618 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    int field_0C;
    void *vtable;
} Part19618;
REGION5_SIZE(Part19618, 0x14);

extern unsigned char D_08B19618[];
void func_08B193C4(Part19618 *self);
Part19618 *func_08B19320(Part19618 *self);
void func_08B19358(Part19618 *self, int flags);
void func_08B193B0(Part19618 *self);

/* ------------------------------------------------------------ Part19300 */

/* 0x20 bytes. Reset func_08B1916C, release func_08B19178. */
typedef struct Part19300 {
    unsigned char active;
    char pad_01[3];
    Part19618 field_04;
    unsigned int field_18;
    void *vtable;
} Part19300;
REGION5_SIZE(Part19300, 0x20);

extern unsigned char D_08B19300[];
Part19300 *func_08B190C8(Part19300 *self);
void func_08B19108(Part19300 *self, int flags);
void func_08B1916C(Part19300 *self);
void func_08B19178(Part19300 *self);
unsigned int func_08B192E0(Part19300 *self);

/* ------------------------------------------------------------ 0x08B1A0D8 */

/* The objects Part1D810 keeps at field_64 (reset func_08B1A0D8, release
 * func_08B1A458) and field_68 (reset func_08B1C008, release func_08B1C2FC);
 * func_08B1AAF8 resets a third class. Only the fields used here are known. */
typedef struct Part1A0D8 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    unsigned int field_08;
} Part1A0D8;

typedef struct Part1AAF8 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    char pad_08[4];
    unsigned int field_0C;
    int field_10;
    int field_14;
    int field_18;
    char pad_1C[4];
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    int field_38;
} Part1AAF8;
REGION5_AT(Part1AAF8, field_20, 0x20);
REGION5_AT(Part1AAF8, field_38, 0x38);

typedef struct Part1C008 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    char pad_0C[4];
    unsigned int field_10;
} Part1C008;

void func_08B1A458(Part1A0D8 *self);
int func_08B1A758(Part1A0D8 *self);
void func_08B1C2FC(Part1C008 *self);
int func_08B1C6EC(Part1C008 *self);
void func_08B1A0D8(Part1A0D8 *self);
unsigned int func_08B1A1FC(Part1A0D8 *self);
unsigned char func_08B1A21C(Part1A0D8 *self);
unsigned int func_08B1A610(Part1A0D8 *self);
unsigned char func_08B1A7A0(Part1A0D8 *self);
void func_08B1AAF8(Part1AAF8 *self);
unsigned char func_08B1AE60(Part1AAF8 *self);
unsigned char func_08B1AE84(Part1AAF8 *self);
void func_08B1C008(Part1C008 *self);
unsigned char func_08B1C12C(Part1C008 *self);
unsigned int func_08B1C898(Part1C008 *self);
unsigned char func_08B1C998(Part1C008 *self);
unsigned char func_08B1C9C0(Part1C008 *self);
unsigned char func_08B1C9E8(Part1C008 *self);
unsigned char func_08B1CD40(void *self, int id);

/* ------------------------------------------------------------ Part1D810 */

/* 0x70 bytes. field_04 is a phase; field_64 and field_68 are allocated
 * from the heap at field_0C. The battle keeps one in D_08B1D820. */
typedef struct Part1D810 {
    unsigned char active;
    char pad_01[3];
    int phase;
    int field_08;
    void *heap;
    void *field_10;
    unsigned char field_14[0x10];       /* built by func_0887A61C */
    unsigned char field_24[0x10];       /* built by func_0887A61C */
    unsigned char field_34[0x10];       /* built by func_0887A61C */
    Part19300 field_44;
    Part1A0D8 *field_64;
    Part1C008 *field_68;
    void *vtable;
} Part1D810;
REGION5_SIZE(Part1D810, 0x70);
REGION5_AT(Part1D810, field_44, 0x44);

extern unsigned char D_08B1D810[];
extern Part1D810 *D_08B1D820;
void func_0885C538(void *heap, void *object, int arg);
Part1D810 *func_08B1CD68(Part1D810 *self);
void func_08B1CDC0(Part1D810 *self, int flags);
void func_08B1CE50(Part1D810 *self);
void func_08B1CE74(Part1D810 *self);
int func_08B1D190(Part1D810 *self);
void func_08B1D77C(Part1D810 *self);
void func_08B05DCC(void);

/* ------------------------------------------------------------ Part1FCA8 */

/* 0x40 bytes. Reset func_08B1D938. */
typedef struct Part1FCA8 {
    unsigned char active;
    char pad_01[0x3B];
    void *vtable;
} Part1FCA8;
REGION5_SIZE(Part1FCA8, 0x40);

extern unsigned char D_08B1FCA8[];
void func_08B1D938(Part1FCA8 *self);
Part1FCA8 *func_08B1D8A8(Part1FCA8 *self);
void func_08B1D8E0(Part1FCA8 *self, int flags);
void func_08B1D980(Part1FCA8 *self);
unsigned char func_08B1DBC8(Part1FCA8 *self);
int func_08B1E5D4(void);

/* ------------------------------------------------------------ Part20A68 */

/* 0x2C bytes, held twice by Part212C8. Reset func_08B1FEF8, release
 * func_08B1FF28. */
typedef struct Part20A68 {
    unsigned char active;
    char pad_01[3];
    int phase;
    int field_08;
    char field_0C;
    char pad_0D[3];
    int field_10;
    int field_14;
    void *field_18;
    int field_1C;
    int field_20;
    int field_24;
    void *vtable;
} Part20A68;
REGION5_SIZE(Part20A68, 0x2C);

extern unsigned char D_08B20A68[];
void *func_08B1FE68(void *self);
void func_08B1FEA0(Part20A68 *self, int flags);
void func_08B1FEF8(Part20A68 *self);
void func_08B1FF28(Part20A68 *self);
unsigned char func_08B20118(Part20A68 *self);
int func_08B203C8(Part20A68 *self);
int func_08B205BC(void);

/* ------------------------------------------------------------ Part212C8 */

/* 0x70 bytes. Reset func_08B20B50, release func_08B20B70. */
typedef struct Part212C8 {
    unsigned char active;
    char pad_01[0xB];
    Part20A68 field_0C[2];
    char pad_64[8];
    void *vtable;
} Part212C8;
REGION5_SIZE(Part212C8, 0x70);

extern unsigned char D_08B212C8[];
void func_08B20B50(Part212C8 *self);
void func_08B20B70(Part212C8 *self);
Part212C8 *func_08B20A88(Part212C8 *self);
void func_08B20AD8(Part212C8 *self, int flags);
unsigned char func_08B21010(Part212C8 *self);
int func_08B212C0(void);

/* ------------------------------------------------------------ Part22EF8 */

/* 0x1D8 bytes, with two of region 2's Part05658. Reset func_08B213B8,
 * release func_08B21778. */
typedef struct Part22EF8 {
    unsigned char active;
    char pad_01[0x13];
    Part05658 field_14[2];
    char pad_164[0x70];
    void *vtable;
} Part22EF8;
REGION5_SIZE(Part22EF8, 0x1D8);

extern unsigned char D_08B22EF8[];
Part05658 *func_08A04928(Part05658 *self);
void func_08B213B8(Part22EF8 *self);
void func_08B21778(Part22EF8 *self);
Part22EF8 *func_08B212F0(Part22EF8 *self);
void func_08B21340(Part22EF8 *self, int flags);
unsigned char func_08B21478(Part22EF8 *self);

typedef struct Node214DC {
    char pad_00[0x14];
    unsigned char flags;
} Node214DC;

typedef struct Part214DC {
    char pad_00[4];
    Node214DC *field_04;
} Part214DC;

int func_08B214DC(Part214DC *self);

/* ------------------------------------------------------------ Part2368C */

typedef struct Part2368C {
    char pad_000[4];
    void *field_04;
    char pad_008[0x28];
    unsigned char field_30;
    char pad_031[0xF];
    unsigned char field_40[0xC0];       /* built by func_0885E7C0 */
    unsigned char field_100[0x98];      /* built by func_08873498 */
    int field_198;
    char pad_19C[3];
    unsigned char field_19F;
    char pad_1A0[4];
    void *field_1A4;
} Part2368C;
REGION5_AT(Part2368C, field_40, 0x40);
REGION5_AT(Part2368C, field_198, 0x198);
REGION5_AT(Part2368C, field_1A4, 0x1A4);

void func_0885E7C0(void *object);
void *func_08873498(void *object);
void *func_088CA42C(void *object);
void func_08B235BC(void *object);
void func_08B2721C(void *object);
void func_08B274AC(void *object);
void func_08B236EC(Part2368C *self, int arg1, int arg2, int arg3, int arg4,
                   int arg5, int arg6, int arg7, int arg8, int arg9);
Part2368C *func_08B2368C(Part2368C *self);
unsigned char func_08B23D4C(Part2368C *self);
int func_08B23D54(Part2368C *self);
unsigned int func_08B23DFC(Part2368C *self, unsigned char enable);
void func_08B23EDC(Part2368C *self);
void func_08B23F04(Part2368C *self);
void func_08B23F20(Part2368C *self);

int func_08897EFC(void *object);
void *func_08899300(void);
void func_08B24518(void);

/* ------------------------------------------------------------ Part2CCA8 */

/* 0x28 bytes. Reset func_08B2C2F8, release func_08B2C334. */
typedef struct Part2CCA8 {
    unsigned char active;
    char pad_01[0x23];
    void *vtable;
} Part2CCA8;
REGION5_SIZE(Part2CCA8, 0x28);

extern unsigned char D_08B2CCA8[];
void func_08B2C2F8(Part2CCA8 *self);
void func_08B2C334(Part2CCA8 *self);
Part2CCA8 *func_08B2C268(Part2CCA8 *self);
void func_08B2C2A0(Part2CCA8 *self, int flags);
unsigned char func_08B2C558(Part2CCA8 *self);

/* ------------------------------------------------------------ Part2C228 */

/* 0x24 bytes. Reset func_08B2BB98, release func_08B2BBC0. */
typedef struct Part2C228 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    void *vtable;
} Part2C228;
REGION5_SIZE(Part2C228, 0x24);

extern unsigned char D_08B2C228[];
void func_08B2BBC0(Part2C228 *self);
Part2C228 *func_08B2BB08(Part2C228 *self);
void func_08B2BB40(Part2C228 *self, int flags);
void func_08B2BB98(Part2C228 *self);
unsigned char func_08B2BD98(Part2C228 *self);

/* ------------------------------------------------------------ Part2BAF0 */

typedef struct Node2BAF0 {
    char pad_0000[0xBDA0];
    int field_BDA0;
} Node2BAF0;

/* 0x1BC bytes. Reset func_08B29F80, release func_08B29FF0. */
typedef struct Part2BAF0 {
    unsigned char active;
    char pad_001[0x1AF];
    Node2BAF0 *field_1B0;
    char pad_1B4[4];
    void *vtable;
} Part2BAF0;
REGION5_SIZE(Part2BAF0, 0x1BC);
REGION5_AT(Part2BAF0, field_1B0, 0x1B0);

extern unsigned char D_08B2BAF0[];
void func_08B29F80(Part2BAF0 *self);
void func_08B29FF0(Part2BAF0 *self);
int func_088C8464(void *object);
int func_088C913C(void *object);
Part2BAF0 *func_08B29EF0(Part2BAF0 *self);
void func_08B29F28(Part2BAF0 *self, int flags);
unsigned char func_08B2A1A8(Part2BAF0 *self);
unsigned char func_08B2A4F8(Part2BAF0 *self, int id);
int func_08B2A528(Part2BAF0 *self);
void func_08B2A698(Part2BAF0 *self);
void func_08B2A730(Part2BAF0 *self);
int func_08B2A7BC(Part2BAF0 *self);

/* ------------------------------------------------------------ Part29ED8 */

/* 0x224 bytes. Reset func_08B29A14, release func_08B29A30. */
typedef struct Part29ED8 {
    unsigned char active;
    char pad_001[3];
    void *field_04;
    void *field_08;
    Part2CCA8 field_0C;
    Part2C228 field_34;
    Part2BAF0 field_58;
    int field_214;
    int field_218;
    int field_21C;
    void *vtable;
} Part29ED8;
REGION5_SIZE(Part29ED8, 0x224);
REGION5_AT(Part29ED8, field_58, 0x58);
REGION5_AT(Part29ED8, field_214, 0x214);

extern unsigned char D_08B29ED8[];
Part29ED8 *func_08B29948(Part29ED8 *self);
void func_08B29998(Part29ED8 *self, int flags);
void func_08B29A14(Part29ED8 *self);
void func_08B29A30(Part29ED8 *self);
unsigned char func_08B29B6C(Part29ED8 *self);

/* ------------------------------------------------------------ Part297C8 */

/* 0x68 bytes. Reset func_08B28F18, release func_08B28F70. */
typedef struct Part297C8 {
    unsigned char active;
    char pad_01[3];
    void *field_04;
    char pad_08[0x5C];
    void *vtable;
} Part297C8;
REGION5_SIZE(Part297C8, 0x68);

extern unsigned char D_08B297C8[];
void func_08B28F18(Part297C8 *self);
void func_08B28FB4(Part297C8 *self);
Part297C8 *func_08B28E88(Part297C8 *self);
void func_08B28EC0(Part297C8 *self, int flags);
void func_08B28F70(Part297C8 *self);
unsigned char func_08B291D8(Part297C8 *self);

/* ------------------------------------------------------------ Part28E48 */

/* 0x28 bytes. Reset func_08B28620, release func_08B28650. */
typedef struct Part28E48 {
    unsigned char active;
    char pad_01[0x23];
    void *vtable;
} Part28E48;
REGION5_SIZE(Part28E48, 0x28);

extern unsigned char D_08B28E48[];
void func_08B28620(Part28E48 *self);
void func_08B28650(Part28E48 *self);
Part28E48 *func_08B28590(Part28E48 *self);
void func_08B285C8(Part28E48 *self, int flags);
unsigned char func_08B28818(Part28E48 *self);

/* ------------------------------------------------------------ Part28570 */

/* 0x2DC bytes. Reset func_08B27A80, release func_08B27A9C. */
typedef struct Part28570 {
    unsigned char active;
    char pad_001[3];
    int field_04;
    int field_08;
    int field_0C;
    char field_10;
    char pad_011[3];
    unsigned char field_14[0x10];       /* built by func_0887A61C */
    Part29ED8 field_24;
    Part297C8 field_248;
    Part28E48 field_2B0;
    void *vtable;
} Part28570;
REGION5_SIZE(Part28570, 0x2DC);
REGION5_AT(Part28570, field_248, 0x248);
REGION5_AT(Part28570, field_2B0, 0x2B0);

extern unsigned char D_08B28570[];
int func_08B2800C(void);
Part28570 *func_08B27998(Part28570 *self);
void func_08B279F0(Part28570 *self, int flags);
void func_08B27A80(Part28570 *self);
void func_08B27A9C(Part28570 *self);
void func_08B27AE0(Part28570 *self);

/* ------------------------------------------------------------ Part2D280 */

/* 0x1C bytes. Reset (and release) func_08B2CE04. */
typedef struct Part2D280 {
    char pad_00[0x18];
    void *vtable;
} Part2D280;
REGION5_SIZE(Part2D280, 0x1C);

extern unsigned char D_08B2D280[];
void func_08B2CE04(Part2D280 *self);
Part2D280 *func_08B2CCD0(Part2D280 *self);
void func_08B2CD08(Part2D280 *self, int flags);

int func_08B382B8(void *self, int arg1, int arg2);
void func_08B2D80C(void *self);
void func_08B2D82C(void *self);

void func_08833E2C(void *self);
void func_08B333F0(void *self);
void func_08B2F028(void *self);

/* ------------------------------------------------------------ fighter */

/* The fighter fields used in this region (auto_region3.h's BattleFighter
 * and auto_region4.h's BattleFighter4 name others). */
typedef struct FighterNode39C {
    char pad_00[6];
    unsigned char field_06;
} FighterNode39C;

/* Nine 0xC6-byte entries from field_10; field_773 picks one. */
typedef struct FighterNode30C {
    char pad_000[0x10];
    unsigned char entries[9][0xC6];
    char pad_706[0x6D];
    char field_773;
} FighterNode30C;

typedef struct FighterNode390 {
    char pad_00[0x88];
    int field_88;
} FighterNode390;

typedef struct FighterSlot1028 {
    int field_00;
    char field_04;
    char field_05;
    char pad_06[2];
} FighterSlot1028;

typedef struct BattleFighter5 {
    char pad_0000[0x30C];
    FighterNode30C *field_30C;
    char pad_0310[0x80];
    FighterNode390 *field_390;
    char pad_0394[8];
    FighterNode39C *field_39C;
    char pad_03A0[0x18C];
    void *field_52C;
    char pad_0530[0x40];
    unsigned char field_570[0x10];
    unsigned char field_580[0x28];
    void *field_5A8;
    char pad_05AC[0x134];
    int field_6E0;
    char pad_06E4[0x1C];
    unsigned char field_700[0x4A0];
    unsigned char field_BA0[0x1C4];
    unsigned char field_D64[0x2B8];
    int field_101C;
    int field_1020;
    int field_1024;
    FighterSlot1028 field_1028[8];
    short field_1068;
    char pad_106A[0xD];
    unsigned char field_1077;
    char pad_1078[0x50];
    short field_10C8;
    short field_10CA;
    char pad_10CC[2];
    short field_10CE;
    char pad_10D0[0x50];
    unsigned char field_1120[0x184];
    unsigned char field_12A4;
    unsigned char field_12A5;
} BattleFighter5;
REGION5_AT(BattleFighter5, field_390, 0x390);
REGION5_AT(BattleFighter5, field_39C, 0x39C);
REGION5_AT(BattleFighter5, field_52C, 0x52C);
REGION5_AT(BattleFighter5, field_570, 0x570);
REGION5_AT(BattleFighter5, field_5A8, 0x5A8);
REGION5_AT(BattleFighter5, field_6E0, 0x6E0);
REGION5_AT(BattleFighter5, field_700, 0x700);
REGION5_AT(BattleFighter5, field_BA0, 0xBA0);
REGION5_AT(BattleFighter5, field_D64, 0xD64);
REGION5_AT(BattleFighter5, field_101C, 0x101C);
REGION5_AT(BattleFighter5, field_1068, 0x1068);
REGION5_AT(BattleFighter5, field_1077, 0x1077);
REGION5_AT(BattleFighter5, field_10C8, 0x10C8);
REGION5_AT(BattleFighter5, field_10CE, 0x10CE);
REGION5_AT(BattleFighter5, field_1120, 0x1120);
REGION5_AT(BattleFighter5, field_12A4, 0x12A4);

typedef struct Global9D4974 Global9D4974;
extern Global9D4974 *D_089D4974;

void func_088316B8(BattleFighter5 *fighter);
void func_08AC5958(void *object);
void func_08AC5A64(void *object);
void func_088B7A88(void *entry, int arg1, int arg2);
void func_08B33B08(BattleFighter5 *fighter);
int func_08842D3C(void *model, int arg1, int arg2, void *arg3);
float func_08A62F00(void *object);
int func_0884F6F4(void *object);
void func_08848078(Global9D4974 *object, int arg);

unsigned int func_08B2F794(BattleFighter5 *fighter);
void func_08B308B8(BattleFighter5 *fighter);
void func_08B30904(BattleFighter5 *fighter);
void func_08B33374(BattleFighter5 *fighter);
unsigned char func_08B3337C(BattleFighter5 *fighter);
int func_08B33394(BattleFighter5 *fighter);
void func_08B3339C(BattleFighter5 *fighter);
int func_08B333B4(BattleFighter5 *fighter);
unsigned char func_08B333BC(BattleFighter5 *fighter);
unsigned char func_08B333C8(BattleFighter5 *fighter);
unsigned char func_08B333D8(BattleFighter5 *fighter);
void *func_08B333E8(BattleFighter5 *fighter);
void func_08B33858(BattleFighter5 *fighter);
int func_08B3390C(BattleFighter5 *fighter);
void func_08B33B6C(BattleFighter5 *fighter, int arg1, int arg2);
void func_08B34160(BattleFighter5 *fighter, unsigned char value);
unsigned char func_08B3416C(BattleFighter5 *fighter);
void func_08B34A4C(BattleFighter5 *fighter, int arg1, int arg2);
void func_08B34C08(BattleFighter5 *fighter, const void *value);
void func_08B34C24(BattleFighter5 *fighter, const void *value);
float func_08B34C94(BattleFighter5 *fighter);
void func_08B34DB8(BattleFighter5 *fighter);
void func_08B34DD4(BattleFighter5 *fighter);

/* ------------------------------------------------------------ Part3AF54 */

typedef struct Part3AF54 {
    char pad_000[0x110];
    unsigned int field_110;
    char pad_114[4];
    unsigned char field_118;
    unsigned char field_119;
    char pad_11A[2];
    int field_11C;
    char pad_120[5];
    unsigned char field_125;
    char pad_126[0x22];
    unsigned char field_148[0x48];      /* run by func_08B4024C */
    unsigned char field_190[0x50];      /* a matrix */
    unsigned char field_1E0[4];
} Part3AF54;
REGION5_AT(Part3AF54, field_11C, 0x11C);
REGION5_AT(Part3AF54, field_125, 0x125);
REGION5_AT(Part3AF54, field_148, 0x148);
REGION5_AT(Part3AF54, field_190, 0x190);
REGION5_AT(Part3AF54, field_1E0, 0x1E0);

void *func_089A3EFC(void *dst, const void *src);   /* copy a 4x4 matrix */
void func_0880FE20(void *object);
void func_0880FE58(void *object);
void func_08B4024C(void *object);
unsigned int func_08B3B3EC(Part3AF54 *self, unsigned int arg1, unsigned char arg2);
void func_08B3AF54(Part3AF54 *self);
void func_08B3AF9C(Part3AF54 *self, const void *matrix);
void func_08B3AFB8(Part3AF54 *self);
unsigned char func_08B3B308(Part3AF54 *self);
void func_08B3B310(Part3AF54 *self, unsigned char value);
void func_08B3B5EC(Part3AF54 *self, int value);

void *func_08B3B918(unsigned int size);
void func_08B3B934(void *object);

/* ------------------------------------------------------------ Part3BBFC */

/* Entries of 0xB0 bytes from field_110; field_6D0 lists objects destroyed
 * with func_08B3AD98. */
typedef struct Entry3BECC {
    unsigned char field_00[0x10];
    unsigned char field_10[0x10];
    unsigned char field_20[0x90];
} Entry3BECC;
REGION5_SIZE(Entry3BECC, 0xB0);

typedef struct Part3BBFC {
    char pad_000[0x110];
    Entry3BECC field_110[8];
    char pad_690[0x40];
    void *field_6D0[0x20];
    unsigned char field_750[0x128];     /* released by func_0880FC58 */
    unsigned char field_878[4];         /* released by func_08B41B24 */
} Part3BBFC;
REGION5_AT(Part3BBFC, field_6D0, 0x6D0);
REGION5_AT(Part3BBFC, field_750, 0x750);
REGION5_AT(Part3BBFC, field_878, 0x878);

void func_0880FC58(void *object);
void func_08B41B24(void *object);
void func_08B41BA4(void);
void func_08B3AD98(void *object, int flags);
void func_08B3BD28(Part3BBFC *self, int index, void *entry);
void func_08B3BD9C(Part3BBFC *self, int index, void *entry);
void func_08B3BE10(Part3BBFC *self, int index, void *entry);
void func_08B3BBFC(Part3BBFC *self);
void func_08B3BCF4(Part3BBFC *self, int index);
void func_08B3BECC(Part3BBFC *self, int index);
void func_08B3BF00(Part3BBFC *self, int index);
void func_08B3BF34(Part3BBFC *self, int index);

extern void *D_08B3D6B8;
void func_08B3CA24(void *object);
void func_08B3C0D8(void *object);
void func_08B3C41C(void);
void func_08B3C43C(void);

/* ------------------------------------------------------------ Obj3E2E8 */

typedef struct Obj3E2E8 {
    void *vtable;
} Obj3E2E8;

typedef struct Obj3F118 {
    Obj3E2E8 base;
    int field_04;
    int field_08;
} Obj3F118;

extern unsigned char D_08B3E2E8[];
extern unsigned char D_08B3F118[];
Obj3E2E8 *func_08B3D6C0(Obj3E2E8 *self);
Obj3F118 *func_08B3E3C8(Obj3F118 *self);

typedef struct Global9D4790 {
    char pad_00[8];
    int field_08;
} Global9D4790;

typedef struct Part3E840 {
    char pad_00[4];
    int field_04;
} Part3E840;

typedef struct Node3E87C {
    char pad_00[0xFC];
    int field_FC;
} Node3E87C;

typedef struct Node3E888 {
    char pad_00[0x34];
    unsigned int flags;
} Node3E888;

extern Global9D4790 *D_089D4790;
Node3E888 *func_08B3E8B0(void);
void func_08B3E840(Part3E840 *self, int value);
unsigned char func_08B3E87C(void *self, Node3E87C *other);
unsigned char func_08B3E888(void);

/* ------------------------------------------------------------ 0x08B40A50 */

typedef struct Node40A50 {
    char pad_00[0x80];
    unsigned char field_80[4];
} Node40A50;

typedef struct Part40A50 {
    char pad_00[0xF8];
    Node40A50 *field_F8;
} Part40A50;

int func_08AB7B7C(void *object, void *arg);
void *func_08835778(void *object);
int func_088BF4CC(void *object, int id);
void func_08B40A50(void *self, Part40A50 *other);
unsigned char func_08B40A8C(void *self, Part40A50 *other);
unsigned char func_08B40AB8(void *self, Part40A50 *other);
void func_08B3D72C(void);
void func_08B40B54(void);

/* ------------------------------------------------------------ Handle41620 */

/* A handle with a phase: 1 shown, 2 to hide, 3 hidden. */
typedef struct Handle41620 {
    void *handle;
    int phase;
    int field_08;
    int field_0C;
} Handle41620;

typedef struct Part41AD0 {
    unsigned char field_00;
    unsigned char field_01;
    char pad_02[6];
    Handle41620 field_08;
    char pad_18[0x24];
    unsigned int field_3C;
} Part41AD0;
REGION5_AT(Part41AD0, field_3C, 0x3C);

extern void *D_08B422A8;
void func_08B4166C(Handle41620 *self);
Handle41620 *func_08B41620(Handle41620 *self);
void func_08B41638(Handle41620 *self, void *source);
void func_08B41790(Handle41620 *self);
void func_08B417EC(Handle41620 *self);
void func_08B418A0(Handle41620 *self, int arg1, int arg2);
void func_08B41A20(Handle41620 *self, const void *value);
void func_08B41AD0(Part41AD0 *self, unsigned char value);
void func_08B41B70(void);

/* ------------------------------------------------------------ Part44B78 */

/* Base built by func_08B44B78 and destroyed by func_08B44BCC; the vtable is
 * at field_14. Derived classes replace it. */
typedef struct Part44B78 {
    char pad_00[0x14];
    void *vtable;
    char pad_18[0x18];
} Part44B78;

typedef struct Part42EF8 {
    Part44B78 base;
} Part42EF8;

typedef struct Part43668 {
    Part44B78 base;
} Part43668;

typedef struct Part44858 {
    Part44B78 base;
} Part44858;

typedef struct Part46080 {
    Part44B78 base;
    int field_30;
    int field_34;
} Part46080;

typedef struct Part46668 {
    Part44B78 base;
    char pad_30[0x10];
    int field_40;
    int field_44;
    int field_48;
} Part46668;

typedef struct Part46A18 {
    Part44B78 base;
    int field_30;
} Part46A18;

typedef struct Part470C8 {
    Part44B78 base;
} Part470C8;

extern unsigned char D_08B42EF8[];
extern unsigned char D_08B43668[];
extern unsigned char D_08B44858[];
extern unsigned char D_08B46080[];
extern unsigned char D_08B46668[];
extern unsigned char D_08B46A18[];
extern unsigned char D_08B470C8[];
void *func_089110C4(unsigned int size);
void func_089110FC(void *object);
Part44B78 *func_08B44B78(Part44B78 *self);
void func_08B44BCC(Part44B78 *self, int flags);
void func_08B42350(Part42EF8 *self, int flags);
void func_08B43010(Part43668 *self, int flags);
Part44858 *func_08B44510(Part44858 *self);
void func_08B44544(Part44858 *self, int flags);
Part46080 *func_08B45C40(Part46080 *self);
void func_08B45C7C(Part46080 *self, int flags);
Part46668 *func_08B460E8(Part46668 *self);
void func_08B46128(Part46668 *self, int flags);
Part46A18 *func_08B466E0(Part46A18 *self);
void func_08B46718(Part46A18 *self, int flags);
void func_08B46AB4(Part470C8 *self, int flags);
void *func_08B43818(unsigned int size);
void func_08B43834(void *object);
void *func_08B44D2C(unsigned int size);
void func_08B44D48(void *object);
void *func_08B44F8C(unsigned int size);
void func_08B44FA8(void *object);

typedef struct Part437D4 {
    int field_00;
    unsigned char entries[0x20][0x14];  /* built by func_08B436C0 */
} Part437D4;

void *func_08B436C0(void *self);
Part437D4 *func_08B437D4(Part437D4 *self);

typedef struct Node452B4 {
    char pad_00[0xC];
    struct Node452B4 *next;
} Node452B4;

typedef struct Part452B4 {
    char pad_00[4];
    unsigned char field_04;
    char pad_05[3];
    Node452B4 *first;
} Part452B4;

void func_08B44CFC(Node452B4 *node);
void func_08B452B4(Part452B4 *self);

/* ------------------------------------------------------------ Part47170 */

/* 0xC bytes: a phase and two resources. */
typedef struct Part47170 {
    int phase;
    void *handles[2];
} Part47170;
REGION5_SIZE(Part47170, 0xC);

extern unsigned char D_08B47328[];
void func_088D3F00(int arg0, void *handles, const void *table);
void func_088D3CF0(int arg0, void *handles, const void *table);
void func_088D44EC(void *handles, void *models, const void *table);
void func_08B4724C(Part47170 *self);
Part47170 *func_08B47170(Part47170 *self);
void func_08B47198(Part47170 *self);
void func_08B471BC(Part47170 *self);
void func_08B471E8(Part47170 *self);
void func_08B47214(Part47170 *self);
void func_08B472D4(Part47170 *self);

/* ------------------------------------------------------------ Part47358 */

/* 0x4C bytes, held fourteen times by Part4785C. Reset func_08B47380. */
typedef struct Part47358 {
    int phase;
    int field_04;
    void *handles[3];
    char pad_14[4];
    void *models[5];
    unsigned char field_2C[0x20];
} Part47358;
REGION5_SIZE(Part47358, 0x4C);

extern unsigned char D_08B48490[];
void func_08B47380(Part47358 *self);
Part47358 *func_08B47358(Part47358 *self);
void func_08B47508(Part47358 *self);
int func_08B475DC(Part47358 *self);
void func_08B475E4(Part47358 *self);
void func_08B47710(Part47358 *self);
void func_08B47808(Part47358 *self);

/* ------------------------------------------------------------ Part49220 */

typedef struct Motion49220 {
    unsigned int flags;
    int field_04;
} Motion49220;

/* 0x58 bytes, held by Part4785C at field_58. Reset func_08B49248, release
 * func_08B492F4. */
typedef struct Part49220 {
    int phase;
    void *handles[7];
    void *models[1];
    int field_24;
    int field_28;
    Motion49220 field_2C;
    char pad_34[0x24];
} Part49220;
REGION5_SIZE(Part49220, 0x58);
REGION5_AT(Part49220, field_2C, 0x2C);

extern unsigned char D_08B49838[];
extern unsigned char D_08B49870[];
void func_088D46E4(Motion49220 *motion, int arg1, int arg2, int arg3, int arg4, int arg5);
void func_08B49290(Part49220 *self, void *resource, void *handle, int arg3, int arg4, int arg5);
void func_08B49598(Part49220 *self, int arg);
Part49220 *func_08B49220(Part49220 *self);
void func_08B49248(Part49220 *self);
void func_08B492F4(Part49220 *self);
void func_08B49328(Part49220 *self);
void func_08B4934C(Part49220 *self, int arg1, int arg2);
int func_08B493B0(Part49220 *self);
void func_08B49418(Part49220 *self);
void func_08B495DC(Part49220 *self);
void func_08B4962C(Part49220 *self);
void func_08B49710(Part49220 *self);
void func_08B497E4(Part49220 *self);

/* ------------------------------------------------------------ Part4785C */

/* Derives from the main-module class built by func_0892ABC0 (vtable at
 * field_24). field_28 is a phase; fourteen Part47358 entries follow, of
 * which field_4F0 picks one. */
typedef struct Part4785C {
    char pad_000[0x24];
    void *vtable;
    int phase;
    void *resources[3];
    void *handles[4];
    void *models[2];
    int field_50;
    int field_54;
    Part49220 field_58;
    Part47170 field_B0;
    Part47358 entries[14];
    char pad_4E4[4];
    int field_4E8;
    char pad_4EC[4];
    int field_4F0;
    char pad_4F4[0x24];
    int field_518;
} Part4785C;
REGION5_AT(Part4785C, resources, 0x2C);
REGION5_AT(Part4785C, field_50, 0x50);
REGION5_AT(Part4785C, field_B0, 0xB0);
REGION5_AT(Part4785C, entries, 0xBC);
REGION5_AT(Part4785C, field_4E8, 0x4E8);
REGION5_AT(Part4785C, field_518, 0x518);

extern unsigned char D_08B48448[];
extern unsigned char D_08B484C0[];
extern unsigned char D_08B484D8[];
void func_0892ABC0(void *self);
void func_08B478C0(Part4785C *self);
Part4785C *func_08B4785C(Part4785C *self);
void func_08B47E48(Part4785C *self);
int func_08B47EF8(Part4785C *self);
int func_08B47F00(Part4785C *self);
void func_08B47FD4(Part4785C *self);
void func_08B48240(Part4785C *self);
void func_08B48294(Part4785C *self);
void func_08B48300(Part4785C *self);
void func_08B483F0(Part4785C *self);

/* ------------------------------------------------------------ Part488E8 */

/* The battle keeps one in D_08B491E8. Reset func_08B488E8. */
typedef struct Part488E8 {
    void *field_00;
    char pad_04[0x34];
    int field_38;
    int field_3C;
    int field_40;
    int field_44;
    int field_48;
    int field_4C[2];
    int field_54;
    int field_58;
    int field_5C;
    int field_60;
    char pad_64[0xC];
    unsigned char field_70[0x10];       /* a vector */
    unsigned char field_80[0x10];       /* a vector */
    int field_90;
} Part488E8;
REGION5_AT(Part488E8, field_38, 0x38);
REGION5_AT(Part488E8, field_60, 0x60);
REGION5_AT(Part488E8, field_70, 0x70);
REGION5_AT(Part488E8, field_90, 0x90);

/* What func_08B485B8 copies two vectors from. */
typedef struct Source485B8 {
    char pad_00[0x80];
    unsigned char field_80[0x10];
    unsigned char field_90[0x10];
} Source485B8;

extern Part488E8 *D_08B491E8;
void func_08882158(void *object);
void func_08B48AF8(Part488E8 *self);
void func_08B48B78(Part488E8 *self);
void func_08B485A0(int index, int value);
void func_08B485B8(Source485B8 *source);
void func_08B488E8(Part488E8 *self);
void func_08B489BC(void);
void func_08B489DC(void);
void func_08B489FC(void);
void func_08B48A38(Part488E8 *self, int arg1, int arg2);
void func_08B48B70(Part488E8 *self);

/* ------------------------------------------------------------ 0x08B48C60 */

typedef struct Part48C60 {
    char pad_00[4];
    unsigned char field_04[4];
} Part48C60;

/* An entry of the tables func_08912FA4 and func_08912F7C walk. */
typedef struct Entry48C7C {
    short field_00;
    short field_02;
    short field_04;
    char pad_06[4];
    unsigned short flags;
} Entry48C7C;

void func_08865158(void *object);
void func_08912B04(void *list, int id);
Entry48C7C *func_08912F7C(int group, Entry48C7C *entry);
Entry48C7C *func_08912FA4(int id);
void *func_0891705C(int arg);
void func_08B48C60(Part48C60 *self);
void func_08B48C7C(void *self, int id);

/* ------------------------------------------------------------ Part49AAC */

/* Seven members of main-module classes (each set up through
 * func_0892ACAC); field_00 is passed to several of them, field_6EC is the
 * id func_08B49F08 started on field_5DC (-1 when none). */
typedef struct Part49AAC {
    int field_00;
    unsigned char field_04[0x1D0];
    unsigned char field_1D4[0x88];
    unsigned char field_25C[0x74];
    unsigned char field_2D0[0x38];
    unsigned char field_308[0x2D4];
    unsigned char field_5DC[0xBC];
    unsigned char field_698[0x54];
    int field_6EC;
    unsigned char field_6F0;
} Part49AAC;
REGION5_AT(Part49AAC, field_1D4, 0x1D4);
REGION5_AT(Part49AAC, field_25C, 0x25C);
REGION5_AT(Part49AAC, field_2D0, 0x2D0);
REGION5_AT(Part49AAC, field_308, 0x308);
REGION5_AT(Part49AAC, field_5DC, 0x5DC);
REGION5_AT(Part49AAC, field_698, 0x698);
REGION5_AT(Part49AAC, field_6EC, 0x6EC);

typedef struct Global9D712C {
    char pad_00000[0x10CC8];
    int field_10CC8;
    char pad_10CCC[0x2E87C];
    unsigned char field_3F548[4];
} Global9D712C;
REGION5_AT(Global9D712C, field_3F548, 0x3F548);

extern Global9D712C *D_089D712C;
void func_0892ACAC(void *object);
void func_0892BA64(void *object);
void func_0892BD10(void *object);
void func_0892BE78(void *object);
void func_0892C00C(void *object);
void func_0892CD08(void *object);
void func_0892CD98(void *object, int arg1, int arg2, int arg3, int arg4, int arg5);
void func_0892CE60(void *object);
void func_0892CF00(void *object);
void func_0892D1D8(void *object);
int func_0892D2C0(void *object, int arg1, int arg2, int arg3);
int func_0892D2DC(void *object, int arg1, int arg2, int arg3);
void func_0892D324(void *object);
void func_0892D424(void *object);
void func_0892DBE4(void *object);
int func_0892DD34(void *object);
void func_0892DD6C(void *object);
void func_0892FEF8(void *object);
void func_0892FFF4(void *object);
void func_08930C9C(void *object);
void func_089313C0(void *object);
void func_08931468(void *object);
void func_08932B64(void *object);
void func_08932C74(void *object, int arg1, int arg2, int arg3, int arg4, int arg5);
void func_08932E04(void *object);
void func_08932E48(void *object);
void func_08932E74(void *object);
int func_089203BC(void *object);
void func_08B49D60(Part49AAC *self);
void func_08B4A1AC(Part49AAC *self);
void func_08B49AAC(Part49AAC *self);
void func_08B49B04(Part49AAC *self);
void func_08B49C54(Part49AAC *self);
void func_08B49CF4(Part49AAC *self);
void func_08B49D28(Part49AAC *self);
void func_08B49F08(Part49AAC *self, int id);
void func_08B49F58(Part49AAC *self, int id);
void func_08B49F98(Part49AAC *self, int arg1, int arg2, int arg3);
void func_08B4A054(Part49AAC *self);
void func_08B4A070(Part49AAC *self);
void func_08B4A0F4(Part49AAC *self);

/* ------------------------------------------------------------ D_08B4AE48 */

/* Three tables, each a start pointer and an entry count. */
typedef struct Entry4AC70 {
    unsigned char data[8];
} Entry4AC70;

typedef struct Entry4ACAC {
    unsigned char data[10];
} Entry4ACAC;

typedef struct Entry4ACF0 {
    unsigned char data[0x26];
} Entry4ACF0;

typedef struct Tables4AE48 {
    char pad_00[8];
    int count_08;
    int count_0C;
    int count_10;
    Entry4AC70 *table_14;
    Entry4ACAC *table_18;
    Entry4ACF0 *table_1C;
} Tables4AE48;

extern Tables4AE48 *D_08B4AE48;
int func_08B4AD38(Entry4AC70 *begin, Entry4AC70 *end, unsigned int mask);
int func_08B4AD7C(Entry4ACAC *begin, Entry4ACAC *end, unsigned int mask);
int func_08B4ADC0(Entry4ACF0 *begin, Entry4ACF0 *end, int arg);
void func_08B4AC70(unsigned int arg1, unsigned int arg2);
void func_08B4ACAC(unsigned int arg1, unsigned int arg2);
void func_08B4ACF0(int arg);

/* ------------------------------------------------------------ 0x08B4B4D8 */

void func_08B4B874(void *self);
void func_08B4B9E4(void *self);
void func_08B4B4D8(void *self);

/* Two handles that get the same matrix or value. */
typedef struct Pair4B5A4 {
    void *field_00;
    void *field_04;
} Pair4B5A4;

void func_08B4B5A4(Pair4B5A4 *self, const void *matrix);
void func_08B4B5E4(Pair4B5A4 *self, const void *value);

void func_08B4BC68(void);
void func_08B4BE40(void);
void func_08B4C008(void);
void func_08B4C0E4(void);
void func_08B4C1BC(void);
void func_08B4C1D8(void);
void func_08B4C1F4(void);
void func_08B4C210(void);

Handle4C22C *func_08B4C22C(Handle4C22C *self);
void func_08B4C264(Handle4C22C *self, void *source, int arg1, int arg2);
void func_08B4C444(Handle4C22C *self);
void func_08B4C46C(Handle4C22C *self);
void func_08B4C494(Handle4C22C *self, const void *matrix);
void func_08B4C4C8(Handle4C22C *self, const void *value);

void func_08B4CE8C(void *self);
void func_08B4D13C(void *self);
void func_08B4CCD0(void *self);

typedef struct Part4CD9C {
    char pad_00[8];
    void *field_08;
} Part4CD9C;

void func_08B4CD9C(Part4CD9C *self, const void *matrix);
void func_08B4CDB8(Part4CD9C *self, const void *value);

/* ------------------------------------------------------------ heaps */

/* field_04 is the heap the other objects were allocated from. */
typedef struct Part4DEA4 {
    char pad_00[4];
    void *heap;
    char pad_08[0x14];
    void *field_1C;
    void *field_20;
} Part4DEA4;

typedef struct Part4F6EC {
    char pad_00[4];
    void *heap;
    char pad_08[0x18];
    void *field_20;
    void *field_24;
} Part4F6EC;

typedef struct Part55484 {
    char pad_00[4];
    void *heap;
    char pad_08[4];
    void *field_0C;
    void *field_10;
    char pad_14[4];
    void *field_18;
} Part55484;

/* Three objects from field_28 (two words each), freed with func_08842138. */
typedef struct Slot5C92C {
    void *object;
    int field_04;
} Slot5C92C;

typedef struct Part5C92C {
    char pad_00[4];
    void *heap;
    char pad_08[0x20];
    Slot5C92C field_28[3];
    char pad_40[4];
    void *field_44;
} Part5C92C;
REGION5_AT(Part5C92C, field_28, 0x28);
REGION5_AT(Part5C92C, field_44, 0x44);

void func_08B4DEA4(Part4DEA4 *self);
void func_08B4F6EC(Part4F6EC *self);
void func_08B55484(Part55484 *self);
void func_08B55BD4(Part55484 *self);
void func_08B5C92C(Part5C92C *self);

/* ------------------------------------------------------------ D_089D4DC8 */

typedef struct Global9D4DC8 {
    char pad_0000[0x160];
    unsigned char field_160[0x14];
    unsigned char field_174[0x700C];
    unsigned char field_7180[4];
} Global9D4DC8;
REGION5_AT(Global9D4DC8, field_7180, 0x7180);

extern Global9D4DC8 *D_089D4DC8;
void func_088542AC(Global9D4DC8 *object);
void func_0885631C(void *object);
void func_0885ACE0(void *object);
int func_0885BA0C(void *object, int arg);
int func_0886EB84(void *object);

/* Objects holding handles released with func_0899BB1C. */
typedef struct Part4EE2C {
    char pad_00[0xC];
    void *field_0C;
    void *field_10;
    void *field_14;
} Part4EE2C;

typedef struct Part522B0 {
    char pad_00[0x34];
    void *field_34;
} Part522B0;

typedef struct Part5C9F0 {
    char pad_00[0x2C];
    void *field_2C;
    char pad_30[0x18];
    void *field_48;
} Part5C9F0;

void func_08B4EE2C(Part4EE2C *self);
void func_08B503C8(Part4EE2C *self);
void func_08B522B0(Part522B0 *self);
void func_08B52DB8(Part4EE2C *self);
void func_08B5553C(Part4EE2C *self);
void func_08B5C9F0(Part5C9F0 *self);
void func_08B5D954(Part5C9F0 *self);
void func_08B55A80(void);

void func_08B4F250(void *self);
void func_08B4F3E0(void *self);
void func_08B4F768(void *self);

/* ------------------------------------------------------------ Part54330 */

typedef struct Node1974 {
    char pad_00[4];
    int field_04;
    unsigned char field_08[4];
} Node1974;

/* The object D_08B54330 points to. */
typedef struct Part54330 {
    char pad_0000[0x1054];
    unsigned char field_1054[0x91C];
    void *field_1970;
    Node1974 *field_1974;
} Part54330;
REGION5_AT(Part54330, field_1054, 0x1054);
REGION5_AT(Part54330, field_1970, 0x1970);

/* More of the object D_089D6AC8 points to than auto_region3.h's
 * Global89D6AC8 shows. */
typedef struct Node9D6AC8 {
    char pad_00[4];
    short field_04;
    char pad_06[2];
    unsigned char field_08[4];
} Node9D6AC8;

typedef struct Global89D6AC8View {
    char pad_000[0x710];
    Node9D6AC8 *field_710;
} Global89D6AC8View;

#define GLOBAL_9D6AC8 ((Global89D6AC8View *)D_089D6AC8)

extern Part54330 *D_08B54330;
extern unsigned char D_08B53248[];
extern unsigned char D_08B542B8[];
extern unsigned char D_08B542BC[];
void *func_08934944(int arg0, void *arg1, int arg2, int arg3, void *arg4);
unsigned int func_089346D0(char *buffer, int size, const void *format, void *arg);
void *func_08935C14(const char *text, const void *other);
void func_08B5D630(void *self);
void *func_08B4FC58(void);
void *func_08B50B7C(void);
void *func_08B52348(void);
void *func_08B55934(void);
void *func_08B562F8(void);
void *func_08B5743C(void);
void *func_08B58938(void);
void *func_08B5A608(void);
void *func_08B5CD54(void);
void *func_08B5E60C(void);
void *func_08B5EE98(void);
void func_08B53254(Part54330 *self);
void func_08B53830(void);

void *func_08B51870(void);
void *func_08B5C018(void);
void *func_08B5EE40(void);

/* ------------------------------------------------------------ Part5202C */

typedef struct Pair51F38 {
    void *table;
    int count;
} Pair51F38;

typedef struct Part5202C {
    int field_00;
    int field_04;
    void *field_08;
    Pair51F38 field_0C;
    Pair51F38 field_14;
    Pair51F38 field_1C;
} Part5202C;

extern unsigned char D_08B52900[];
extern unsigned char D_08B529A0[];
extern unsigned char D_08B52AE0[];
extern unsigned char D_08B52BF8[];
void func_08B51F38(Pair51F38 *self, void *table, int count);
Part5202C *func_08B5202C(Part5202C *self);

void *func_08B52F88(int arg1, int arg2);

/* More of the object D_089D4D80 points to: 0x14-byte entries from
 * field_258C. */
typedef struct Entry559E0 {
    unsigned char data[0x14];
} Entry559E0;

typedef struct Global9D4D80View {
    char pad_0000[0x258C];
    Entry559E0 field_258C[1];           /* length unknown */
} Global9D4D80View;

#define GLOBAL_9D4D80 ((Global9D4D80View *)D_089D4D80)

Entry559E0 *func_08B559E0(int index);

/* ------------------------------------------------------------ 0x08B563D0 */

void *func_0881BAA8(void);
int func_0881BAF8(void *object, int id, char arg2, int arg3, char arg4, int arg5, char arg6);
int func_0881BBF4(void *object, int id);
int func_0881BD58(void *object, int id);
int func_0889877C(void *object, int id);
void *func_088985F0(void *object, int id);
void func_08B563D0(void);
void func_08B5648C(int *seed);

void func_08B566C0(void *self);
void func_08B56A9C(void *self);
void func_08B56EC0(void *self);
void func_08B570EC(void *self);

/* ------------------------------------------------------------ Part58A9C */

/* Ten 0xD0-byte entries from field_10, then a handle. */
typedef struct Part58A9C {
    char pad_000[0x10];
    unsigned char entries[10][0xD0];
    void *field_830;
} Part58A9C;
REGION5_AT(Part58A9C, field_830, 0x830);

int func_0899D2F8(void *handle);
void func_0899FDC4(unsigned int arg);
void func_089A0384(unsigned int arg);
void func_089A0C8C(int arg);
void func_089A119C(unsigned int arg);
void func_089A12A4(int arg0, unsigned int arg1, int arg2, unsigned int arg3, unsigned int arg4);
void func_089A250C(void);
void func_089A2568(void);
void func_089A296C(int arg);
void func_08B598AC(void *entry);
void func_08B58A9C(Part58A9C *self);
void func_08B58B0C(Part58A9C *self);

void func_08B5DC80(void *self, void *item, char arg);
void func_08B5E9A4(void);
void func_08B5D980(void);

/* ------------------------------------------------------------ Part61C08 */

typedef struct Part61C08 {
    char pad_00[4];
    int field_04;
    char pad_08[4];
    void *field_0C;
    void *field_10;
    int field_14;
} Part61C08;

void func_08B61B10(Part61C08 *self, int value);
void func_08B61B2C(Part61C08 *self);
void func_08B61C08(Part61C08 *self, int value);
void func_08B61C5C(Part61C08 *self);
void func_08B61C8C(Part61C08 *self);

#ifdef __cplusplus
}
#endif

#endif
