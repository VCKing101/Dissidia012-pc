#ifndef DISSIDIA_MAIN_AUTO_REGION1_H
#define DISSIDIA_MAIN_AUTO_REGION1_H

/* Types and prototypes behind the automatically matched functions in main
 * 0x088045FC..0x08885340 (readability pass, main region 1).
 *
 * Small C++ objects that keep a vtable pointer are named after the vtable's
 * address (Obj9C6DA0 has its vtable pointer set to D_089C6DA0). Objects
 * reached through a global pointer are named after the global (World9D4958
 * is *D_089D4958). Fields whose use is not plain keep their offset as their
 * name. A first byte that a reset clears and a getter returns is `active`.
 * Every layout carries compile-time size/offset assertions (MAIN1_SIZE /
 * MAIN1_AT). */

#define MAIN1_OFFSET(type, field) ((unsigned int)&((type *)0)->field)
#define MAIN1_ASSERT(name, cond) typedef char main1_assert_##name[(cond) ? 1 : -1]
#define MAIN1_SIZE(type, size) MAIN1_ASSERT(size_##type, sizeof(type) == (size))
#define MAIN1_AT(type, field, offset) \
    MAIN1_ASSERT(at_##type##_##field, MAIN1_OFFSET(type, field) == (offset))

/* ------------------------------------------------------------ runtime (SDK/libc) */

void *func_08935524(void *dst, const void *src, unsigned int size);       /* memcpy */
void *func_08935828(void *dst, int value, unsigned int size);             /* memset */
int func_0893585C(const char *a, const char *b);                           /* strcmp */
void *func_08934944(int key, const void *base, int count, int size, void *compare); /* bsearch */
void func_08992F04(void *object);                                          /* operator delete */
void func_08992F6C(void *entry);            /* registers a static object's destructor */
void func_08993560(void *array, int count, int size, void *ctor);          /* build each element */
void func_0899372C(void *array, int count, int size, void *dtor, int arg4, int arg5);

/* Vectors and matrices. */
void *func_089A34E0(float *v);                       /* clears a 3-float vector */
void *func_089A3868(float *dst, const float *src);   /* copies a 4x4 matrix */
void *func_089A3890(float *m);                       /* sets a 4x4 matrix to identity */
int func_089A3C18(void *dst, const void *src);
int func_089A3F40(void *dst, const void *a, const void *b);

/* Vtables and static records. */
extern int D_089C6CA8, D_089C6CE0, D_089C6D18;
extern int D_089C6D60, D_089C6D70, D_089C6D80, D_089C6DA0;
extern int D_089C6DB0, D_089C6DC0, D_089C6DD0, D_089C6DE0, D_089C6DF0, D_089C6E00;
extern int D_089C6F88, D_089C6FB8, D_089C7248, D_089C74B0, D_089C74C8, D_089C74E8;
extern int D_089C74F8, D_089C7510, D_089C7520, D_089C7530;
extern int D_089C7548, D_089C7558, D_089C7568, D_089C7578, D_089C7588, D_089C7598;
extern int D_089C75A8, D_089C75B8, D_089C75C8, D_089C75D8, D_089C75E8, D_089C75F8;
extern int D_089C7608, D_089C7618;
extern int D_089C7638, D_089C7650, D_089C7668, D_089C7680, D_089C7690;
extern int D_089C78C0, D_089C78D0, D_089C7C48;

/* ------------------------------------------------------------ small shared shapes */

/* An object whose only visible field is one word, cleared by its constructor. */
typedef struct Holder {
    void *value;
} Holder;
MAIN1_SIZE(Holder, 0x4);
MAIN1_AT(Holder, value, 0x0);


/* A two-word member object with its vtable pointer after the data. */
typedef struct VPair {
    int field_00;
    int field_04;
    void *vtable;
} VPair;
MAIN1_SIZE(VPair, 0xC);
MAIN1_AT(VPair, field_00, 0x0);
MAIN1_AT(VPair, field_04, 0x4);
MAIN1_AT(VPair, vtable, 0x8);


/* A four-word member object with its vtable pointer after the data. */
typedef struct VQuad {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
    void *vtable;
} VQuad;
MAIN1_SIZE(VQuad, 0x14);
MAIN1_AT(VQuad, field_00, 0x0);
MAIN1_AT(VQuad, field_04, 0x4);
MAIN1_AT(VQuad, field_08, 0x8);
MAIN1_AT(VQuad, field_0C, 0xC);
MAIN1_AT(VQuad, vtable, 0x10);


/* A word that is only valid while the first byte is set. */
typedef struct ActiveWord {
    unsigned char active;
    char pad_01[0x3];
    unsigned int value;
} ActiveWord;
MAIN1_SIZE(ActiveWord, 0x8);
MAIN1_AT(ActiveWord, active, 0x0);
MAIN1_AT(ActiveWord, value, 0x4);


/* ------------------------------------------------------------ actors */

/* Two 256-bit sets indexed by actor id (at an actor link's +0x28). */
typedef struct ActorBits {
    int field_00;
    unsigned int bits_04[8];
    unsigned int bits_24[8];
} ActorBits;
MAIN1_SIZE(ActorBits, 0x44);
MAIN1_AT(ActorBits, field_00, 0x0);
MAIN1_AT(ActorBits, bits_04, 0x4);
MAIN1_AT(ActorBits, bits_24, 0x24);


/* The object an actor keeps at +0x3AC. */
typedef struct ActorLink {
    char pad_00[0x28];
    ActorBits bits;
} ActorLink;
MAIN1_AT(ActorLink, bits, 0x28);


/* An actor (base built by func_0883975C); listed by the world at +0x0C, next at +0x4A0. */
typedef struct Actor {
    char pad_00[0x8];
    unsigned int flags_08;
    char pad_0C[0x1A];
    short id;
    char pad_28[0x384];
    struct ActorLink *link;
    char pad_3B0[0xF0];
    struct Actor *next;
} Actor;
MAIN1_AT(Actor, flags_08, 0x8);
MAIN1_AT(Actor, id, 0x26);
MAIN1_AT(Actor, link, 0x3AC);
MAIN1_AT(Actor, next, 0x4A0);


/* An object on the world's list at +0x1C (next at +0xB54). */
typedef struct Chara {
    char pad_00[0xBC];
    float field_BC;
    char pad_C0[0x2C4];
    unsigned int flags_384;
    char pad_388[0x1F0];
    float duration;
    float rate;
    unsigned int flags_580;
    char pad_584[0x5D0];
    struct Chara *next;
} Chara;
MAIN1_AT(Chara, field_BC, 0xBC);
MAIN1_AT(Chara, flags_384, 0x384);
MAIN1_AT(Chara, duration, 0x578);
MAIN1_AT(Chara, rate, 0x57C);
MAIN1_AT(Chara, flags_580, 0x580);
MAIN1_AT(Chara, next, 0xB54);


/* A component that keeps its actor at +0x30. */
typedef struct ActorPart {
    char pad_00[0x30];
    struct Actor *actor;
} ActorPart;
MAIN1_AT(ActorPart, actor, 0x30);


void func_08804498(void *self, Chara *chara, int mode);
void func_08805C0C(Chara *self);
void func_08806B1C(Chara *self, unsigned char exclusive);
void func_08806B58(Chara *self, unsigned char exclusive);
void *func_08807A00(unsigned int size);
void func_08807A1C(void *object);
void func_08807788(void *self, int flags);
int func_0880AD28(ActorBits *bits, void *self, int arg, int *result);
void func_0880AE5C(ActorBits *bits, Actor *actor);
void func_08809CEC(void *self, void *object);
void func_088045FC(void *self, int arg1, int arg2);
void func_08A5BF84(int arg1, int arg2);

/* Class with vtable D_089C6CA8 at +0x70 (base destructor func_08807788). */
typedef struct Obj9C6CA8 {
    char pad_00[0x70];
    void *vtable;
} Obj9C6CA8;
MAIN1_AT(Obj9C6CA8, vtable, 0x70);


/* Class with vtable D_089C6D18 at +0x70 (base destructor func_08807788). */
typedef struct Obj9C6D18 {
    char pad_00[0x70];
    void *vtable;
    char pad_74[0x194];
    int *field_208;
} Obj9C6D18;
MAIN1_AT(Obj9C6D18, vtable, 0x70);
MAIN1_AT(Obj9C6D18, field_208, 0x208);


typedef struct Obj80CD90 {
    char pad_00[0x3D];
    char field_3D;
} Obj80CD90;
MAIN1_AT(Obj80CD90, field_3D, 0x3D);


/* Built by func_0880FB6C; field_9C is a member built by func_0886F028. */
typedef struct Obj80FB6C {
    char pad_00[0x40];
    float matrix[16];
    char pad_80[0x18];
    void *field_98;
    char field_9C[0x18];
    int field_B4;
    float field_B8;
    unsigned int flags;
} Obj80FB6C;
MAIN1_AT(Obj80FB6C, matrix, 0x40);
MAIN1_AT(Obj80FB6C, field_98, 0x98);
MAIN1_AT(Obj80FB6C, field_9C, 0x9C);
MAIN1_AT(Obj80FB6C, field_B4, 0xB4);
MAIN1_AT(Obj80FB6C, field_B8, 0xB8);
MAIN1_AT(Obj80FB6C, flags, 0xBC);


void func_0880FFE8(Obj80FB6C *self);
void func_0880FC58(Obj80FB6C *self);
void *func_0886F028(void *part);
void func_0886F048(void *part, int flags);
void func_0886F08C(void *part);
void func_0886AB1C(void *owner, void *part, int arg);
void func_0880FE8C(Obj80FB6C *self);
void func_0880FEA0(Obj80FB6C *self, unsigned int bits, unsigned char on);

/* Four words cleared by func_088103F4. */
typedef struct Obj8103F4 {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
} Obj8103F4;
MAIN1_SIZE(Obj8103F4, 0x10);
MAIN1_AT(Obj8103F4, field_00, 0x0);
MAIN1_AT(Obj8103F4, field_04, 0x4);
MAIN1_AT(Obj8103F4, field_08, 0x8);
MAIN1_AT(Obj8103F4, field_0C, 0xC);


/* A loaded file's header: counts and offsets from its start. */
typedef struct FileHeader810414 {
    char pad_00[0x4];
    int count_04;
    int offset_08;
    char pad_0C[0x4];
    int offset_10;
    char pad_14[0x4];
    int offset_18;
    int count_1C;
    int offset_20;
} FileHeader810414;
MAIN1_AT(FileHeader810414, count_04, 0x4);
MAIN1_AT(FileHeader810414, offset_08, 0x8);
MAIN1_AT(FileHeader810414, offset_10, 0x10);
MAIN1_AT(FileHeader810414, offset_18, 0x18);
MAIN1_AT(FileHeader810414, count_1C, 0x1C);
MAIN1_AT(FileHeader810414, offset_20, 0x20);


/* The header's counts and tables as absolute addresses. */
typedef struct FileView810414 {
    int count_00;
    int table_04;
    int count_08;
    int table_0C;
    int table_10;
    int table_14;
} FileView810414;
MAIN1_SIZE(FileView810414, 0x18);
MAIN1_AT(FileView810414, count_00, 0x0);
MAIN1_AT(FileView810414, table_04, 0x4);
MAIN1_AT(FileView810414, count_08, 0x8);
MAIN1_AT(FileView810414, table_0C, 0xC);
MAIN1_AT(FileView810414, table_10, 0x10);
MAIN1_AT(FileView810414, table_14, 0x14);


/* Vtable D_089C6D60; reset by func_088105B4. */
typedef struct Obj9C6D60 {
    char active;
    char pad_01[0x3];
    int field_04;
    int field_08;
    VPair field_0C;
    VPair field_18;
    void *vtable;
} Obj9C6D60;
MAIN1_SIZE(Obj9C6D60, 0x28);
MAIN1_AT(Obj9C6D60, active, 0x0);
MAIN1_AT(Obj9C6D60, field_04, 0x4);
MAIN1_AT(Obj9C6D60, field_08, 0x8);
MAIN1_AT(Obj9C6D60, field_0C, 0xC);
MAIN1_AT(Obj9C6D60, field_18, 0x18);
MAIN1_AT(Obj9C6D60, vtable, 0x24);


/* Vtable D_089C6DA0; reset by func_088109F4. */
typedef struct Obj9C6DA0 {
    unsigned char active;
    char pad_01[0x3];
    int field_04;
    int field_08;
    void *vtable;
} Obj9C6DA0;
MAIN1_SIZE(Obj9C6DA0, 0x10);
MAIN1_AT(Obj9C6DA0, active, 0x0);
MAIN1_AT(Obj9C6DA0, field_04, 0x4);
MAIN1_AT(Obj9C6DA0, field_08, 0x8);
MAIN1_AT(Obj9C6DA0, vtable, 0xC);


/* Vtable D_089C6DB0; five member pairs; reset by func_08810D7C. */
typedef struct Obj9C6DB0 {
    char active;
    char pad_01[0x3];
    int field_04;
    int field_08;
    VPair field_0C;
    VPair field_18;
    VPair field_24;
    VPair field_30;
    VPair field_3C;
    void *vtable;
} Obj9C6DB0;
MAIN1_SIZE(Obj9C6DB0, 0x4C);
MAIN1_AT(Obj9C6DB0, active, 0x0);
MAIN1_AT(Obj9C6DB0, field_04, 0x4);
MAIN1_AT(Obj9C6DB0, field_08, 0x8);
MAIN1_AT(Obj9C6DB0, field_0C, 0xC);
MAIN1_AT(Obj9C6DB0, field_18, 0x18);
MAIN1_AT(Obj9C6DB0, field_24, 0x24);
MAIN1_AT(Obj9C6DB0, field_30, 0x30);
MAIN1_AT(Obj9C6DB0, field_3C, 0x3C);
MAIN1_AT(Obj9C6DB0, vtable, 0x48);


void func_088105B4(Obj9C6D60 *self);
void func_088109F4(Obj9C6DA0 *self);
void func_08810A04(Obj9C6DA0 *self);
void func_08810D7C(Obj9C6DB0 *self);

extern int D_089D4790;   /* a buffer freed with func_08882158 */
extern int D_089D4794;
extern int D_089D4798;
extern int D_089D479C;
void func_08882158(void *block);
void func_088118B8(Holder *self);
void *func_088130F0(void);
void func_088131BC(void *object, int arg1, int arg2, int arg3);

/* Vtable D_089C6F88 at +0 (also the base of D_089C6FB8 and D_089C7C48). */
typedef struct Obj9C6F88 {
    void *vtable;
    const void *field_04;
} Obj9C6F88;
MAIN1_SIZE(Obj9C6F88, 0x8);
MAIN1_AT(Obj9C6F88, vtable, 0x0);
MAIN1_AT(Obj9C6F88, field_04, 0x4);


void func_088A4EC0(Obj9C6F88 *self);
void func_08814360(Obj9C6F88 *self, unsigned int flags);
float func_08838300(Actor *actor);
void func_08831360(void);

/* Member object with vtable D_089C7690 at +0x34 (func_08822294 / func_088222CC). */
typedef struct Obj9C7690 {
    char pad_00[0x34];
    void *vtable;
} Obj9C7690;
MAIN1_SIZE(Obj9C7690, 0x38);
MAIN1_AT(Obj9C7690, vtable, 0x34);


/* Two vectors and three flag/value pairs; vtable D_089C74C8 at +0x28. */
typedef struct Obj9C74C8 {
    float field_00[3];
    float field_0C[3];
    char field_18;
    char field_19;
    char field_1A;
    char pad_1B[0x1];
    int field_1C;
    int field_20;
    int field_24;
    void *vtable;
} Obj9C74C8;
MAIN1_SIZE(Obj9C74C8, 0x2C);
MAIN1_AT(Obj9C74C8, field_00, 0x0);
MAIN1_AT(Obj9C74C8, field_0C, 0xC);
MAIN1_AT(Obj9C74C8, field_18, 0x18);
MAIN1_AT(Obj9C74C8, field_19, 0x19);
MAIN1_AT(Obj9C74C8, field_1A, 0x1A);
MAIN1_AT(Obj9C74C8, field_1C, 0x1C);
MAIN1_AT(Obj9C74C8, field_20, 0x20);
MAIN1_AT(Obj9C74C8, field_24, 0x24);
MAIN1_AT(Obj9C74C8, vtable, 0x28);


/* Vtable D_089C7680 at +0x14; reset by func_08821168. */
typedef struct Obj9C7680 {
    char active;
    char field_01;
    char pad_02[0x2];
    int field_04;
    char pad_08[0x4];
    int field_0C;
    char pad_10[0x4];
    void *vtable;
} Obj9C7680;
MAIN1_SIZE(Obj9C7680, 0x18);
MAIN1_AT(Obj9C7680, active, 0x0);
MAIN1_AT(Obj9C7680, field_01, 0x1);
MAIN1_AT(Obj9C7680, field_04, 0x4);
MAIN1_AT(Obj9C7680, field_0C, 0xC);
MAIN1_AT(Obj9C7680, vtable, 0x14);


/* An actor class (base func_0883975C) with a second vtable D_089C7248 at +0x544. */
typedef struct Actor9C7248 {
    char pad_00[0x544];
    void *vtable_544;
    char pad_548[0x18];
    unsigned int flags_560;
    char pad_564[0xC];
    Obj9C7690 field_570;
    char pad_5A8[0x8];
    Obj9C74C8 field_5B0;
    Obj9C7680 field_5DC;
} Actor9C7248;
MAIN1_AT(Actor9C7248, vtable_544, 0x544);
MAIN1_AT(Actor9C7248, flags_560, 0x560);
MAIN1_AT(Actor9C7248, field_570, 0x570);
MAIN1_AT(Actor9C7248, field_5B0, 0x5B0);
MAIN1_AT(Actor9C7248, field_5DC, 0x5DC);


void *func_0883975C(void *self, int kind, int arg);
void func_088310BC(void *self, int flags);
void func_0883133C(void *object);
void func_08814C94(Actor9C7248 *self);
void func_08814D08(Actor9C7248 *self);
Obj9C7690 *func_08822294(Obj9C7690 *self);
void func_088222CC(Obj9C7690 *self, unsigned int flags);
void func_08822324(Obj9C7690 *self);
void func_088223A8(Obj9C7690 *self);
int func_088227D0(Obj9C7690 *self);
Obj9C74C8 *func_08817C44(Obj9C74C8 *self);
void func_08817C7C(Obj9C74C8 *self, unsigned int flags);
void func_08817CD4(Obj9C74C8 *self);
void func_08817D18(Obj9C74C8 *self);
Obj9C7680 *func_088210D8(Obj9C7680 *self);
void func_08821110(Obj9C7680 *self, unsigned int flags);
void func_08821168(Obj9C7680 *self);
void func_0882117C(Obj9C7680 *self);
void func_088160D8(Actor9C7248 *self);

/* Vtable D_089C7520 at +0x14; reset by func_0881AFDC. */
typedef struct Obj9C7520 {
    char active;
    char pad_01[0x3];
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    void *vtable;
} Obj9C7520;
MAIN1_SIZE(Obj9C7520, 0x18);
MAIN1_AT(Obj9C7520, active, 0x0);
MAIN1_AT(Obj9C7520, field_04, 0x4);
MAIN1_AT(Obj9C7520, field_08, 0x8);
MAIN1_AT(Obj9C7520, field_0C, 0xC);
MAIN1_AT(Obj9C7520, field_10, 0x10);
MAIN1_AT(Obj9C7520, vtable, 0x14);


/* Vtable D_089C74B0 at +0x1C; holds an Obj9C7520. */
typedef struct Obj9C74B0 {
    char active;
    char pad_01[0x3];
    Obj9C7520 field_04;
    void *vtable;
} Obj9C74B0;
MAIN1_SIZE(Obj9C74B0, 0x20);
MAIN1_AT(Obj9C74B0, active, 0x0);
MAIN1_AT(Obj9C74B0, field_04, 0x4);
MAIN1_AT(Obj9C74B0, vtable, 0x1C);


void func_088167E8(Obj9C74B0 *self);
void func_088167F0(Obj9C74B0 *self);
Obj9C7520 *func_0881AF4C(Obj9C7520 *self);
void func_0881AF84(Obj9C7520 *self, unsigned int flags);
void func_0881AFDC(Obj9C7520 *self);
void func_0881AFF4(Obj9C7520 *self);
void func_08816EB8(void *self);
void func_088170C0(void *self);
void func_08817518(void *self);

/* Vtable D_089C74F8 at +8; reset by func_08819ED0. */
typedef struct Obj9C74F8 {
    int field_00;
    int field_04;
    void *vtable;
} Obj9C74F8;
MAIN1_SIZE(Obj9C74F8, 0xC);
MAIN1_AT(Obj9C74F8, field_00, 0x0);
MAIN1_AT(Obj9C74F8, field_04, 0x4);
MAIN1_AT(Obj9C74F8, vtable, 0x8);


/* Built by func_08818744; holds an Obj9C74F8 at +0x18. */
typedef struct Obj818744 {
    char pad_00[0x18];
    Obj9C74F8 field_18;
} Obj818744;
MAIN1_AT(Obj818744, field_18, 0x18);


/* Vtable D_089C74E8 at +0x10; reset by func_08819C1C. */
typedef struct Obj9C74E8 {
    unsigned char active;
    unsigned char field_01;
    char pad_02[0x2];
    int field_04[3];
    void *vtable;
} Obj9C74E8;
MAIN1_SIZE(Obj9C74E8, 0x14);
MAIN1_AT(Obj9C74E8, active, 0x0);
MAIN1_AT(Obj9C74E8, field_01, 0x1);
MAIN1_AT(Obj9C74E8, field_04, 0x4);
MAIN1_AT(Obj9C74E8, vtable, 0x10);


/* Vtable D_089C7510 at +0x24; reset by func_0881A25C. */
typedef struct Obj9C7510 {
    char active;
    char pad_01[0x3];
    int field_04;
    void **field_08;
    int field_0C;
    struct Entry81A9C8 *field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    void *vtable;
} Obj9C7510;
MAIN1_SIZE(Obj9C7510, 0x28);
MAIN1_AT(Obj9C7510, active, 0x0);
MAIN1_AT(Obj9C7510, field_04, 0x4);
MAIN1_AT(Obj9C7510, field_08, 0x8);
MAIN1_AT(Obj9C7510, field_0C, 0xC);
MAIN1_AT(Obj9C7510, field_10, 0x10);
MAIN1_AT(Obj9C7510, field_14, 0x14);
MAIN1_AT(Obj9C7510, field_18, 0x18);
MAIN1_AT(Obj9C7510, field_1C, 0x1C);
MAIN1_AT(Obj9C7510, field_20, 0x20);
MAIN1_AT(Obj9C7510, vtable, 0x24);


/* One 12-byte entry of Obj9C7510's field_10 table. */
typedef struct Entry81A9C8 {
    int field_00;
    int field_04;
    unsigned char field_08;
    char pad_09[0x3];
} Entry81A9C8;
MAIN1_SIZE(Entry81A9C8, 0xC);
MAIN1_AT(Entry81A9C8, field_00, 0x0);
MAIN1_AT(Entry81A9C8, field_04, 0x4);
MAIN1_AT(Entry81A9C8, field_08, 0x8);


/* (kind, index) looked up by func_0881A9C8. */
typedef struct Ref81A9C8 {
    int kind;
    int index;
} Ref81A9C8;
MAIN1_SIZE(Ref81A9C8, 0x8);
MAIN1_AT(Ref81A9C8, kind, 0x0);
MAIN1_AT(Ref81A9C8, index, 0x4);


/* A node of Obj9C7530's list at +0x110. */
typedef struct SysNode {
    struct SysNode *next;
} SysNode;
MAIN1_AT(SysNode, next, 0x0);


/* Vtable D_089C7530 at +0x148; the static instance is D_08B632A0. */
typedef struct Obj9C7530 {
    char pad_00[0x8];
    char field_08;
    char pad_09[0x3];
    char field_0C[8];
    Obj9C74B0 field_14;
    Obj9C7510 field_34;
    Obj9C74E8 field_5C;
    char pad_70[0xA0];
    struct SysNode *first;
    char pad_114[0x34];
    void *vtable;
} Obj9C7530;
MAIN1_SIZE(Obj9C7530, 0x14C);
MAIN1_AT(Obj9C7530, field_08, 0x8);
MAIN1_AT(Obj9C7530, field_0C, 0xC);
MAIN1_AT(Obj9C7530, field_14, 0x14);
MAIN1_AT(Obj9C7530, field_34, 0x34);
MAIN1_AT(Obj9C7530, field_5C, 0x5C);
MAIN1_AT(Obj9C7530, first, 0x110);
MAIN1_AT(Obj9C7530, vtable, 0x148);


Obj9C74B0 *func_08816744(Obj9C74B0 *self);
void func_08816784(Obj9C74B0 *self, unsigned int flags);
void func_088187D0(Obj818744 *self);
void func_08818818(Obj818744 *self);
void *func_08818848(unsigned int size);
void func_08818864(void *object);
Obj9C74F8 *func_08819E40(Obj9C74F8 *self);
void func_08819E78(Obj9C74F8 *self, unsigned int flags);
void func_08819ED0(Obj9C74F8 *self);
void func_08819F00(Obj9C74F8 *self);
Obj9C74E8 *func_08819B8C(Obj9C74E8 *self);
void func_08819BC4(Obj9C74E8 *self, unsigned int flags);
void func_08819C1C(Obj9C74E8 *self);
void func_08819C44(Obj9C74E8 *self);
Obj9C7510 *func_0881A1CC(Obj9C7510 *self);
void func_0881A204(Obj9C7510 *self, unsigned int flags);
void func_0881A25C(Obj9C7510 *self);
void func_0881A284(Obj9C7510 *self);
unsigned int func_0881A3C8(Obj9C7510 *self, unsigned int arg1, unsigned int arg2);
void func_0881B558(Obj9C7530 *self);
void func_0881B61C(Obj9C7530 *self);
Obj9C7530 *func_0881B39C(Obj9C7530 *self);
Obj9C7530 *func_0881BAA8(void);
SysNode *func_0881CB80(SysNode *prev);
void func_088191A0(SysNode *node);
extern int D_08B63290;           /* set once D_08B632A0 is built */
extern Obj9C7530 D_08B632A0;
extern int D_089D4900;

/* Vtable D_089C7548 at +0xFC; seventeen member objects; reset by func_0881E1F0. */
typedef struct Obj9C7548 {
    char active;
    char pad_01[0x3];
    int field_04;
    int field_08;
    VPair field_0C;
    VQuad field_18;
    VQuad field_2C;
    VPair field_40;
    VPair field_4C;
    VPair field_58[4];
    VPair field_88;
    VQuad field_94;
    VPair field_A8;
    VPair field_B4;
    VPair field_C0[2];
    VPair field_D8;
    VPair field_E4[2];
    void *vtable;
} Obj9C7548;
MAIN1_SIZE(Obj9C7548, 0x100);
MAIN1_AT(Obj9C7548, active, 0x0);
MAIN1_AT(Obj9C7548, field_04, 0x4);
MAIN1_AT(Obj9C7548, field_08, 0x8);
MAIN1_AT(Obj9C7548, field_0C, 0xC);
MAIN1_AT(Obj9C7548, field_18, 0x18);
MAIN1_AT(Obj9C7548, field_2C, 0x2C);
MAIN1_AT(Obj9C7548, field_40, 0x40);
MAIN1_AT(Obj9C7548, field_4C, 0x4C);
MAIN1_AT(Obj9C7548, field_58, 0x58);
MAIN1_AT(Obj9C7548, field_88, 0x88);
MAIN1_AT(Obj9C7548, field_94, 0x94);
MAIN1_AT(Obj9C7548, field_A8, 0xA8);
MAIN1_AT(Obj9C7548, field_B4, 0xB4);
MAIN1_AT(Obj9C7548, field_C0, 0xC0);
MAIN1_AT(Obj9C7548, field_D8, 0xD8);
MAIN1_AT(Obj9C7548, field_E4, 0xE4);
MAIN1_AT(Obj9C7548, vtable, 0xFC);


void func_0881E1F0(Obj9C7548 *self);

/* ------------------------------------------------------------ the world (*D_089D4958) */

/* An object on the world's list at +0x14 (next at +0x1114). */
typedef struct Fighter14 {
    char pad_00[0x76C];
    char field_76C[4];
    char pad_770[0x9A4];
    struct Fighter14 *next;
} Fighter14;
MAIN1_AT(Fighter14, field_76C, 0x76C);
MAIN1_AT(Fighter14, next, 0x1114);


/* An object on the world's list at +0x24 (next at +0x5F0). */
typedef struct Obj24 {
    char pad_00[0x5D8];
    char field_5D8[4];
    char pad_5DC[0x14];
    struct Obj24 *next;
} Obj24;
MAIN1_AT(Obj24, field_5D8, 0x5D8);
MAIN1_AT(Obj24, next, 0x5F0);


/* An object on the world's list at +0x34 (next at +0x550). */
typedef struct Obj34 {
    char pad_00[0x550];
    struct Obj34 *next;
} Obj34;
MAIN1_AT(Obj34, next, 0x550);


/* The world: its heap and object lists (D_089D4958 points at it). */
typedef struct World9D4958 {
    void *field_00;
    char pad_04[0x4];
    void *heap;
    struct Actor *actors;
    char pad_10[0x4];
    struct Fighter14 *first_14;
    char pad_18[0x4];
    struct Chara *charas;
    char pad_20[0x4];
    struct Obj24 *first_24;
    char pad_28[0xC];
    struct Obj34 *first_34;
    char pad_38[0x14];
    int field_4C;
} World9D4958;
MAIN1_SIZE(World9D4958, 0x50);
MAIN1_AT(World9D4958, field_00, 0x0);
MAIN1_AT(World9D4958, heap, 0x8);
MAIN1_AT(World9D4958, actors, 0xC);
MAIN1_AT(World9D4958, first_14, 0x14);
MAIN1_AT(World9D4958, charas, 0x1C);
MAIN1_AT(World9D4958, first_24, 0x24);
MAIN1_AT(World9D4958, first_34, 0x34);
MAIN1_AT(World9D4958, field_4C, 0x4C);


extern World9D4958 *D_089D4958;
Actor *func_08841F64(Actor *prev);
Fighter14 *func_08841F84(Fighter14 *prev);
Chara *func_08842048(Chara *prev);
Obj24 *func_08842068(Obj24 *prev);
Obj34 *func_088420A8(Obj34 *prev);
void *func_08842100(unsigned int size);
void func_08842138(void *object);

/* Menu-overlay state released by func_088138AC / func_08813BB8. */
extern int D_08ABBDF8, D_08A80218, D_08AC8EF8;
void func_08ABAE78(int state);
void func_08ABAE5C(int state);
void func_08A801E4(int state);
void func_08AC6218(int state);

/* The table D_089D4960 points at: a block, a heap and a list of entries. */
typedef struct Table9D4960 {
    void *block;
    void *heap;
    struct Entry9D4960 *first;
} Table9D4960;
MAIN1_SIZE(Table9D4960, 0xC);
MAIN1_AT(Table9D4960, block, 0x0);
MAIN1_AT(Table9D4960, heap, 0x4);
MAIN1_AT(Table9D4960, first, 0x8);


extern Table9D4960 *D_089D4960;
void func_08844520(Table9D4960 *self, int id);

#endif /* DISSIDIA_MAIN_AUTO_REGION1_H */
