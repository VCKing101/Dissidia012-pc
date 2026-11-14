#ifndef DISSIDIA_MAIN_AUTO_REGION2_H
#define DISSIDIA_MAIN_AUTO_REGION2_H

/* Types and prototypes behind the automatically matched functions in main
 * 0x0881E75C..0x0888AD24 (readability pass, main region 2). Builds on
 * auto_region1.h and follows its naming: classes with a vtable pointer are
 * named after the vtable (Obj9C7638 stores D_089C7638), others after the
 * first function that shows their layout (Obj82F470), objects reached through
 * a global pointer after the global (Sound9D4DC8 is *D_089D4DC8). Fields whose
 * use is not plain keep their offset as their name. Every layout carries
 * compile-time size/offset assertions (MAIN2_SIZE / MAIN2_AT).
 *
 * Note on auto_region1.h: func_089A3890 clears a 4-float vector and
 * func_089A3868 copies one (16 bytes each), not 4x4 matrices. */

#include "main/auto_region1.h"

#define MAIN2_OFFSET(type, field) ((unsigned int)&((type *)0)->field)
#define MAIN2_ASSERT(name, cond) typedef char main2_assert_##name[(cond) ? 1 : -1]
#define MAIN2_SIZE(type, size) MAIN2_ASSERT(size_##type, sizeof(type) == (size))
#define MAIN2_AT(type, field, offset) \
    MAIN2_ASSERT(at_##type##_##field, MAIN2_OFFSET(type, field) == (offset))

/* ------------------------------------------------------------ small shared shapes */

/* Objects whose only visible fields are words their constructor clears. */
typedef struct Words2 {
    int field_00;
    int field_04;
} Words2;
MAIN2_SIZE(Words2, 0x8);
MAIN2_AT(Words2, field_00, 0x0);
MAIN2_AT(Words2, field_04, 0x4);

typedef struct Words3 {
    int field_00;
    int field_04;
    int field_08;
} Words3;
MAIN2_SIZE(Words3, 0xC);
MAIN2_AT(Words3, field_00, 0x0);
MAIN2_AT(Words3, field_04, 0x4);
MAIN2_AT(Words3, field_08, 0x8);

/* A handle from func_0899B9F0, released with func_0899BB1C. */
typedef struct Handle {
    int *handle;
} Handle;
MAIN2_SIZE(Handle, 0x4);
MAIN2_AT(Handle, handle, 0x0);

/* ------------------------------------------------------------ 0x0881E75C..0x08823384 */

extern void *D_089D4910;
void *func_0881E950(void *self, int arg);
void func_0881E75C(int arg);

/* Vtable D_089C7650 at +0x18; reset by func_0881F47C. */
typedef struct Obj9C7650 {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    int field_14;
    void *vtable;
} Obj9C7650;
MAIN2_SIZE(Obj9C7650, 0x1C);
MAIN2_AT(Obj9C7650, field_00, 0x0);
MAIN2_AT(Obj9C7650, field_04, 0x4);
MAIN2_AT(Obj9C7650, field_08, 0x8);
MAIN2_AT(Obj9C7650, field_0C, 0xC);
MAIN2_AT(Obj9C7650, field_10, 0x10);
MAIN2_AT(Obj9C7650, field_14, 0x14);
MAIN2_AT(Obj9C7650, vtable, 0x18);

/* Vtable D_089C7668 at +0x8; reset by func_08820240. */
typedef struct Obj9C7668 {
    int field_00;
    int field_04;
    void *vtable;
} Obj9C7668;
MAIN2_SIZE(Obj9C7668, 0xC);
MAIN2_AT(Obj9C7668, field_00, 0x0);
MAIN2_AT(Obj9C7668, field_04, 0x4);
MAIN2_AT(Obj9C7668, vtable, 0x8);

/* Vtable D_089C7638 at +0x30; two member objects; reset by func_0881F05C. */
typedef struct Obj9C7638 {
    char active;
    char pad_01[0x3];
    int field_04;
    Obj9C7650 field_08;
    Obj9C7668 field_24;
    void *vtable;
} Obj9C7638;
MAIN2_SIZE(Obj9C7638, 0x34);
MAIN2_AT(Obj9C7638, active, 0x0);
MAIN2_AT(Obj9C7638, field_04, 0x4);
MAIN2_AT(Obj9C7638, field_08, 0x8);
MAIN2_AT(Obj9C7638, field_24, 0x24);
MAIN2_AT(Obj9C7638, vtable, 0x30);

extern int D_089C7650, D_089C7668;
Obj9C7638 *func_0881EFA4(Obj9C7638 *self);
void func_0881EFEC(Obj9C7638 *self, unsigned int flags);
void func_0881F05C(Obj9C7638 *self);
void func_0881F068(Obj9C7638 *self);
void func_0881F36C(Obj9C7638 *self);
void func_0881F388(Obj9C7638 *self);
void func_0881F3A4(Obj9C7638 *self);
void func_0881F3C0(Obj9C7638 *self);
Obj9C7650 *func_0881F3DC(Obj9C7650 *self);
void func_0881F424(Obj9C7650 *self, unsigned int flags);
void func_0881F47C(Obj9C7650 *self);
void func_0881F49C(Obj9C7650 *self);
int func_0881F858(Obj9C7650 *self);
int func_0881F8E0(Obj9C7650 *self);
void func_0881F968(void);
void func_0881F9AC(void);
Obj9C7668 *func_088201B0(Obj9C7668 *self);
void func_088201E8(Obj9C7668 *self, unsigned int flags);
void func_08820240(Obj9C7668 *self);
void func_08820250(Obj9C7668 *self);
int func_0882056C(Obj9C7668 *self);
int func_088205FC(Obj9C7668 *self);
void func_0882068C(void);
void func_088206CC(void);

/* An object whose word at +0x08 points at an index into a float table. */
typedef struct Obj81FC0C {
    char pad_00[0x8];
    int *index;
    char pad_0C[0x30];
    float field_3C;
} Obj81FC0C;
MAIN2_AT(Obj81FC0C, index, 0x8);
MAIN2_AT(Obj81FC0C, field_3C, 0x3C);

void func_0881FC0C(Obj81FC0C *self, float *values);

/* What func_08823234 reads through field_24. */
typedef struct Obj823234Data {
    char pad_00[0x2C];
    float field_2C;
} Obj823234Data;
MAIN2_AT(Obj823234Data, field_2C, 0x2C);

typedef struct Obj823234Link {
    char pad_00[0x568];
    Obj823234Data *field_568;
} Obj823234Link;
MAIN2_AT(Obj823234Link, field_568, 0x568);

typedef struct Obj823234 {
    char pad_00[0x20];
    unsigned int flags;
    Obj823234Link *field_24;
} Obj823234;
MAIN2_AT(Obj823234, flags, 0x20);
MAIN2_AT(Obj823234, field_24, 0x24);

float func_08823234(Obj823234 *self);
void func_08823384(Obj823234 *self);

/* ------------------------------------------------------------ 0x08828A84..0x08828E8C */

/* Vtable D_089C78C0 at +0x18; a member pair (vtable D_089C78D0); reset by func_08828C40. */
typedef struct Obj9C78C0 {
    unsigned char active;
    char pad_01[0x3];
    int field_04;
    int field_08;
    VPair field_0C;
    void *vtable;
} Obj9C78C0;
MAIN2_SIZE(Obj9C78C0, 0x1C);
MAIN2_AT(Obj9C78C0, active, 0x0);
MAIN2_AT(Obj9C78C0, field_04, 0x4);
MAIN2_AT(Obj9C78C0, field_08, 0x8);
MAIN2_AT(Obj9C78C0, field_0C, 0xC);
MAIN2_AT(Obj9C78C0, vtable, 0x18);

Words3 *func_08828A84(Words3 *self);
Obj9C78C0 *func_08828B50(Obj9C78C0 *self);
void func_08828C40(Obj9C78C0 *self);
unsigned char func_08828E8C(Obj9C78C0 *self);

/* ------------------------------------------------------------ actors (0x0882F470..0x08838DCC) */

/* The actor as the functions from 0x08833804 on see it (Actor in auto_region1.h
 * is the same object); field_394 is the model/motion object func_0886B344,
 * func_0886CB64 and func_0886B590 work on. */
typedef struct ActorView {
    char pad_00[0x80];
    float position[4];
    char pad_90[0x20];
    float field_B0[4];
    char pad_C0[0x60];
    float field_120[4];
    char pad_130[0x1D0];
    int field_300;
    char pad_304[0x2C];
    float field_330[4];
    char pad_340[0x40];
    unsigned int flags_380;
    unsigned int flags_384;
    char pad_388[0x8];
    struct ActorData390 *field_390;
    void *model;
    char pad_398[0x14];
    struct ActorLinkView *link;
    char pad_3B0[0x4];
    float field_3B4;
    char pad_3B8[0x17C];
    void *field_534;
} ActorView;
MAIN2_AT(ActorView, position, 0x80);
MAIN2_AT(ActorView, field_B0, 0xB0);
MAIN2_AT(ActorView, field_120, 0x120);
MAIN2_AT(ActorView, field_300, 0x300);
MAIN2_AT(ActorView, field_330, 0x330);
MAIN2_AT(ActorView, flags_380, 0x380);
MAIN2_AT(ActorView, flags_384, 0x384);
MAIN2_AT(ActorView, field_390, 0x390);
MAIN2_AT(ActorView, model, 0x394);
MAIN2_AT(ActorView, link, 0x3AC);
MAIN2_AT(ActorView, field_3B4, 0x3B4);
MAIN2_AT(ActorView, field_534, 0x534);

/* A component with two 4-float vectors, its actor at +0x30 and a sorted table at +0x48
 * (the functions at 0x0882F470..0x0882FE00). */
typedef struct Entry82F664 {
    short key;
    unsigned short id;
    int field_04;
} Entry82F664;
MAIN2_SIZE(Entry82F664, 0x8);
MAIN2_AT(Entry82F664, key, 0x0);
MAIN2_AT(Entry82F664, id, 0x2);
MAIN2_AT(Entry82F664, field_04, 0x4);

typedef struct Table82F664 {
    int field_00;
    int count;
    Entry82F664 entries[1];
} Table82F664;
MAIN2_AT(Table82F664, field_00, 0x0);
MAIN2_AT(Table82F664, count, 0x4);
MAIN2_AT(Table82F664, entries, 0x8);

typedef struct Pair82FE00 {
    short field_00;
    short field_02;
} Pair82FE00;
MAIN2_SIZE(Pair82FE00, 0x4);
MAIN2_AT(Pair82FE00, field_00, 0x0);
MAIN2_AT(Pair82FE00, field_02, 0x2);

typedef struct Obj82F470 {
    float field_00[4];
    char pad_10[0x10];
    float field_20[4];
    ActorView *actor;
    char pad_34[0x14];
    Table82F664 *table;
    char pad_4C[0x4];
    short field_50;
    char pad_52[0x6];
    unsigned int flags;
    char pad_5C[0x4];
    char field_60[0x10];
    char pad_70[0x20];
    int field_90;
    char pad_94[0x4];
    int field_98;
    int field_9C;
    char pad_A0[0xC0];
    Pair82FE00 field_160[4];
} Obj82F470;
MAIN2_AT(Obj82F470, field_00, 0x0);
MAIN2_AT(Obj82F470, field_20, 0x20);
MAIN2_AT(Obj82F470, actor, 0x30);
MAIN2_AT(Obj82F470, table, 0x48);
MAIN2_AT(Obj82F470, field_50, 0x50);
MAIN2_AT(Obj82F470, flags, 0x58);
MAIN2_AT(Obj82F470, field_60, 0x60);
MAIN2_AT(Obj82F470, field_90, 0x90);
MAIN2_AT(Obj82F470, field_98, 0x98);
MAIN2_AT(Obj82F470, field_9C, 0x9C);
MAIN2_AT(Obj82F470, field_160, 0x160);

/* A class whose member at +0x34 is the 0x18-byte part func_0886F028 builds. */
typedef struct Obj82F614 {
    char pad_00[0x34];
    char field_34[0x18];
} Obj82F614;
MAIN2_AT(Obj82F614, field_34, 0x34);

void func_0882F470(Obj82F470 *self);
void func_0882F4E4(Obj82F470 *self);
void func_0882F504(Obj82F470 *self);
void func_0882F614(Obj82F614 *self, unsigned int flags);
unsigned int func_0882F664(Obj82F470 *self, int index);
unsigned char func_0882F6D0(Obj82F470 *self, int key);
void func_0882F720(Obj82F470 *self, int value);
void func_0882F774(Obj82F470 *self);
void func_0882FBF0(Obj82F470 *self);
void func_0882FBFC(Obj82F470 *self);
void func_0882FCE8(Obj82F470 *self);
void func_0882FE00(Obj82F470 *self);
int func_0882FE28(Obj82F470 *self, int index);
int func_0882EF04(const void *a, const void *b);
float func_0886B344(void *model);
float func_0886B3D4(void *model);
unsigned char func_0886B464(void *model);

typedef struct Obj830938 {
    char pad_00[0x10];
    float field_10[4];
} Obj830938;
MAIN2_AT(Obj830938, field_10, 0x10);

void func_08830938(Obj830938 *self);
extern char D_09AFE090[];
void func_08830C74(const void *src, unsigned int size);
extern int D_089C7C48;
void func_08831010(Obj9C6F88 *self, unsigned int flags);
void func_088BF17C(void *self, unsigned int flags);
void func_0883106C(void *self, unsigned int flags);
void *func_088312D0(void *self, unsigned int size);
void func_088312EC(void *self, void **slot);
void *func_08831320(unsigned int size);
void func_0883241C(void *self, int arg, int mode);
void func_08832544(void *self, int arg);

typedef struct Obj832D64 {
    char pad_00[0x28];
    unsigned int flags;
} Obj832D64;
MAIN2_AT(Obj832D64, flags, 0x28);

int func_08A51750(void *self, int arg, int mode);
void func_08832D64(void *self, Obj832D64 *target, int arg);
void func_08833804(ActorView *self);

/* What an actor keeps at +0x390. */
typedef struct ActorData390 {
    char pad_00[0x18];
    void *field_18;
    char pad_1C[0xB0];
    int field_CC;
} ActorData390;
MAIN2_AT(ActorData390, field_18, 0x18);
MAIN2_AT(ActorData390, field_CC, 0xCC);

/* The actor link at Actor +0x3AC (ActorLink in auto_region1.h); its bit sets at +0x28
 * are destroyed by func_0880ABF4. */
typedef struct ActorLinkView {
    int field_00;
    char pad_04[0x24];
    ActorBits bits;
} ActorLinkView;
MAIN2_AT(ActorLinkView, field_00, 0x0);
MAIN2_AT(ActorLinkView, bits, 0x28);

/* An entry of ModelData's table (0x60 bytes). */
typedef struct ModelPart {
    char pad_00[0x50];
    short field_50;
    char pad_52[0xE];
} ModelPart;
MAIN2_SIZE(ModelPart, 0x60);
MAIN2_AT(ModelPart, field_50, 0x50);

/* What a model keeps at +0x84: its part table and the handle func_089B0E78 takes. */
typedef struct ModelData {
    ModelPart *parts;
    char pad_04[0x4];
    int handle;
} ModelData;
MAIN2_AT(ModelData, parts, 0x0);
MAIN2_AT(ModelData, handle, 0x8);

typedef void (*ModelCallback)(int arg, int handle);

/* The model/motion object an actor keeps at +0x394 (functions at 0x0886A24C..0x0886CD5C). */
typedef struct ModelObj {
    char pad_00[0xC];
    int handle_0C;
    char pad_10[0x74];
    ModelData *data;
    char pad_88[0x8];
    unsigned int flags;
    char pad_94[0x98];
    ModelCallback callback_12C;
    int arg_130;
    ModelCallback callback_134;
    int arg_138;
} ModelObj;
MAIN2_AT(ModelObj, handle_0C, 0xC);
MAIN2_AT(ModelObj, data, 0x84);
MAIN2_AT(ModelObj, flags, 0x90);
MAIN2_AT(ModelObj, callback_12C, 0x12C);
MAIN2_AT(ModelObj, arg_130, 0x130);
MAIN2_AT(ModelObj, callback_134, 0x134);
MAIN2_AT(ModelObj, arg_138, 0x138);

int func_0883E108(ActorLinkView *link, int arg1, int arg2, int arg3);
int func_08833B2C(ActorView *self, int arg1, int arg2, int arg3);
void func_08834D60(ActorView *self, float value);
unsigned int func_08811F44(void *object, unsigned int arg1, int arg2);
int func_08835134(ActorView *self, unsigned int arg1, int arg2);
void *func_088350AC(ActorView *self, int kind, void *prev);
int func_08835284(ActorView *self, int kind);
void *func_088352E8(ActorView *self, int kind, int n);
void func_08835174(ActorView *self, int arg1, unsigned int arg2);
void func_08835358(ActorView *self);
void func_08835C08(ActorView *self, const float *value);
void func_08836580(ActorView *self, const float *value);
void func_08836DA4(ActorView *self, int value);
void func_0886CB64(ModelObj *model, unsigned int arg1, unsigned char on);
void func_0886CC8C(ModelObj *model, unsigned int arg1, unsigned char on);
void func_08837290(ActorView *self, unsigned int arg1, unsigned char on);
void func_088372B0(ActorView *self, int arg1);
void func_088372D0(ActorView *self, int arg1);
void func_088372F0(ActorView *self, unsigned int arg1, unsigned char on);
unsigned char func_088377C0(ActorView *self);
unsigned char func_088378E8(ActorView *self);
unsigned char func_088378F8(ActorView *self);
void func_08837938(ActorView *self);
void func_08837AA4(ActorView *self, ActorView *other);
int func_0883E254(ActorLinkView *link);
void func_08838718(ActorView *self);
void func_088394B8(void *object, int arg1, int arg2, int arg3);
void func_08838948(ActorView *self, int arg1, int arg2, int arg3);
void *func_0886B590(ModelObj *self, int part);
void func_08838DCC(ActorView *self, float *out, int part);

/* ------------------------------------------------------------ 0x0883DFA8..0x0883F3B8 */

/* Built by func_0883F014 over the base built by func_088BF108 (destroyed by func_088BF17C). */
typedef struct Obj83F014 {
    char pad_00[0x5E4];
    int field_5E4;
    int field_5E8;
    int field_5EC;
    char pad_5F0[0x40];
    int field_630;
} Obj83F014;
MAIN2_AT(Obj83F014, field_5E4, 0x5E4);
MAIN2_AT(Obj83F014, field_5E8, 0x5E8);
MAIN2_AT(Obj83F014, field_5EC, 0x5EC);
MAIN2_AT(Obj83F014, field_630, 0x630);

/* Reset by func_0883ECC4 (also Entry9D4960's member at +0x34). */
typedef struct Obj83ECC4 {
    char active;
    char pad_01[0x1F];
    int field_20;
    int field_24;
    void *field_28;
} Obj83ECC4;
MAIN2_SIZE(Obj83ECC4, 0x2C);
MAIN2_AT(Obj83ECC4, active, 0x0);
MAIN2_AT(Obj83ECC4, field_20, 0x20);
MAIN2_AT(Obj83ECC4, field_24, 0x24);
MAIN2_AT(Obj83ECC4, field_28, 0x28);

extern int D_09AFE7C0;            /* static object built by func_08882950 */
extern Obj83F014 D_09AFE820;      /* static object built by func_0883F014 */
extern int D_089D4948;            /* their destructor entry */
void *func_08882950(void *self);
Obj83F014 *func_0883F014(Obj83F014 *self);
void func_0883DFA8(void);
void func_0883E1F0(ActorLinkView *self);
void func_0880ABF4(ActorBits *bits, unsigned int flags);
void func_0883E0AC(ActorLinkView *self, unsigned int flags);
void func_0883ECC4(Obj83ECC4 *self);
Obj83ECC4 *func_0883EAE8(Obj83ECC4 *self);
unsigned char func_088A9BF8(void *object);
int func_0883EC9C(Obj83ECC4 *self);
void *func_088BF108(void *self);
void func_0883F3DC(void *self, int arg, char flag);
void func_0883F3B8(void *self, int arg);

/* ------------------------------------------------------------ the world (0x088417C8..0x08842308) */

/* The world as func_08841C4C sets it up (World9D4958 in auto_region1.h lacks the words
 * at +0x10, +0x18 and +0x3C..+0x48). */
typedef struct WorldView {
    void *field_00;
    char pad_04[0x4];
    void *heap;
    struct Actor *actors;
    int field_10;
    struct Fighter14 *first_14;
    int field_18;
    struct Chara *charas;
    char pad_20[0x4];
    struct Obj24 *first_24;
    char pad_28[0xC];
    struct Obj34 *first_34;
    char pad_38[0x4];
    int field_3C;
    int field_40;
    int field_44;
    int field_48;
    int field_4C;
} WorldView;
MAIN2_SIZE(WorldView, 0x50);
MAIN2_AT(WorldView, field_00, 0x0);
MAIN2_AT(WorldView, heap, 0x8);
MAIN2_AT(WorldView, actors, 0xC);
MAIN2_AT(WorldView, field_10, 0x10);
MAIN2_AT(WorldView, first_14, 0x14);
MAIN2_AT(WorldView, field_18, 0x18);
MAIN2_AT(WorldView, charas, 0x1C);
MAIN2_AT(WorldView, first_24, 0x24);
MAIN2_AT(WorldView, first_34, 0x34);
MAIN2_AT(WorldView, field_3C, 0x3C);
MAIN2_AT(WorldView, field_40, 0x40);
MAIN2_AT(WorldView, field_44, 0x44);
MAIN2_AT(WorldView, field_48, 0x48);
MAIN2_AT(WorldView, field_4C, 0x4C);

void func_08882704(void *object);
void func_088417C8(void);
void func_088417F8(void *self);
void func_08841950(void *self);
void func_08841144(void);
void func_08841218(void);
void func_088412B0(void);
void func_088412E4(void);
void func_0884197C(void);
void func_08841998(void);
void func_088419B4(void);
void func_088419D0(void);
void func_0881160C(void);
void func_08808364(void);
void func_0884E628(void);
void func_088083A8(void);
void func_0884E650(void);
void func_08841C4C(WorldView *self, void *heap);
void func_08841C90(WorldView *self);
void func_08841EB4(void *heap);
void func_08841ED8(void);
void func_0885C538(void *heap, void *block, int arg);
void func_0885C488(void *heap);
void func_088421EC(void);
void func_08842210(void);
void *func_08841FF4(void *prev);
void func_08842284(void *item);
void func_08842240(void);
extern int D_09AF84D0;            /* random seed */
extern int D_09AF8B00;            /* last random value (0..0x7FFF) */
void func_08842308(void);

/* One 0xC-byte element of Obj84236C (built by D_08842354). */
typedef struct Elem842354 {
    int field_00;
    int field_04;
    int field_08;
} Elem842354;
MAIN2_SIZE(Elem842354, 0xC);
MAIN2_AT(Elem842354, field_00, 0x0);
MAIN2_AT(Elem842354, field_04, 0x4);
MAIN2_AT(Elem842354, field_08, 0x8);

/* Three elements and a flag; the size is the room Entry9D4960 gives it (+0x60..+0xD4). */
typedef struct Obj84236C {
    Elem842354 elems[3];
    char field_24;
    char pad_25[0x4F];
} Obj84236C;
MAIN2_SIZE(Obj84236C, 0x74);
MAIN2_AT(Obj84236C, elems, 0x0);
MAIN2_AT(Obj84236C, field_24, 0x24);

extern int D_08842354;            /* element constructor */
Obj84236C *func_0884236C(Obj84236C *self);

/* ------------------------------------------------------------ tables (0x08842450..0x08847130) */

/* A 0x44-byte record kept sorted by id in Records9D4968's table. */
typedef struct Record9D4968 {
    short id;
    char pad_02[0x32];
    char field_34[0x10];
} Record9D4968;
MAIN2_SIZE(Record9D4968, 0x44);
MAIN2_AT(Record9D4968, id, 0x0);
MAIN2_AT(Record9D4968, field_34, 0x34);

typedef struct RecordTable9D4968 {
    int field_00;
    int count;
    Record9D4968 records[1];
} RecordTable9D4968;
MAIN2_AT(RecordTable9D4968, field_00, 0x0);
MAIN2_AT(RecordTable9D4968, count, 0x4);
MAIN2_AT(RecordTable9D4968, records, 0x8);

/* The record bank D_089D4968 points at. */
typedef struct Records9D4968 {
    RecordTable9D4968 *table;
} Records9D4968;
MAIN2_AT(Records9D4968, table, 0x0);

extern Records9D4968 *D_089D4968;
void func_088440F8(Table9D4960 *self);
void func_08846C38(Records9D4968 *self);

/* An entry of the table D_089D4960 (Table9D4960 in auto_region1.h): use count at +0xE0,
 * reference count at +0xE2, next entry at +0xE8. */
struct Entry9D4960;
typedef struct Entry9D4960 {
    int field_00;
    char pad_04[0x8];
    int field_0C;
    char pad_10[0x24];
    Obj83ECC4 field_34;
    Obj84236C field_60;
    char field_D4[0xC];
    short field_E0;
    short refs;
    char pad_E4[0x4];
    struct Entry9D4960 *next;
} Entry9D4960;
MAIN2_AT(Entry9D4960, field_00, 0x0);
MAIN2_AT(Entry9D4960, field_0C, 0xC);
MAIN2_AT(Entry9D4960, field_34, 0x34);
MAIN2_AT(Entry9D4960, field_60, 0x60);
MAIN2_AT(Entry9D4960, field_D4, 0xD4);
MAIN2_AT(Entry9D4960, field_E0, 0xE0);
MAIN2_AT(Entry9D4960, refs, 0xE2);
MAIN2_AT(Entry9D4960, next, 0xE8);

void func_088427F4(Obj84236C *self, Elem842354 *elem);
void func_08842450(Obj84236C *self);
void func_08842938(void *self, float *v);
unsigned char func_08842F1C(int value, int limit);
unsigned char func_08842F48(int value, int total, int percent);

/* Holds the record func_088470B8 returned. */
typedef struct Obj8430B8 {
    Record9D4968 *record;
} Obj8430B8;
MAIN2_AT(Obj8430B8, record, 0x0);

extern int D_089C8850;
Record9D4968 *func_088470B8(int id);
char *func_08844FA4(char *dst, void *src);
int func_0884333C(Obj8430B8 *self, int arg1, int arg2);
int func_088459E0(Obj8430B8 *self, int arg1, int arg2);
int func_08843460(Obj8430B8 *self, int arg1, int arg2);
void func_088430B8(Obj8430B8 *self, int id, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8);

/* A block at field_10 whose objects start 8 bytes in; field_14 points at them. */
typedef struct Obj8434C8 {
    char pad_00[0x10];
    char *field_10;
    void *field_14;
} Obj8434C8;
MAIN2_AT(Obj8434C8, field_10, 0x10);
MAIN2_AT(Obj8434C8, field_14, 0x14);

extern int D_08842F70;            /* element constructor */
void func_088434C8(Obj8434C8 *self);
void func_08843F4C(Entry9D4960 *self);
void func_08843F88(Entry9D4960 *self);
void func_08843FBC(Entry9D4960 *self);
void func_08843FF8(Entry9D4960 *self);
void func_0883EBE4(Obj83ECC4 *self);
void func_0883EC44(Obj83ECC4 *self);
void func_088424C4(Obj84236C *self, int arg);
void func_08842650(Obj84236C *self);
void func_088A39D4(void *part);
void func_088A3A10(void *part);
int func_08844030(Entry9D4960 *self);
void func_0884403C(Entry9D4960 *self);
void func_0884404C(Entry9D4960 *self);
unsigned char func_0884405C(Entry9D4960 *self);
void func_0884415C(Table9D4960 *self, void *block);
Entry9D4960 *func_088444D4(Table9D4960 *self, int id);
void func_0884318C(Entry9D4960 *entry, int arg1, int arg2, int arg3);
void func_08844250(Table9D4960 *self, int id, int arg1, int arg2, int arg3);
void func_08846628(Entry9D4960 *entry);
void func_08844A60(Table9D4960 *self);
void func_08844AB4(Table9D4960 *self);
void func_08846A3C(Table9D4960 *self);
void func_08844F84(void);
Records9D4968 *func_08846C18(Records9D4968 *self);
int func_08846C0C(const void *a, const void *b);
void func_08847068(Records9D4968 *self, Record9D4968 *record);
Record9D4968 *func_08846C98(Records9D4968 *self, int id);
int func_08846CCC(Records9D4968 *self, unsigned int arg1, unsigned int arg2, unsigned char arg3);
int func_08846E18(Records9D4968 *self, int arg1, int arg2);
int func_08846F40(Records9D4968 *self, int arg1, int arg2);
void func_088470DC(unsigned int arg1, unsigned int arg2, unsigned char arg3);
void func_08847108(int arg1, int arg2);
void func_08847130(int arg1, int arg2);

/* ------------------------------------------------------------ 0x088473C0..0x0884F140 */

void func_0884722C(void *self);
void *func_088473C0(void *self);

typedef struct Obj847804 {
    char pad_00[0x30];
    int field_30;
} Obj847804;
MAIN2_AT(Obj847804, field_30, 0x30);

void *func_088C5E44(void *self);
Obj847804 *func_08847804(Obj847804 *self);

/* An object that keeps an actor at +0x04 (functions at 0x08847908..0x08847CA4). */
typedef struct Obj847908 {
    char pad_00[0x4];
    ActorView *actor;
    void *field_08;
} Obj847908;
MAIN2_AT(Obj847908, actor, 0x4);
MAIN2_AT(Obj847908, field_08, 0x8);

char *func_08835798(ActorView *actor);
int func_088374B0(ActorView *actor);
unsigned char func_08847B0C(void *object);
unsigned char func_08847908(Obj847908 *self);
void func_08847948(Obj847908 *self);
void func_08847C34(Obj847908 *self);
void func_08847CA4(Obj847908 *self);
Holder *func_0884806C(Holder *self);
void func_08849B50(void *object);
void func_08849700(void *object);
void func_0884A294(void);
void func_089A0384(unsigned int state);
void func_089A03DC(unsigned int state);
void func_0884B908(void *part, unsigned char on);
void func_0884BE74(unsigned int arg, unsigned char on);

/* Two handles from func_0899B9F0. */
typedef struct Slot84C4DC {
    int *field_00;
    int *field_04;
    char pad_08[0xC];
} Slot84C4DC;
MAIN2_SIZE(Slot84C4DC, 0x14);
MAIN2_AT(Slot84C4DC, field_00, 0x0);
MAIN2_AT(Slot84C4DC, field_04, 0x4);

typedef struct Obj84C4DC {
    char pad_00[0x2590];
    Slot84C4DC slots[1];
    char pad_25A4[0x268];
    char *buffer;
    char *data;
} Obj84C4DC;
MAIN2_AT(Obj84C4DC, slots, 0x2590);
MAIN2_AT(Obj84C4DC, buffer, 0x280C);
MAIN2_AT(Obj84C4DC, data, 0x2810);

int *func_0899B9F0(int arg);
int func_0899BB68(int *handle, unsigned int *arg1, int arg2, int arg3);
void func_0899BB1C(int *handle);
void func_0884C4DC(Obj84C4DC *self, int index, unsigned int *arg1, int arg2, unsigned int *arg3, int arg4);
void *func_088820D8(unsigned int size);
void func_0884C588(Obj84C4DC *self, const void *src, unsigned int size);

typedef struct Obj84E53C {
    char pad_00[0x4];
    unsigned short flags;
} Obj84E53C;
MAIN2_AT(Obj84E53C, flags, 0x4);

unsigned short func_0884E53C(Obj84E53C *self);
extern int D_089D4D98;
Holder *func_0884E578(Holder *self);
extern char D_09B166F0[];
void func_0884E658(void);

/* The object D_089D69E0 points at. */
typedef struct Obj9D69E0 {
    char pad_00[0x10];
    int field_10;
    char pad_14[0x4];
    float field_18;
    char pad_1C[0x8];
    char field_24;
    char field_25;
} Obj9D69E0;
MAIN2_AT(Obj9D69E0, field_10, 0x10);
MAIN2_AT(Obj9D69E0, field_18, 0x18);
MAIN2_AT(Obj9D69E0, field_24, 0x24);
MAIN2_AT(Obj9D69E0, field_25, 0x25);

extern Obj9D69E0 *D_089D69E0;

typedef struct Obj84E9B0 {
    char pad_00[0x10];
    float field_10;
} Obj84E9B0;
MAIN2_AT(Obj84E9B0, field_10, 0x10);

void func_0884EC0C(Obj84E9B0 *self);
void func_0884E9B0(Obj84E9B0 *self);

/* One 0x20-byte element of Obj84F140 (built by func_0884ECAC). */
typedef struct Elem84ECAC {
    char pad_00[0x4];
    int field_04;
    int field_08;
    int field_0C;
    short id;
    char pad_12[0x6];
    char field_18;
    char pad_19[0x7];
} Elem84ECAC;
MAIN2_SIZE(Elem84ECAC, 0x20);
MAIN2_AT(Elem84ECAC, field_04, 0x4);
MAIN2_AT(Elem84ECAC, field_08, 0x8);
MAIN2_AT(Elem84ECAC, field_0C, 0xC);
MAIN2_AT(Elem84ECAC, id, 0x10);
MAIN2_AT(Elem84ECAC, field_18, 0x18);

typedef struct Obj84F140 {
    Elem84ECAC elems[16];
    int field_200;
    char field_204;
    char field_205;
    char field_206;
    char field_207;
    char field_208;
} Obj84F140;
MAIN2_AT(Obj84F140, elems, 0x0);
MAIN2_AT(Obj84F140, field_200, 0x200);
MAIN2_AT(Obj84F140, field_204, 0x204);
MAIN2_AT(Obj84F140, field_205, 0x205);
MAIN2_AT(Obj84F140, field_206, 0x206);
MAIN2_AT(Obj84F140, field_207, 0x207);
MAIN2_AT(Obj84F140, field_208, 0x208);

void func_0884EC9C(Elem84ECAC *self);
void func_0884E9EC(void *owner, Elem84ECAC *elem);
void func_0884ECD4(Elem84ECAC *self);
Elem84ECAC *func_0884ECAC(Elem84ECAC *self);
unsigned char func_0884EDC0(Elem84ECAC *self, int id);
void func_0884F12C(Elem84ECAC *self);
Obj84F140 *func_0884F140(Obj84F140 *self, int value);

/* ------------------------------------------------------------ 0x0885087C..0x08851C18 */

/* The object D_089D4DA8 points at (constructor func_0885087C). */
typedef struct Obj9D4DA8 {
    void *field_00;
} Obj9D4DA8;
MAIN2_SIZE(Obj9D4DA8, 0x4);
MAIN2_AT(Obj9D4DA8, field_00, 0x0);

extern Obj9D4DA8 *D_089D4DA8;
Obj9D4DA8 *func_0885087C(Obj9D4DA8 *self);
void func_0885089C(Obj9D4DA8 *self, int arg);
unsigned int func_088508D0(Obj9D4DA8 *self, int arg1, unsigned char arg2);
int func_088509AC(Obj9D4DA8 *self, int arg1, int arg2);
void func_08850A40(int arg);
void func_08850A64(unsigned int arg1, unsigned char arg2);
void func_08850A90(int arg1, int arg2);
void func_08850BC4(void);

/* Keeps a copy of a record (Record9D4968) at +0x590. */
typedef struct Obj850FF4 {
    char pad_00[0x590];
    Record9D4968 record;
    char pad_5D4[0x3B];
    char field_60F;
    char pad_610[0x1];
    char field_611;
} Obj850FF4;
MAIN2_AT(Obj850FF4, record, 0x590);
MAIN2_AT(Obj850FF4, field_60F, 0x60F);
MAIN2_AT(Obj850FF4, field_611, 0x611);

void func_08850FF4(Obj850FF4 *self, const Record9D4968 *record);
void func_0885106C(Obj850FF4 *self);
Words2 *func_08851C18(Words2 *self);

/* ------------------------------------------------------------ the sound manager *D_089D4DC8 (0x08851C28..0x0885B518) */

/* Member at +0x0000 of Sound9D4DC8. */
typedef struct Part853F10 {
    char pad_00[0x150];
    int field_150;
    int field_154;
    char pad_158[0x8];
} Part853F10;
MAIN2_SIZE(Part853F10, 0x160);
MAIN2_AT(Part853F10, field_150, 0x150);
MAIN2_AT(Part853F10, field_154, 0x154);

/* A handle, a block from heap and two words (members at +0x160 and +0x174). */
typedef struct HandleSlot {
    int *handle;
    void *block;
    int field_08;
    int field_0C;
    void *heap;
} HandleSlot;
MAIN2_SIZE(HandleSlot, 0x14);
MAIN2_AT(HandleSlot, handle, 0x0);
MAIN2_AT(HandleSlot, block, 0x4);
MAIN2_AT(HandleSlot, field_08, 0x8);
MAIN2_AT(HandleSlot, field_0C, 0xC);
MAIN2_AT(HandleSlot, heap, 0x10);

/* Member at +0x0190. */
typedef struct Part85874C {
    char pad_00[0x330];
    int field_330;
    int field_334;
    char pad_338[0x18];
} Part85874C;
MAIN2_SIZE(Part85874C, 0x350);
MAIN2_AT(Part85874C, field_330, 0x330);
MAIN2_AT(Part85874C, field_334, 0x334);

/* One 0x40-byte entry of Part859060. */
typedef struct Entry859060 {
    char pad_00[0x8];
    char active;
    char pad_09[0x37];
} Entry859060;
MAIN2_SIZE(Entry859060, 0x40);
MAIN2_AT(Entry859060, active, 0x8);

/* Member at +0x0500: five handles and ten entries. */
typedef struct Part859060 {
    int *handles[5];
    char pad_14[0xC];
    Entry859060 entries[10];
    int field_2A0;
    int field_2A4;
    char field_2A8;
    char pad_2A9[0x7];
} Part859060;
MAIN2_SIZE(Part859060, 0x2B0);
MAIN2_AT(Part859060, handles, 0x0);
MAIN2_AT(Part859060, entries, 0x20);
MAIN2_AT(Part859060, field_2A0, 0x2A0);
MAIN2_AT(Part859060, field_2A4, 0x2A4);
MAIN2_AT(Part859060, field_2A8, 0x2A8);

/* Member at +0x07B0. */
typedef struct Part85B518 {
    char pad_00[0x4B10];
    int field_4B10;
    int field_4B14;
    char pad_4B18[0x8];
} Part85B518;
MAIN2_SIZE(Part85B518, 0x4B20);
MAIN2_AT(Part85B518, field_4B10, 0x4B10);
MAIN2_AT(Part85B518, field_4B14, 0x4B14);

/* One 0x60-byte entry of Part8577D8. */
typedef struct Entry8577D8 {
    char active;
    char pad_01[0x5F];
} Entry8577D8;
MAIN2_SIZE(Entry8577D8, 0x60);
MAIN2_AT(Entry8577D8, active, 0x0);

/* Member at +0x52D0: forty entries. */
typedef struct Part8577D8 {
    char pad_00[0x20];
    Entry8577D8 entries[40];
    char pad_F20[0x10];
} Part8577D8;
MAIN2_SIZE(Part8577D8, 0xF30);
MAIN2_AT(Part8577D8, entries, 0x20);

/* Member at +0x6200. */
typedef struct Part859D98 {
    char pad_00[0x3D0];
    int field_3D0;
    int field_3D4;
    char pad_3D8[0x8];
} Part859D98;
MAIN2_SIZE(Part859D98, 0x3E0);
MAIN2_AT(Part859D98, field_3D0, 0x3D0);
MAIN2_AT(Part859D98, field_3D4, 0x3D4);

/* Member at +0x65E0. */
typedef struct Part851C28 {
    char pad_00[0x10];
    char field_10[0x3C0];
    int field_3D0;
    int field_3D4;
    char pad_3D8[0x8];
} Part851C28;
MAIN2_SIZE(Part851C28, 0x3E0);
MAIN2_AT(Part851C28, field_10, 0x10);
MAIN2_AT(Part851C28, field_3D0, 0x3D0);
MAIN2_AT(Part851C28, field_3D4, 0x3D4);

/* Member at +0x69C0: fourteen word pairs, a flag and the handle func_088D107C returns. */
typedef struct Part85697C {
    Words2 pairs[14];
    char field_70;
    char pad_71[0xC3];
    int field_134;
    int *field_138;
    char pad_13C[0x4];
} Part85697C;
MAIN2_SIZE(Part85697C, 0x140);
MAIN2_AT(Part85697C, pairs, 0x0);
MAIN2_AT(Part85697C, field_70, 0x70);
MAIN2_AT(Part85697C, field_134, 0x134);
MAIN2_AT(Part85697C, field_138, 0x138);

/* The sound manager D_089D4DC8 points at; func_0885A31C, func_0885A398, func_0885A41C and
 * func_0885A4FC run one step over every member. */
typedef struct Sound9D4DC8 {
    Part853F10 field_0000;
    HandleSlot field_0160;
    HandleSlot field_0174;
    char field_0188[0x8];
    Part85874C field_0190;
    char field_04E0[0x20];
    Part859060 field_0500;
    Part85B518 field_07B0;
    Part8577D8 field_52D0;
    Part859D98 field_6200;
    Part851C28 field_65E0;
    Part85697C field_69C0;
    char field_6B00[0x4];
} Sound9D4DC8;
MAIN2_AT(Sound9D4DC8, field_0000, 0x0);
MAIN2_AT(Sound9D4DC8, field_0160, 0x160);
MAIN2_AT(Sound9D4DC8, field_0174, 0x174);
MAIN2_AT(Sound9D4DC8, field_0188, 0x188);
MAIN2_AT(Sound9D4DC8, field_0190, 0x190);
MAIN2_AT(Sound9D4DC8, field_04E0, 0x4E0);
MAIN2_AT(Sound9D4DC8, field_0500, 0x500);
MAIN2_AT(Sound9D4DC8, field_07B0, 0x7B0);
MAIN2_AT(Sound9D4DC8, field_52D0, 0x52D0);
MAIN2_AT(Sound9D4DC8, field_6200, 0x6200);
MAIN2_AT(Sound9D4DC8, field_65E0, 0x65E0);
MAIN2_AT(Sound9D4DC8, field_69C0, 0x69C0);
MAIN2_AT(Sound9D4DC8, field_6B00, 0x6B00);

extern Sound9D4DC8 *D_089D4DC8;
void func_08851C28(Part851C28 *self);
void func_08851C60(Part851C28 *self);
void func_08851C6C(Part851C28 *self);
void func_08851D30(Part851C28 *self);
void func_08851E8C(Part851C28 *self);
void func_088525A0(void *self);
void func_088525A8(void *self, int arg);
void func_08852AAC(void *self);
void func_08853274(void *self);
void *func_08853D2C(void *self);
void func_08853D34(void *self);
Handle *func_08853ED8(Handle *self);
void func_08853F10(Part853F10 *self);
void func_08853F1C(Part853F10 *self);
void func_08853F28(Part853F10 *self);
void func_08854008(Part853F10 *self);
void func_08854254(Handle *self, unsigned int *arg1, int arg2);
void func_088542AC(Handle *self);
Words3 *func_0885493C(Words3 *self);
void func_088549CC(void *self);
void func_08854A18(void *self);
void func_08854FAC(void *self);
void func_08855438(void *self);
HandleSlot *func_08856160(HandleSlot *self);
void func_08856170(HandleSlot *self);
void func_0885617C(HandleSlot *self);
void func_08856188(HandleSlot *self);
void func_08856194(HandleSlot *self);
void func_0885631C(HandleSlot *self);
void func_08856568(HandleSlot *self);
Part85697C *func_0885697C(Part85697C *self);
extern void *D_089D7830;
int *func_088D107C(void *self, void *callback, int arg);
void func_08857108(int arg1, short arg2);
void func_088569A4(Part85697C *self);
void func_088569EC(Part85697C *self);
void func_08856A58(Part85697C *self);
void func_08856B94(Part85697C *self);
void func_08856F7C(void *self, ActorView *actor, int value, int arg3, unsigned int arg4);
void func_08856F54(void *self, ActorView *actor, int arg3, unsigned int arg4);

/* Four words and a fifth, cleared by func_088577B0. */
typedef struct Obj8577B0 {
    int field_00[4];
    int field_10;
} Obj8577B0;
MAIN2_SIZE(Obj8577B0, 0x14);
MAIN2_AT(Obj8577B0, field_00, 0x0);
MAIN2_AT(Obj8577B0, field_10, 0x10);

Obj8577B0 *func_088577B0(Obj8577B0 *self);
void func_088577D8(Part8577D8 *self);
void func_088577FC(Part8577D8 *self);
void func_08857804(Part8577D8 *self);
void func_08857A3C(Part8577D8 *self);
void func_08857D24(Part8577D8 *self);
Handle *func_0885865C(Handle *self);
void func_088586AC(Handle *self, unsigned int *arg1, int arg2);
void func_08858704(Part85874C *self);
void func_0885874C(Part85874C *self);
void func_08858758(Part85874C *self);
void func_0885883C(Part85874C *self);
Part859060 *func_08859060(Part859060 *self);
void func_08859084(Part859060 *self);
void func_088590B4(Part859060 *self);
void func_0885911C(Part859060 *self, int index, unsigned int *arg1, int arg2);
void func_08859188(Part859060 *self);
void func_08859194(Part859060 *self);
void func_088595B4(Part859060 *self);
Words2 *func_08859D5C(Words2 *self);
void func_08859D98(Part859D98 *self);
void func_08859E54(Part859D98 *self);
void func_08859F14(Part859D98 *self);
void func_0885A31C(Sound9D4DC8 *self);
void func_0885A398(Sound9D4DC8 *self);
void func_0885A41C(Sound9D4DC8 *self);
void func_0885A4FC(Sound9D4DC8 *self);
HandleSlot *func_0885A5B8(HandleSlot *self);
void func_0885A5C8(HandleSlot *self);
void func_0885A5D4(HandleSlot *self);
void func_0885A5E0(HandleSlot *self);
void func_0885A5EC(HandleSlot *self);
void func_0885A8B4(HandleSlot *self);
void func_0885ACE0(HandleSlot *self);
Handle *func_0885AF40(Handle *self);
void func_0885AF78(Handle *self, unsigned int *arg1, int arg2);
void func_0885AFD0(Part85B518 *self);
void func_0885B010(Part85B518 *self);
void func_0885B308(Part85B518 *self);
void func_0885B518(Part85B518 *self);

/* GU-style render state calls used by the manager's draw step. */
int func_089A04E8(void);
void func_089A296C(int arg);
void func_089A250C(void);
void func_089A2568(void);
void func_0899FDC4(unsigned int arg);
void func_089A119C(unsigned int arg);
void func_089A0C8C(int arg);

/* ------------------------------------------------------------ 0x0885BA00..0x0885EB60 */

/* An object whose only visible field is its first byte. */
typedef struct ActiveByte {
    unsigned char active;
} ActiveByte;
MAIN2_SIZE(ActiveByte, 0x1);
MAIN2_AT(ActiveByte, active, 0x0);

/* An object whose only visible field is one word. */
typedef struct Word1 {
    int value;
} Word1;
MAIN2_SIZE(Word1, 0x4);
MAIN2_AT(Word1, value, 0x0);

void *func_0885BC44(void *self);
ActiveByte *func_0885BA00(ActiveByte *self);
ActiveByte *func_0885BF64(ActiveByte *self);
void func_0885BF8C(ActiveByte *self, unsigned int flags);
void func_0885BFD8(ActiveByte *self);
void func_0885BFE0(ActiveByte *self);
int func_0885BFFC(ActiveByte *self);
unsigned char func_0885C304(ActiveByte *self);
int func_0885C404(void);

/* Vtable D_089C8E28 at +0x24 (constructor func_0885C490, reset func_0885C508). */
typedef struct Obj9C8E28 {
    int field_00;
    int field_04;
    char pad_08[0x4];
    int field_0C;
    char pad_10[0x14];
    void *vtable;
} Obj9C8E28;
MAIN2_AT(Obj9C8E28, field_00, 0x0);
MAIN2_AT(Obj9C8E28, field_04, 0x4);
MAIN2_AT(Obj9C8E28, field_0C, 0xC);
MAIN2_AT(Obj9C8E28, vtable, 0x24);

extern int D_089C8E28;
void func_0885C508(Obj9C8E28 *self);
Obj9C8E28 *func_0885C490(Obj9C8E28 *self, int arg1, int arg2, int arg3);

/* A header that starts with the bytes "ARC" 1 and points at itself. */
typedef struct Arc85CAFC {
    char magic[4];
    int field_04;
    struct Arc85CAFC *field_08;
} Arc85CAFC;
MAIN2_AT(Arc85CAFC, magic, 0x0);
MAIN2_AT(Arc85CAFC, field_04, 0x4);
MAIN2_AT(Arc85CAFC, field_08, 0x8);

void func_0885CAFC(Arc85CAFC *self);
void func_0885CB78(void);
unsigned int *func_089A00F4(void);
void func_0885CD44(void);

/* Two pointers at the static buffers D_09B17DB0 and D_09B17DF8. */
typedef struct Obj85CDE0 {
    void *field_00;
    void *field_04;
} Obj85CDE0;
MAIN2_SIZE(Obj85CDE0, 0x8);
MAIN2_AT(Obj85CDE0, field_00, 0x0);
MAIN2_AT(Obj85CDE0, field_04, 0x4);

extern char D_09B17DB0[];
extern char D_09B17DF8[];
Obj85CDE0 *func_0885CDE0(Obj85CDE0 *self);

/* Sixteen floats, then three 4-float vectors. */
typedef struct Obj85EA88 {
    float field_00[16];
    float field_40[4];
    float field_50[4];
    float field_60[4];
} Obj85EA88;
MAIN2_AT(Obj85EA88, field_00, 0x0);
MAIN2_AT(Obj85EA88, field_40, 0x40);
MAIN2_AT(Obj85EA88, field_50, 0x50);
MAIN2_AT(Obj85EA88, field_60, 0x60);

float *func_0885EA88(Obj85EA88 *self);
float *func_0885EA90(Obj85EA88 *self);
float *func_0885EA98(Obj85EA88 *self);
float *func_0885EAA0(Obj85EA88 *self);
void func_0885EAA8(Obj85EA88 *self, const float *value);
void func_0885EAC4(Obj85EA88 *self, const float *value);
void func_0885EAE0(Obj85EA88 *self, const float *value);
void func_0885EB34(Obj85EA88 *self, int arg1, int arg2, int arg3, int arg4);
Obj85EA88 *func_0885EAFC(Obj85EA88 *self);
void func_0885EB54(Word1 *self);
void func_0885EB60(Word1 *self);

/* ------------------------------------------------------------ 0x08860E3C..0x08863148 */

extern int D_089D5E80;            /* clock mode: 1 = 333 MHz, 0 = 222 MHz */
extern int D_089D5E84;            /* the mode before the last switch */
void func_089C5800(int pll, int cpu, int bus);
void func_08860E3C(void);
void func_08860E78(void);
void func_08860EB0(void);

/* Vtable D_089C8E60 at +0x43C; reset by func_08860F80. */
typedef struct Obj9C8E60 {
    unsigned char active;
    char pad_01[0x43B];
    void *vtable;
} Obj9C8E60;
MAIN2_SIZE(Obj9C8E60, 0x440);
MAIN2_AT(Obj9C8E60, active, 0x0);
MAIN2_AT(Obj9C8E60, vtable, 0x43C);

extern int D_089C8E60;
extern void *D_089D5E88;
void func_08860F80(Obj9C8E60 *self);
Obj9C8E60 *func_08860EF0(Obj9C8E60 *self);
void func_08860F28(Obj9C8E60 *self, unsigned int flags);
void func_08860FE4(Obj9C8E60 *self);
int func_08861000(Obj9C8E60 *self);
int func_08861208(void *self);
void func_08861030(void);
unsigned char func_08861050(Obj9C8E60 *self);

/* One step of Obj8625D4's list (reset by func_08861C48). */
typedef struct Step861C48 {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
} Step861C48;
MAIN2_SIZE(Step861C48, 0x14);
MAIN2_AT(Step861C48, field_00, 0x0);
MAIN2_AT(Step861C48, field_04, 0x4);
MAIN2_AT(Step861C48, field_08, 0x8);
MAIN2_AT(Step861C48, field_0C, 0xC);
MAIN2_AT(Step861C48, field_10, 0x10);

/* Runs its steps in turn. */
typedef struct Obj8625D4 {
    char pad_00[0x54];
    Step861C48 *steps[4];
    int count;
    int index;
} Obj8625D4;
MAIN2_AT(Obj8625D4, steps, 0x54);
MAIN2_AT(Obj8625D4, count, 0x64);
MAIN2_AT(Obj8625D4, index, 0x68);

void func_08861C48(Step861C48 *self);
int func_08861C64(Step861C48 *self);
void func_08861D7C(void *object, void *owner);
void func_08861E2C(void *object);
void func_08861EA0(void *owner, void *object);
void func_08861EC4(void *owner, void *object);
int func_088623A0(void);
int func_088625D4(Obj8625D4 *self);
extern char D_089C8EC6[];
void func_089C5828(int arg);
void func_089C5D58(void *arg);
void func_08862650(void);
extern void *D_089D5E98;          /* a heap */
void func_0886278C(void *block);

/* What D_089D5EA0's field_04 points at. */
typedef struct Obj9D5EA0Sub {
    char pad_00[0x20];
    int field_20;
} Obj9D5EA0Sub;
MAIN2_AT(Obj9D5EA0Sub, field_20, 0x20);

/* The object D_089D5EA0 points at. */
typedef struct Obj9D5EA0 {
    char pad_00[0x4];
    Obj9D5EA0Sub *field_04;
    void *field_08;
} Obj9D5EA0;
MAIN2_AT(Obj9D5EA0, field_04, 0x4);
MAIN2_AT(Obj9D5EA0, field_08, 0x8);

/* An object with 28-bit size/offset bits in its field_08 (func_088716C8). */
typedef struct Obj8716C8 {
    char pad_00[0x8];
    unsigned int field_08;
} Obj8716C8;
MAIN2_AT(Obj8716C8, field_08, 0x8);

/* Keeps one Obj8716C8. */
typedef struct Obj862A9C {
    Obj8716C8 *object;
} Obj862A9C;
MAIN2_AT(Obj862A9C, object, 0x0);

extern Obj9D5EA0 *D_089D5EA0;
unsigned char func_088630C8(void);
int func_08871718(void *self, Obj8716C8 *object, int arg2, int arg3, int arg4, int arg5, char arg6);
unsigned int func_088716F0(void *self, Obj8716C8 *object);
void func_08864B5C(Obj9D5EA0Sub *self);
void func_08862A9C(Obj862A9C *self, int arg2, int arg3, int arg4, int arg5);
void func_08862CB0(Obj862A9C *self);
void func_088630E0(void);
extern void *D_089D5FB4;
int func_088701F8(void *self, int arg1, int arg2, int arg3);
void func_08863148(int arg1, int arg2, int arg3);

int func_08870328(void *self, unsigned int arg1, unsigned int arg2, unsigned int arg3, unsigned int arg4, unsigned int arg5, unsigned char arg6);
int func_088704D0(void *self, int arg);
void func_08863174(unsigned int arg1, unsigned int arg2, unsigned int arg3, unsigned int arg4, unsigned int arg5, unsigned char arg6);
void func_088631AC(int arg);

/* Vtable D_089C8FE0 at +0x154 over a base (func_08863CD4) that keeps five ids at its start. */
typedef struct Obj9C8FE0 {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    char pad_14[0x140];
    void *vtable;
    int field_158;
} Obj9C8FE0;
MAIN2_AT(Obj9C8FE0, field_00, 0x0);
MAIN2_AT(Obj9C8FE0, field_04, 0x4);
MAIN2_AT(Obj9C8FE0, field_08, 0x8);
MAIN2_AT(Obj9C8FE0, field_0C, 0xC);
MAIN2_AT(Obj9C8FE0, field_10, 0x10);
MAIN2_AT(Obj9C8FE0, vtable, 0x154);
MAIN2_AT(Obj9C8FE0, field_158, 0x158);

extern int D_089C8FE0;
void *func_08863CD4(Obj9C8FE0 *self);
Obj9C8FE0 *func_088631D0(Obj9C8FE0 *self);
void func_089C5BD8(int id);
void func_089C5F90(int id);
void func_089C5810(int arg);
void func_08863E40(Obj9C8FE0 *self);
void func_088632D4(Obj9C8FE0 *self);

/* ------------------------------------------------------------ models and pools (0x088698FC..0x0886FCDC) */

Holder *func_088698FC(Holder *self);
void *func_088699F4(void *self);
void func_08869908(void *self, unsigned int flags);

/* One 0x13C-byte element of Pool86CE7C (constructor func_08869C40, destructor func_08869C74). */
typedef struct Elem869C40 {
    char pad_00[0x90];
    int field_90;
    char pad_94[0x74];
    int field_108;
    int field_10C;
    char pad_110[0x2C];
} Elem869C40;
MAIN2_SIZE(Elem869C40, 0x13C);
MAIN2_AT(Elem869C40, field_90, 0x90);
MAIN2_AT(Elem869C40, field_108, 0x108);
MAIN2_AT(Elem869C40, field_10C, 0x10C);

/* One 0x1C-byte item of Pool86CE7C (constructor D_0886546C, destructor func_08865488). */
typedef struct Item86546C {
    int field_00;
    char pad_04[0x18];
} Item86546C;
MAIN2_SIZE(Item86546C, 0x1C);
MAIN2_AT(Item86546C, field_00, 0x0);

/* Two heaps and three blocks, 192 elements and 384 items (destructor func_088799F0). */
typedef struct Pool86CE7C {
    void *block;
    void *heap_04;
    void *block_08;
    void *block_0C;
    void *block_10;
    void *heap_14;
    void *heap_18;
    Elem869C40 elems[192];
    Item86546C items[384];
} Pool86CE7C;
MAIN2_SIZE(Pool86CE7C, 0x1171C);
MAIN2_AT(Pool86CE7C, block, 0x0);
MAIN2_AT(Pool86CE7C, heap_04, 0x4);
MAIN2_AT(Pool86CE7C, block_08, 0x8);
MAIN2_AT(Pool86CE7C, block_0C, 0xC);
MAIN2_AT(Pool86CE7C, block_10, 0x10);
MAIN2_AT(Pool86CE7C, heap_14, 0x14);
MAIN2_AT(Pool86CE7C, heap_18, 0x18);
MAIN2_AT(Pool86CE7C, elems, 0x1C);
MAIN2_AT(Pool86CE7C, items, 0xED1C);

void func_08869CA0(Elem869C40 *self);
Elem869C40 *func_08869C40(Elem869C40 *self);
void func_08869C74(Elem869C40 *self);
extern int D_0886546C;            /* item constructor */
void func_08865488(Item86546C *self);
Pool86CE7C *func_0886CE7C(Pool86CE7C *self);
void func_0888218C(void *block, int arg);
void func_0886D038(Pool86CE7C *self);
void func_0886C774(void *self, int arg1, int arg2);
void func_0886A24C(void *self, int arg);
int func_0886A9B0(ModelObj *self, int part, int value, int index);
int func_0886AA98(ModelObj *self, int part);
void func_0886AC64(ModelObj *self);
void func_0886AC98(ModelObj *self);
void func_0886B230(ModelObj *self);
void func_0886B240(ModelObj *self);
unsigned int func_089B0E78(int handle);
void *func_089B1224(unsigned int object);
short func_089B0E3C(int handle);
void func_089B11CC(unsigned int object, int arg);
void func_0886CD40(ModelObj *self);
void func_0886CD5C(ModelObj *self, int arg1, int arg2);
void func_0899FFC8(unsigned int arg);
void func_0886D0BC(void);

/* The 0x18-byte part func_0886F028 builds (flags at +0x10). */
typedef struct Part86F028 {
    char pad_00[0x10];
    unsigned int flags;
    char pad_14[0x4];
} Part86F028;
MAIN2_SIZE(Part86F028, 0x18);
MAIN2_AT(Part86F028, flags, 0x10);

void *func_0886EFD4(void *self);
unsigned int func_0886F008(Part86F028 *self);
void func_0886F014(Part86F028 *self);
void func_089400B0(const void *data, unsigned int size, void *digest);
void func_0899F3FC(void *context);
void func_0899F030(void *context, const void *data, unsigned int size);
void func_0899F200(void *context, void *digest);
void func_0886F0E4(const void *key, unsigned int key_size, const void *data, int data_size, void *digest);
extern void *D_089D5FF0;
/* Declared without parameters: func_0886FCDC passes it one argument, func_08871644 two. */
unsigned int func_088716C8();
unsigned int func_0886FCDC(void);

/* ------------------------------------------------------------ 0x088707BC..0x08873084 */

void *func_088707BC(void *self);
void *func_0885E7C0(void *self);
void *func_08873498(void *self);
/* A class over the base func_0885E7C0 builds, with a member at +0xC0 (func_08873498). */
typedef struct Obj870D8C {
    char pad_00[0xC0];
    char field_C0[1];
} Obj870D8C;
MAIN2_AT(Obj870D8C, field_C0, 0xC0);

Obj870D8C *func_08870D8C(Obj870D8C *self);
extern int D_089D5FD8, D_089D5FDC, D_089D5FE0, D_089D5FE4;
void func_088711C0(int arg1, int arg2, int arg3, int arg4);
void func_088711E4(void);

/* Vtable D_089C9680 at +0x74. */
typedef struct Obj9C9680 {
    char pad_00[0x4];
    int field_04;
    char pad_08[0x6C];
    void *vtable;
} Obj9C9680;
MAIN2_AT(Obj9C9680, field_04, 0x4);
MAIN2_AT(Obj9C9680, vtable, 0x74);

extern int D_089C9680;
void func_088713B0(Obj9C9680 *self);
void func_0887133C(Obj9C9680 *self, unsigned int flags);
void func_08871938(Obj9C9680 *self);
void func_08871394(Obj9C9680 *self, int value);
Obj8716C8 *func_088713E4(Obj9C9680 *self);
void func_08871644(Obj9C9680 *self);

typedef void (*Callback871A04)(int arg, int data);

/* A callback at +0x68 with its data at +0x6C. */
typedef struct Obj871A04 {
    char pad_00[0x68];
    Callback871A04 callback;
    int data;
} Obj871A04;
MAIN2_AT(Obj871A04, callback, 0x68);
MAIN2_AT(Obj871A04, data, 0x6C);

void func_08871A04(Obj871A04 *self, int arg);
void func_08871A80(Obj871A04 *self, int arg);
void func_088719B8(void *object, void *owner);
void func_08871A34(void *object, void *owner);
void func_08871AB0(void *owner, void *object);
void func_08871AD4(int arg, Obj871A04 *object);
void func_08871AF8(void *owner, void *object);
void func_08871B1C(int arg, Obj871A04 *object);
long long func_08871BC4(long long *self);
long long func_08872138(void *self);
typedef struct Obj871E08 {
    char pad_00[0x20];
    char field_20[1];
} Obj871E08;
MAIN2_AT(Obj871E08, field_20, 0x20);

void func_08871E08(Obj871E08 *self);
void *func_08871CB8(void *self);

/* The three item types of Obj9D6008 (bases built by func_08871CB8). */
typedef struct Item87205C {
    char pad_00[0x50];
    int field_50;
    char pad_54[0xC];
} Item87205C;
MAIN2_SIZE(Item87205C, 0x60);
MAIN2_AT(Item87205C, field_50, 0x50);

typedef struct Item8724CC {
    char pad_00[0x34];
    void *vtable;
    char pad_38[0x8];
    void *field_40;
    unsigned int flags;
    int field_48;
    int field_4C;
} Item8724CC;
MAIN2_SIZE(Item8724CC, 0x50);
MAIN2_AT(Item8724CC, vtable, 0x34);
MAIN2_AT(Item8724CC, field_40, 0x40);
MAIN2_AT(Item8724CC, flags, 0x44);
MAIN2_AT(Item8724CC, field_48, 0x48);
MAIN2_AT(Item8724CC, field_4C, 0x4C);

typedef struct Item872ADC {
    int field_00;
    char pad_04[0x3C];
} Item872ADC;
MAIN2_SIZE(Item872ADC, 0x40);
MAIN2_AT(Item872ADC, field_00, 0x0);

/* The object D_089D6008 points at. */
typedef struct Obj9D6008 {
    Item87205C items60[2];
    Item8724CC items50[2];
    Item872ADC items40[2];
    char field_1E0[0x84];
    unsigned int flags;
} Obj9D6008;
MAIN2_AT(Obj9D6008, items60, 0x0);
MAIN2_AT(Obj9D6008, items50, 0xC0);
MAIN2_AT(Obj9D6008, items40, 0x160);
MAIN2_AT(Obj9D6008, field_1E0, 0x1E0);
MAIN2_AT(Obj9D6008, flags, 0x264);

extern Obj9D6008 *D_089D6008;
extern int D_089C96E8;
Item87205C *func_0887205C(Item87205C *self);
Item87205C *func_08872088(int index);
char *func_088720A4(void);
void func_088720B4(int value);
Item8724CC *func_088724CC(Item8724CC *self);
unsigned int func_08872594(Item8724CC *self);
void func_08872304(void *object);
void func_088725A0(Item8724CC *self);
void func_08872970(Item8724CC *self);
Item8724CC *func_088729A0(int index);
void func_08872E50(Obj9D6008 *self, int arg);
void func_08872A70(int arg);
Item872ADC *func_08872ADC(Item872ADC *self);
Item872ADC *func_08872B48(int index);
unsigned char func_08873084(void);

typedef struct Obj8735C4 {
    char pad_00[0x10];
    char field_10[1];
} Obj8735C4;
MAIN2_AT(Obj8735C4, field_10, 0x10);

char *func_088735C4(Obj8735C4 *self);

typedef struct Obj8745A4 {
    char pad_00[0x18];
    int field_18;
} Obj8745A4;
MAIN2_AT(Obj8745A4, field_18, 0x18);

void func_088745A4(Obj8745A4 *self);
void func_08874C7C(void);
void func_08874DFC(void);
extern void *D_089D6028;
void func_08874F24(void *self);
void func_08874FAC(void *self);
void func_088751B8(void);

/* Vtable D_089C9C38 at +0x20; reset by func_088758E0. */
typedef struct Obj9C9C38 {
    unsigned char active;
    char pad_01[0x1F];
    void *vtable;
} Obj9C9C38;
MAIN2_SIZE(Obj9C9C38, 0x24);
MAIN2_AT(Obj9C9C38, active, 0x0);
MAIN2_AT(Obj9C9C38, vtable, 0x20);

extern int D_089C9C38;
extern void *D_089D6040;
void func_088758E0(Obj9C9C38 *self);
Obj9C9C38 *func_08875850(Obj9C9C38 *self);
void func_08875888(Obj9C9C38 *self, unsigned int flags);
void func_0887590C(Obj9C9C38 *self);
int func_08875928(Obj9C9C38 *self);
int func_08875B40(void *self);
void func_08875958(void);
unsigned char func_08875978(Obj9C9C38 *self);
extern int D_09AFEE54;
void func_089A02E0(unsigned int arg1, unsigned int arg2, unsigned int arg3);
void func_089A0A90(unsigned int arg);
void func_089A0AA4(int arg);
void func_089A0AC4(int arg);
void func_08876330(void);
extern void *D_089D5EC0;
void func_08866AE0(void *self);
void func_089A123C(unsigned int arg1, int arg2, int arg3);
void func_089A1270(unsigned int arg1, int arg2, int arg3);
void func_08876370(void);

/* ------------------------------------------------------------ deleting destructors (0x088799F0..0x0887A19C) */

/* Four words (constructor func_0887A61C, reset func_0887A690, release func_0887AD1C,
 * deleting destructor func_0887A644 with its own delete func_0887B0D0). */
typedef struct Obj87A61C {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
} Obj87A61C;
MAIN2_SIZE(Obj87A61C, 0x10);
MAIN2_AT(Obj87A61C, field_00, 0x0);
MAIN2_AT(Obj87A61C, field_04, 0x4);
MAIN2_AT(Obj87A61C, field_08, 0x8);
MAIN2_AT(Obj87A61C, field_0C, 0xC);

typedef struct Obj879BBC {
    char pad_00[0x148];
    char field_148[0x14];
} Obj879BBC;
MAIN2_AT(Obj879BBC, field_148, 0x148);

typedef struct Obj879CB4 {
    char pad_00[0x288];
    Obj87A61C field_288;
} Obj879CB4;
MAIN2_AT(Obj879CB4, field_288, 0x288);

typedef struct Obj879D04 {
    char pad_00[0x14];
    char field_14[1];
} Obj879D04;
MAIN2_AT(Obj879D04, field_14, 0x14);

typedef struct Obj879D54 {
    char pad_00[0xC];
    Obj87A61C field_0C;
    char field_1C[1];
} Obj879D54;
MAIN2_AT(Obj879D54, field_0C, 0xC);
MAIN2_AT(Obj879D54, field_1C, 0x1C);

/* Seventy 8-byte, thirty-two 0x24-byte and six 0x14-byte elements over a base
 * destroyed by func_089209FC. */
typedef struct Obj879DB0 {
    char pad_00[0x10];
    char field_10[70][8];
    char field_240[32][0x24];
    char field_6C0[6][0x14];
} Obj879DB0;
MAIN2_AT(Obj879DB0, field_10, 0x10);
MAIN2_AT(Obj879DB0, field_240, 0x240);
MAIN2_AT(Obj879DB0, field_6C0, 0x6C0);

/* Forty-five 0x68-byte elements. */
typedef struct Obj879EC4 {
    char items[45][0x68];
} Obj879EC4;
MAIN2_AT(Obj879EC4, items, 0x0);

typedef struct Obj879F28 {
    char pad_00[0x10];
    char field_10[0xC0];
    char field_D0[1];
} Obj879F28;
MAIN2_AT(Obj879F28, field_10, 0x10);
MAIN2_AT(Obj879F28, field_D0, 0xD0);

typedef struct Obj879F84 {
    char pad_00[0x40];
    char field_40[0xC0];
    char field_100[1];
} Obj879F84;
MAIN2_AT(Obj879F84, field_40, 0x40);
MAIN2_AT(Obj879F84, field_100, 0x100);

typedef struct Obj87A19C {
    char pad_00[0x90];
    char field_90[1];
    char pad_91[0x17E3];
    char field_1874[1];
    char pad_1875[0x27B];
    char field_1AF0[1];
} Obj87A19C;
MAIN2_AT(Obj87A19C, field_90, 0x90);
MAIN2_AT(Obj87A19C, field_1874, 0x1874);
MAIN2_AT(Obj87A19C, field_1AF0, 0x1AF0);

void func_088799F0(Pool86CE7C *self, unsigned int flags);
void func_0892E670(void *self);
void func_08879BBC(Obj879BBC *self, unsigned int flags);
void func_0887A644(Obj87A61C *self, unsigned int flags);
void func_08879CB4(Obj879CB4 *self, unsigned int flags);
void func_0889F674(void *self, unsigned int flags);
void func_08879D04(Obj879D04 *self, unsigned int flags);
void func_08879D54(Obj879D54 *self, unsigned int flags);
void func_08920594(void *self);
void func_08922898(void *self);
void func_08924F68(void *self);
void func_089209FC(void *self, unsigned int flags);
void func_08879DB0(Obj879DB0 *self, unsigned int flags);
void func_08879E60(void *self);
void func_08879EC4(Obj879EC4 *self, unsigned int flags);
void func_0885E834(void *self, unsigned int flags);
void func_088734D4(void *self, unsigned int flags);
void func_08879F28(Obj879F28 *self, unsigned int flags);
void func_08879F84(Obj879F84 *self, unsigned int flags);
void func_088CE36C(void *self, unsigned int flags);
void func_088D6CFC(void *self, unsigned int flags);
void func_088DFD94(void *self, unsigned int flags);
void func_0887A19C(Obj87A19C *self, unsigned int flags);

/* ------------------------------------------------------------ 0x0887A2B4..0x0887BA5C */

/* Vtable D_089C9D30 at +0x0C; reset by func_0887A344. */
typedef struct Obj9C9D30 {
    unsigned char active;
    char pad_01[0x3];
    int field_04;
    char field_08;
    char pad_09[0x3];
    void *vtable;
} Obj9C9D30;
MAIN2_SIZE(Obj9C9D30, 0x10);
MAIN2_AT(Obj9C9D30, active, 0x0);
MAIN2_AT(Obj9C9D30, field_04, 0x4);
MAIN2_AT(Obj9C9D30, field_08, 0x8);
MAIN2_AT(Obj9C9D30, vtable, 0xC);

extern int D_089C9D30;
void func_0887A344(Obj9C9D30 *self);
void func_0887A354(Obj9C9D30 *self);
Obj9C9D30 *func_0887A2B4(Obj9C9D30 *self);
void func_0887A2EC(Obj9C9D30 *self, unsigned int flags);
unsigned char func_0887A484(Obj9C9D30 *self);

/* A node of Obj87A4FC's list (next at +0x10). */
typedef struct Node87A5E0 {
    char pad_00[0x10];
    struct Node87A5E0 *next;
} Node87A5E0;
MAIN2_AT(Node87A5E0, next, 0x10);

/* A list (first node at +0x1C) with a count at +0x28. */
typedef struct Obj87A4FC {
    char pad_00[0x12];
    char field_12;
    char field_13;
    int field_14;
    int field_18;
    Node87A5E0 *first;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
} Obj87A4FC;
MAIN2_AT(Obj87A4FC, field_12, 0x12);
MAIN2_AT(Obj87A4FC, field_13, 0x13);
MAIN2_AT(Obj87A4FC, field_14, 0x14);
MAIN2_AT(Obj87A4FC, field_18, 0x18);
MAIN2_AT(Obj87A4FC, first, 0x1C);
MAIN2_AT(Obj87A4FC, field_20, 0x20);
MAIN2_AT(Obj87A4FC, field_24, 0x24);
MAIN2_AT(Obj87A4FC, field_28, 0x28);
MAIN2_AT(Obj87A4FC, field_2C, 0x2C);
MAIN2_AT(Obj87A4FC, field_30, 0x30);
MAIN2_AT(Obj87A4FC, field_34, 0x34);

void func_0887A4FC(Obj87A4FC *self);
void func_0887A524(Obj87A4FC *self);
void func_0887A534(Obj87A4FC *self);
void func_0887A5D0(Obj87A4FC *self);
Node87A5E0 *func_0887A5E0(Obj87A4FC *self, Node87A5E0 *prev);
void func_0887A690(Obj87A61C *self);
Obj87A61C *func_0887A61C(Obj87A61C *self);
void func_0887AD1C(Obj87A61C *self);
void func_0887B0D0(Obj87A61C *self);
unsigned char func_0887AFB0(Obj87A61C *self);

/* A block and the heap it came from. */
typedef struct BlockRef {
    void *block;
    void *heap;
} BlockRef;
MAIN2_SIZE(BlockRef, 0x8);
MAIN2_AT(BlockRef, block, 0x0);
MAIN2_AT(BlockRef, heap, 0x4);

BlockRef *func_0887B1E4(BlockRef *self);
void func_0887B3B8(BlockRef *self);

/* Flags at +0x10 (functions at 0x0887B820..0x0887BA48). */
typedef struct Obj87B820 {
    int field_00;
    char pad_04[0x8];
    int field_0C;
    unsigned int flags;
} Obj87B820;
MAIN2_AT(Obj87B820, field_00, 0x0);
MAIN2_AT(Obj87B820, field_0C, 0xC);
MAIN2_AT(Obj87B820, flags, 0x10);

void func_0887B820(Obj87B820 *self, int value);
void func_0887B828(Obj87B820 *self, const void *src);
void func_0887B7E8(void);
void func_0887B908(void);
void func_0887B924(Obj87B820 *self);
void func_0887B934(Obj87B820 *self);
void func_0887B944(Obj87B820 *self);
void func_0887B98C(Obj87B820 *self);
void func_0887BA48(Obj87B820 *self, int value);
extern void *D_089D6998;          /* text manager */
void func_0887DB1C(void *self, void *text);
void func_0887BA5C(void *text);

/* Four shorts. */
typedef struct Quad87BBC8 {
    short field_00;
    short field_02;
    short field_04;
    short field_06;
} Quad87BBC8;
MAIN2_SIZE(Quad87BBC8, 0x8);
MAIN2_AT(Quad87BBC8, field_00, 0x0);
MAIN2_AT(Quad87BBC8, field_02, 0x2);
MAIN2_AT(Quad87BBC8, field_04, 0x4);
MAIN2_AT(Quad87BBC8, field_06, 0x6);

typedef struct Obj87BBC8 {
    char pad_00[0x28];
    Quad87BBC8 field_28[4];
} Obj87BBC8;
MAIN2_AT(Obj87BBC8, field_28, 0x28);

void func_0887BBC8(Obj87BBC8 *self);
Quad87BBC8 *func_0887BBF4(Obj87BBC8 *self);

/* Vtable D_089C9D48 at +0xCC; reset by func_0887CF40; the static instance is D_09AEF804. */
typedef struct Obj9C9D48 {
    unsigned char active;
    char pad_01[0xCB];
    void *vtable;
} Obj9C9D48;
MAIN2_SIZE(Obj9C9D48, 0xD0);
MAIN2_AT(Obj9C9D48, active, 0x0);
MAIN2_AT(Obj9C9D48, vtable, 0xCC);

extern int D_089C9D48;
extern int D_09AEF800;            /* set once D_09AEF804 is built */
extern Obj9C9D48 D_09AEF804;
extern int D_089D68F0;            /* its destructor entry */
void func_0887CF40(Obj9C9D48 *self);
int func_0887D2A8(Obj9C9D48 *self);
Obj9C9D48 *func_0887CEB0(Obj9C9D48 *self);
void func_0887CEE8(Obj9C9D48 *self, unsigned int flags);
void func_0887CF80(Obj9C9D48 *self);
int func_0887CFAC(Obj9C9D48 *self);
unsigned char func_0887CFDC(Obj9C9D48 *self);
Obj9C9D48 *func_0887CFE4(void);

/* One 500-byte entry of Obj87D574. */
typedef struct Entry87D574 {
    char active;
    char pad_01[0x1];
    unsigned char count;
    char pad_03[0x1F1];
} Entry87D574;
MAIN2_SIZE(Entry87D574, 0x1F4);
MAIN2_AT(Entry87D574, active, 0x0);
MAIN2_AT(Entry87D574, count, 0x2);

/* A 0x10-byte head, a 0x300-byte table, a word and twenty-eight entries. */
typedef struct Obj87D574 {
    char head[0x10];
    char table[0x300];
    int field_310;
    Entry87D574 entries[28];
} Obj87D574;
MAIN2_AT(Obj87D574, head, 0x0);
MAIN2_AT(Obj87D574, table, 0x10);
MAIN2_AT(Obj87D574, field_310, 0x310);
MAIN2_AT(Obj87D574, entries, 0x314);

extern int D_0887D534;            /* entry constructor */
Obj87D574 *func_0887D574(Obj87D574 *self);
void func_0887D5C8(Obj87D574 *self, const Obj87D574 *other);
void func_0887D60C(Obj87D574 *self);
void func_0887D648(Obj87D574 *self);
void func_0887D66C(Obj87D574 *self, int value);
void func_0887D674(Obj87D574 *self);
void func_0887D67C(unsigned int owner, Entry87D574 *entry);
void func_0887D750(void *owner, char *block);
void func_0887D828(void *owner, char *block);
void func_0887DBD4(void *self);
void func_0887DE04(void *self);
void func_0887DA08(void *self, void *text);
void func_0887E4F4(void);
void func_0887E514(void);
void func_0887E77C(void *owner, void *heap);

/* A pair at +0x10 saved to +0x20 by func_0887E798 and restored by func_0887E7B4. */
typedef struct Obj87E798 {
    char pad_00[0x10];
    int field_10;
    int field_14;
    char pad_18[0x8];
    int field_20;
    int field_24;
} Obj87E798;
MAIN2_AT(Obj87E798, field_10, 0x10);
MAIN2_AT(Obj87E798, field_14, 0x14);
MAIN2_AT(Obj87E798, field_20, 0x20);
MAIN2_AT(Obj87E798, field_24, 0x24);

void func_0887E798(Obj87E798 *self, int value);
void func_0887E7B4(Obj87E798 *self);
void func_0887E7D0(void *text);

/* ------------------------------------------------------------ memory (0x0888208C..0x08882FF4) */

/* The object D_089D69A0 points at; its two heaps sit near the end. */
typedef struct Mem9D69A0 {
    char pad_00[0xECDC00];
    void *heap_00;
    void *heap_04;
} Mem9D69A0;
MAIN2_AT(Mem9D69A0, heap_00, 0xECDC00);
MAIN2_AT(Mem9D69A0, heap_04, 0xECDC04);

extern Mem9D69A0 *D_089D69A0;
void func_0888208C(void *block);
void *func_088820C0(void);
void *func_088821C0(void);

typedef struct Obj882250 {
    char pad_00[0x2000];
    struct Obj88245C *field_2000;
    int field_2004;
    int field_2008;
} Obj882250;
MAIN2_AT(Obj882250, field_2000, 0x2000);
MAIN2_AT(Obj882250, field_2004, 0x2004);
MAIN2_AT(Obj882250, field_2008, 0x2008);

struct Obj88245C;
void func_0888245C(struct Obj88245C *self, unsigned int arg1, unsigned int arg2);
int func_088823C4(struct Obj88245C *self);
void func_088828BC(struct Obj88245C *self, void *object);
void func_08882250(Obj882250 *self);
struct Obj88245C *func_08882274(Obj882250 *self);
void func_0888227C(Obj882250 *self);
void func_08882298(Obj882250 *self, void *object);
void func_088822B4(Holder *self);
void func_088825B4(struct Obj88245C *self);
void func_088825D4(struct Obj88245C *self, void *object);

typedef struct Obj882E44 {
    int field_00;
    int field_04;
} Obj882E44;
MAIN2_SIZE(Obj882E44, 0x8);
MAIN2_AT(Obj882E44, field_00, 0x0);
MAIN2_AT(Obj882E44, field_04, 0x4);

extern Obj882E44 *D_089D69B8;
Obj882E44 *func_08882E44(Obj882E44 *self);
void func_08882E74(Obj882E44 *self);
void func_089C60A0(int arg);
void func_08882EC8(void);
int func_08882EF0(void);

/* Reset by func_08882FF4. */
typedef struct Obj882FF4 {
    char active;
    char pad_01[0x4F];
    int field_50;
    int field_54;
    int field_58;
    int field_5C;
    int field_60;
    int field_64;
    int field_68;
} Obj882FF4;
MAIN2_AT(Obj882FF4, active, 0x0);
MAIN2_AT(Obj882FF4, field_50, 0x50);
MAIN2_AT(Obj882FF4, field_54, 0x54);
MAIN2_AT(Obj882FF4, field_58, 0x58);
MAIN2_AT(Obj882FF4, field_5C, 0x5C);
MAIN2_AT(Obj882FF4, field_60, 0x60);
MAIN2_AT(Obj882FF4, field_64, 0x64);
MAIN2_AT(Obj882FF4, field_68, 0x68);

Obj882FF4 *func_08882F00(Obj882FF4 *self);
void func_08882FF4(Obj882FF4 *self);

/* Named nodes (name first) with their next pointer at +0x6C or +0x70. */
typedef struct Node883C88 {
    char name[1];
    char pad_01[0x6B];
    struct Node883C88 *next;
} Node883C88;
MAIN2_AT(Node883C88, name, 0x0);
MAIN2_AT(Node883C88, next, 0x6C);

typedef struct Node883D0C {
    char name[1];
    char pad_01[0x6F];
    struct Node883D0C *next;
} Node883D0C;
MAIN2_AT(Node883D0C, name, 0x0);
MAIN2_AT(Node883D0C, next, 0x70);

typedef struct List883C88 {
    Node883C88 *first;
} List883C88;
MAIN2_AT(List883C88, first, 0x0);

typedef struct List883D0C {
    Node883D0C *first;
} List883D0C;
MAIN2_AT(List883D0C, first, 0x0);

Node883C88 *func_08883C88(List883C88 *self, const char *name);
Node883D0C *func_08883D0C(List883D0C *self, const char *name);
void func_08883F9C(Obj9D69E0 *self, char arg1, unsigned char arg2);
void func_08883EE8(void);
int func_08883F54(void);
void func_08883F64(void);
void func_08883F78(void);

/* ------------------------------------------------------------ 0x088842E0..0x08886208 */

/* Three models and three handles, over a base destroyed by func_08885B14. */
typedef struct Obj8842E0 {
    void *models[3];
    void *handles[3];
} Obj8842E0;
MAIN2_AT(Obj8842E0, models, 0x0);
MAIN2_AT(Obj8842E0, handles, 0xC);

void func_088DA060(void *handle);
void func_088DAB44(void *model);
void func_08885B14(void *self, int flags);
void *func_08885B40(unsigned int size);
void func_08885B5C(void *object);
void func_088842E0(Obj8842E0 *self, unsigned int flags);

typedef struct Obj884990 {
    char pad_00[0x1C];
    unsigned int flags;
} Obj884990;
MAIN2_AT(Obj884990, flags, 0x1C);

void func_08884990(Obj884990 *self);

/* A model, a handle, an Obj87A61C and two texts, over a base destroyed by func_08885B14
 * (the object at Obj885220 +0x04). */
typedef struct Obj884A38 {
    void *model;
    void *handle;
    Obj87A61C *field_08;
    void *text_0C;
    void *text_10;
} Obj884A38;
MAIN2_AT(Obj884A38, model, 0x0);
MAIN2_AT(Obj884A38, handle, 0x4);
MAIN2_AT(Obj884A38, field_08, 0x8);
MAIN2_AT(Obj884A38, text_0C, 0xC);
MAIN2_AT(Obj884A38, text_10, 0x10);

/* The state object behind a model handle of Obj886208. */
typedef struct Handle886208 {
    char pad_00[0xC];
    short state;
} Handle886208;
MAIN2_AT(Handle886208, state, 0xC);

/* A model and a handle, over a base destroyed by func_08885B14 (the objects at
 * Obj885220 +0x08 and +0x0C). */
typedef struct Obj885DAC {
    void *model;
    void *handle;
} Obj885DAC;
MAIN2_AT(Obj885DAC, model, 0x0);
MAIN2_AT(Obj885DAC, handle, 0x4);

typedef struct Obj886208 {
    void *model;
    Handle886208 *handle;
    unsigned int flags;
} Obj886208;
MAIN2_AT(Obj886208, model, 0x0);
MAIN2_AT(Obj886208, handle, 0x4);
MAIN2_AT(Obj886208, flags, 0x8);

/* Four parts (D_089D69E8 points at one); built by func_08885A30, released by func_08885220. */
typedef struct Obj885220 {
    void *field_00;
    Obj884A38 *field_04;
    Obj885DAC *field_08;
    Obj886208 *field_0C;
    Obj8842E0 *field_10;
} Obj885220;
MAIN2_SIZE(Obj885220, 0x14);
MAIN2_AT(Obj885220, field_00, 0x0);
MAIN2_AT(Obj885220, field_04, 0x4);
MAIN2_AT(Obj885220, field_08, 0x8);
MAIN2_AT(Obj885220, field_0C, 0xC);
MAIN2_AT(Obj885220, field_10, 0x10);

extern Obj882250 *D_089D69A8;
extern Obj885220 *D_089D69E8;
void func_08884A38(Obj884A38 *self, unsigned int flags);
void func_08885DAC(Obj885DAC *self, unsigned int flags);
void func_08886208(Obj886208 *self, unsigned int flags);
void func_08885220(Obj885220 *self);
void func_08884388(Obj8842E0 *self, unsigned char arg1, unsigned char arg2);
void func_08884B00(Obj884A38 *self, unsigned char arg1, unsigned char arg2);
void func_08885E2C(Obj885DAC *self, unsigned char arg1, unsigned char arg2);
void func_08886288(Obj886208 *self, unsigned char arg);
void func_088852C4(Obj885220 *self, unsigned char skip, unsigned char arg);
void func_08885340(Obj885220 *self, unsigned char arg);
void func_08885364(Obj885220 *self, unsigned char arg);
void func_08885388(Obj885220 *self);
void func_088853A8(Obj885220 *self, unsigned char arg);
void func_0888630C(Obj886208 *self);
void func_08885EF0(Obj885DAC *self);
void func_08884BC0(Obj884A38 *self);
void func_0888544C(Obj885220 *self);
void func_08885474(Obj885220 *self);
void func_0888549C(Obj885220 *self);
void func_088864E0(Obj886208 *self, unsigned char arg1, unsigned short arg2);
void func_0888611C(Obj885DAC *self, unsigned char arg1, unsigned short arg2);
void func_088854C4(Obj885220 *self, unsigned char arg1, unsigned short arg2);
void func_088854E8(Obj885220 *self, unsigned char arg1, unsigned short arg2);
void func_08885004(Obj884A38 *self);
void func_0888509C(Obj884A38 *self);
void func_0888550C(Obj885220 *self);
void func_08885528(Obj885220 *self);
void func_08886520(Obj886208 *self, unsigned char arg);
void func_088847E4(Obj8842E0 *self, unsigned char arg);
void func_08886588(Obj886208 *self, unsigned char arg);
void func_08884898(Obj8842E0 *self, unsigned char arg);
void func_08885890(Obj885220 *self, unsigned char arg);
void func_088858E4(Obj885220 *self, unsigned char arg);
void func_08884C70(Obj884A38 *self);
void func_08885F8C(Obj885DAC *self);
void func_0888638C(Obj886208 *self);
void func_08884590(Obj8842E0 *self);
void func_088859D0(Obj885220 *self);
void func_08885A10(void);
Obj885220 *func_08885A30(Obj885220 *self);
void func_088CF970(void *self, int arg);
void func_08885A78(int arg);
void *func_08885B0C(void *self);

/* Two Obj87A61C (D_089D69F0 points at one). */
typedef struct Obj885B78 {
    Obj87A61C field_00;
    Obj87A61C field_10;
} Obj885B78;
MAIN2_SIZE(Obj885B78, 0x20);
MAIN2_AT(Obj885B78, field_00, 0x0);
MAIN2_AT(Obj885B78, field_10, 0x10);

extern Obj885B78 *D_089D69F0;
Obj885B78 *func_08885B78(Obj885B78 *self);
void func_08885BA8(Obj885B78 *self, unsigned int flags);
int func_0887AD78(Obj87A61C *self, int arg1, int arg2, int arg3, int arg4, char arg5);
char *func_0887AFBC(Obj87A61C *self, int arg, char flag);
void func_08885CC8(int arg1, int arg2, int arg3);
void func_08885CFC(int arg);
Obj87A61C *func_08885D24(void);
unsigned char func_088864AC(Obj886208 *self);
void func_08886640(Obj886208 *self);

typedef struct Obj886934 {
    char pad_00[0x58];
    char field_58;
    char field_59;
    char pad_5A[0x2];
    Word1 field_5C;
    char pad_60[0x14];
    char field_74[1];
} Obj886934;
MAIN2_AT(Obj886934, field_58, 0x58);
MAIN2_AT(Obj886934, field_59, 0x59);
MAIN2_AT(Obj886934, field_5C, 0x5C);
MAIN2_AT(Obj886934, field_74, 0x74);

void func_0888FA08(void *self);
void func_08886934(Obj886934 *self);

/* Forty-five pointers and the value they are compared with. */
typedef struct Obj886A38 {
    int *field_00[45];
    int field_B4;
} Obj886A38;
MAIN2_AT(Obj886A38, field_00, 0x0);
MAIN2_AT(Obj886A38, field_B4, 0xB4);

unsigned char func_08886A38(Obj886A38 *self, int index);
int func_08886A58(Obj886A38 *self);
int func_088908E0(void);
int func_0887AC70(Obj87A61C *self, int arg1, int arg2, int arg3);
void func_088874EC(Obj87A61C *self, int arg1, int arg2);
void func_0888753C(Obj87A61C *self);

typedef struct Obj8876A8 {
    char pad_00[0x4];
    int field_04;
    char pad_08[0x10];
    char field_18;
} Obj8876A8;
MAIN2_AT(Obj8876A8, field_04, 0x4);
MAIN2_AT(Obj8876A8, field_18, 0x18);

void func_088876A8(Obj8876A8 *self);
void func_088876B0(Obj8876A8 *self);

/* A row of four shorts of the table D_089D7354 points at. */
typedef struct Row887928 {
    short values[4];
} Row887928;
MAIN2_SIZE(Row887928, 0x8);
MAIN2_AT(Row887928, values, 0x0);

/* (row, column) into that table. */
typedef struct Cell887928 {
    int row;
    int column;
} Cell887928;
MAIN2_SIZE(Cell887928, 0x8);
MAIN2_AT(Cell887928, row, 0x0);
MAIN2_AT(Cell887928, column, 0x4);

extern Row887928 *D_089D7354;
int func_08887928(Cell887928 *self);
extern int D_09B47B50;            /* a static object */
void func_088739CC(void *self);
void func_08873A64(void *self);
void func_08888078(void);
void func_08888098(void);
extern void *D_089D4D80;
void func_0884BF0C(void *self);
void func_0884BFE0(void *self);
void func_0884C0BC(void *self);
void func_088880B8(void);
void func_088880D8(void);
void func_088880F8(void);

/* Menu-overlay objects and calls. */
extern void *D_08ACEE20, *D_08ACEE24, *D_08A405E0, *D_08AD0158;
int func_08ACD138(void *self, int arg);
int func_08ACD7EC(void *self);
int func_08ACE908(void *self);
int func_08A3E268(void *self);
int func_08ACFE30(void *self);
void func_08888118(int arg);
void func_0888813C(void);
void func_0888815C(void);
void func_0888817C(void);
void func_0888819C(void);
void func_088881BC(void);
void func_088881DC(void);

/* ------------------------------------------------------------ the flow object *D_089D6A10 (0x0888877C..0x0888AD24) */

typedef void (*FlowCallback)(void);

/* The object D_089D6A10 points at: field_48 is set to 7 before the callbacks are queued,
 * callbacks[i] / callback_flags[i] are set by func_08889ED4. */
typedef struct Flow9D6A10 {
    char pad_00[0x38];
    int field_38[2];
    int field_40[2];
    int field_48;
    unsigned int field_4C;
    char pad_50[0x4];
    int field_54;
    int field_58;
    char pad_5C[0xC];
    unsigned char field_68;
    char field_69;
    char pad_6A[0x6D6];
    unsigned int field_740;
    FlowCallback callbacks[3];
    unsigned char callback_flags[3];
    char pad_753[0x51];
    void *field_7A4;
    char pad_7A8[0x20];
    int field_7C8;
    char pad_7CC[0xC];
    int field_7D8[6];
    int field_7F0[4];
    int field_800;
} Flow9D6A10;
MAIN2_AT(Flow9D6A10, field_38, 0x38);
MAIN2_AT(Flow9D6A10, field_40, 0x40);
MAIN2_AT(Flow9D6A10, field_48, 0x48);
MAIN2_AT(Flow9D6A10, field_4C, 0x4C);
MAIN2_AT(Flow9D6A10, field_54, 0x54);
MAIN2_AT(Flow9D6A10, field_58, 0x58);
MAIN2_AT(Flow9D6A10, field_68, 0x68);
MAIN2_AT(Flow9D6A10, field_69, 0x69);
MAIN2_AT(Flow9D6A10, field_740, 0x740);
MAIN2_AT(Flow9D6A10, callbacks, 0x744);
MAIN2_AT(Flow9D6A10, callback_flags, 0x750);
MAIN2_AT(Flow9D6A10, field_7A4, 0x7A4);
MAIN2_AT(Flow9D6A10, field_7C8, 0x7C8);
MAIN2_AT(Flow9D6A10, field_7D8, 0x7D8);
MAIN2_AT(Flow9D6A10, field_7F0, 0x7F0);
MAIN2_AT(Flow9D6A10, field_800, 0x800);

extern Flow9D6A10 *D_089D6A10;
extern void *D_089D6A14;
void func_089DD8A0(void *object);
void func_0888877C(Flow9D6A10 *self);
extern void *D_08B12DE0;
int func_08B12C48(void *self);
int func_08B12BD4(void *self);
void func_08888CA0(Flow9D6A10 *self);

typedef struct Obj8898AC {
    char pad_00[0x4];
    char field_04[1];
} Obj8898AC;
MAIN2_AT(Obj8898AC, field_04, 0x4);

void func_08865158(void *self);
void func_088898AC(Obj8898AC *self);
void func_08888F2C(Obj8898AC *self);
void func_08888F90(Holder *self);
void func_088FE9CC(void);
unsigned char func_088FEA4C(void);
void func_08889060(void);
void func_0888907C(void);
void func_088886F8(Flow9D6A10 *self);
void func_088892E8(Flow9D6A10 *self);
void func_088890F8(void);
void func_08889118(void);
void func_088892C8(void);
unsigned char func_08889468(void *self, int kind);
void func_08889694(Flow9D6A10 *self, unsigned char arg);
void func_0888964C(Flow9D6A10 *self, unsigned int arg1, int arg2, unsigned int arg3, unsigned char arg4);
void func_088898F8(Flow9D6A10 *self);
void func_08889904(Flow9D6A10 *self, int index);
int func_0889716C(int arg);
int func_08889924(Flow9D6A10 *self);
int func_08889B08(Flow9D6A10 *self);
int func_08889C30(Flow9D6A10 *self);
void func_08889ED4(int index, FlowCallback callback, unsigned char flag);
FlowCallback func_08889EFC(int index);
unsigned char func_08889F14(int index);
extern void *D_08B23058;
int func_08B21480(void *self);
void func_0888A134(void);
void func_0888A2B4(int index, unsigned char on);
void func_0888A3A4(unsigned char first);
void func_0888B8FC(void *self, unsigned char arg);
void func_0888B970(void *self);
void func_0888A3F8(void);
void func_0888A544(void);
void func_0888A578(void);
extern void *D_089D8508;
void func_0891BA74(void *self);
void func_0888A5B0(void);
void func_0888A5F4(void);
void func_0888A664(void);
void func_0888A688(void);
extern void *D_089E09C0;
int func_089DE910(void *self, int arg);
int func_089DE94C(void *self);
void func_0888A6D8(void);
void func_0888A70C(void);
int func_08B3CC50(void);
void func_0888A75C(void);
void func_0888A778(void);
extern void *D_089D6AA8;
void func_0888E734(void *self, int on);
void func_0888A7BC(void);
void func_0888A7E0(void);
extern void *D_08A23FF0;
int func_08A1B278(void *self, int arg1, int arg2);
void func_0888A85C(void);
void func_0888A884(void);
extern void *D_08A4C0FC;
int func_08A49BDC(void *self, int arg);
void func_0888A8C8(void);
void func_0888A8EC(void);
int func_08A50160(void);
void func_0888A93C(void);
void func_0888A958(void);
void func_0888A9A8(void);
void func_0888AA78(void);
extern void *D_089D8140;
extern void *D_089E9D38;
void func_089009D4(void *self);
int func_089E7DF0(void *self, int arg1, int arg2);
void func_0888AAB0(void);
void func_0888AAE4(void);
void func_0888AB28(void);
void func_0888AB6C(void);
void func_0888ABF0(void);
void func_0888AC14(void);
extern void *D_08A8B7A8;
int func_08A88EDC(void *self, int arg1, int arg2);
void func_0888AC78(void);
void func_0888ACA0(void);
int func_089DDD90(void *self, int arg1, int arg2);
void func_0888AD24(int arg1, int arg2);
#endif /* DISSIDIA_MAIN_AUTO_REGION2_H */
