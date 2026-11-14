#ifndef DISSIDIA_BATTLE_AUTO_REGION1_H
#define DISSIDIA_BATTLE_AUTO_REGION1_H

/* Classes behind the automatically matched functions in battle
 * 0x089E7018..0x089FAC40 (readability pilot, region 1).
 *
 * "Part" structs are small C++ objects that keep their vtable pointer after
 * their data. "Unit" structs derive from the class built by func_08A157D0 (or
 * func_089F12F0) and keep the vtable at offset 0. Each struct is named after
 * the address of its vtable. A part's first byte is cleared by its reset,
 * set once it is set up and tested first by its getters, so it is called
 * `active`. Fields whose use is not plain keep their offset as their name.
 * Objects of classes outside this region are byte arrays named after the
 * function that builds them. */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------ runtime */

void func_08992F04(void *object);                                   /* operator delete */
void func_08993560(void *array, int count, int size, void *ctor);  /* build each element */
void func_0899372C(void *array, int count, int size, void *dtor, int arg4, int arg5);
void func_088DA060(void *resource);                                 /* release a resource */
void *func_08890900(unsigned int size);
void func_08890970(void *object);

/* Base of the unit classes: constructor, destructor, operator delete. */
void *func_08A157D0(void *self);
void func_08A157E4(void *self, int flags);
void func_08A15834(void *self);

/* ------------------------------------------------------------ parts */

/* 0x20 bytes. Release func_089E7450 frees field_08 and field_0C. */
typedef struct PartE7988 {
    unsigned char active;
    char pad_01[3];
    unsigned int field_04;
    void *field_08;
    void *field_0C;
    int field_10;
    int field_14;
    int field_18;
    void *vtable;
} PartE7988;

/* 0x54 bytes. Release func_089E831C frees field_14[] and field_24[]. */
typedef struct PartE8D08 {
    char pad_00[8];
    unsigned char active;
    char pad_09[3];
    int state;
    char pad_10[4];
    void *field_14[4];
    void *field_24[4];
    char pad_34[0xC];
    void *field_40;     /* counter object that func_089DF854 adds field_48 to */
    char pad_44[4];
    int field_48;
    char pad_4C[4];
    void *vtable;
} PartE8D08;

/* 0x28 bytes. Reset func_089E8FF8, release func_089E9024. */
typedef struct PartE9568 {
    unsigned char active;
    char pad_01[3];
    float field_04;
    void *field_08;
    void *field_0C;
    void *field_10;
    void *field_14;
    unsigned int field_18;
    void *field_1C;
    void *field_20;
    void *vtable;
} PartE9568;

/* 0x10B8 bytes. Reset func_089E9EF4, release func_089E9F04. */
typedef struct PartEA518 {
    unsigned char active;
    char pad_01[3];
    void *field_04;
    unsigned char block[0x10];          /* built by func_0887A61C */
    void *field_18;
    unsigned char entries[2][0x84C];    /* built by func_089EA530 */
    void *vtable;
} PartEA518;

/* 0x30 bytes. Reset func_089EB804. */
typedef struct PartEB850 {
    int field_00;
    int field_04;
    unsigned char field_08;
    unsigned char field_09;
    char pad_0A[2];
    int field_0C;
    int field_10[3];
    unsigned char field_1C;
    char pad_1D[3];
    int field_20;
    unsigned char field_24;
    unsigned char field_25;
    char pad_26[2];
    int field_28;
    void *vtable;
} PartEB850;

/* 0x10 bytes. Release func_089EB904 frees field_08. */
typedef struct PartEBB88 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    void *field_08;
    void *vtable;
} PartEBB88;

/* 0x78 bytes. Release func_089EBCC8. */
typedef struct PartECED0 {
    char pad_00[8];
    unsigned char active;
    char pad_09[3];
    int state;
    char pad_10[0x14];
    unsigned char field_24;
    char pad_25[8];
    unsigned char field_2D;
    char pad_2E[0xA];
    float field_38;
    char pad_3C[8];
    PartEB850 field_44;
    void *vtable;
} PartECED0;

/* 0x4C bytes. Release func_089ED974 frees field_10..field_38. */
typedef struct PartEE950 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    int field_0C;
    void *field_10;
    void *field_14;
    void *field_18;
    void *field_1C;
    void *field_20;
    void *field_24;
    void *field_28;
    void *field_2C;
    void *field_30;
    void *field_34;
    void *field_38;
    int field_3C;
    int field_40;
    int field_44;
    void *vtable;
} PartEE950;

/* 0x0C bytes. Also the layout of the classes with vtables D_089EF198,
 * D_089EF1A8, D_089EF1B8 and D_089EF1C8. */
typedef struct PartEF188 {
    int field_00;
    int field_04;
    void *vtable;
} PartEF188;

/* 0x70 bytes. */
typedef struct PartEF178 {
    unsigned char active;
    char pad_01[3];
    unsigned char field_04[0x14];       /* built by func_08A21C40 */
    PartEF188 cells[7];
    void *vtable;
} PartEF178;

/* 0x28 bytes. Reset func_089EF320, release func_089EF350. */
typedef struct PartEFC58 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    float field_0C;
    int field_10;
    float field_14;
    int field_18;
    int field_1C;
    int field_20;
    void *vtable;
} PartEFC58;

/* 0x14 bytes. Reset func_089F0F40, release func_089F0F58. */
typedef struct PartF12D8 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    float field_0C;
    void *vtable;
} PartF12D8;

/* 0x14 bytes. Reset func_089F1C68, release func_089F1C80. */
typedef struct PartF1FE0 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    int field_0C;
    void *vtable;
} PartF1FE0;

/* 0x20 bytes. Reset func_089F7CD8, release func_089F7D0C frees field_04[]. */
typedef struct PartF81A8 {
    unsigned char active;
    unsigned char field_01;
    unsigned char field_02;
    char pad_03;
    void *field_04[4];
    int field_14;
    int field_18;
    void *vtable;
} PartF81A8;

/* 0x16C bytes. Reset func_089F20C4, release func_089F2108. */
typedef struct PartF2908 {
    unsigned char active;
    char pad_01[3];
    void *field_04;
    PartF81A8 field_08;
    PartF81A8 field_28;
    char pad_48[0x120];
    void *vtable;
} PartF2908;

/* 0x140 bytes (arrays of it use that stride). Reset func_089F34BC,
 * release func_089F34D4. */
typedef struct PartF3820 {
    unsigned char active;
    char pad_01[3];
    void *field_04;
    PartF81A8 field_08;
    char pad_28[0x108];
    int field_130;
    int field_134;
    void *vtable;
    char pad_13C[4];
} PartF3820;

/* 0x1C bytes. Reset func_089F38E8, release func_089F3908 frees
 * field_04..field_0C. */
typedef struct PartF3E68 {
    unsigned char active;
    char pad_01[3];
    void *field_04;
    void *field_08;
    void *field_0C;
    int field_10;
    int field_14;
    void *vtable;
} PartF3E68;

/* 0x48 bytes. Release func_089F5098 frees field_04 and field_08[][]. */
typedef struct PartF5628 {
    unsigned char active;
    char pad_01[3];
    void *field_04;
    void *field_08[4][3];
    char pad_38[0xC];
    void *vtable;
} PartF5628;

/* 0x108 bytes. Reset func_089F57C8, release func_089F5808. */
typedef struct PartF5E58 {
    unsigned char active;
    char pad_01[3];
    PartF5628 field_04[3];
    int field_DC[3];
    int field_E8[3];
    int field_F4;
    int field_F8;
    int field_FC;
    unsigned char field_100;
    char pad_101[3];
    void *vtable;
} PartF5E58;

/* 0x48 bytes. Reset func_089F5F14, release func_089F5F44. */
typedef struct PartF63E0 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    int field_0C;
    PartF81A8 field_10;
    unsigned char field_30;
    char pad_31[3];
    float field_34;
    int field_38;
    int field_3C;
    float field_40;
    void *vtable;
} PartF63E0;

/* 0x938 bytes. Reset func_089F6488, release func_089F6490. */
typedef struct PartF65E0 {
    unsigned char active;
    char pad_01[3];
    unsigned char field_04[0x930];      /* built by func_08AEAD20 */
    void *vtable;
} PartF65E0;

/* 0x38 bytes. Reset func_089F82AC, release func_089F82C8. */
typedef struct PartF8670 {
    unsigned char active;
    char pad_01[3];
    int field_04;
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    int field_10;
    PartF81A8 field_14;
    void *vtable;
} PartF8670;

/* 0x90 bytes. Reset func_089F8710, release func_089F8788. */
typedef struct PartF8E08 {
    unsigned char active;
    char pad_01[0x8B];
    void *vtable;
} PartF8E08;

/* Subobject at +0x1DC of the battle root. */
typedef struct RootPart1DC {
    char pad_00[4];
    unsigned char field_04[0x854];
    unsigned char field_858[8];         /* tested by func_089E6FDC */
} RootPart1DC;

/* Entry returned by func_089E95B0 and the object it points to. */
typedef struct LookupStats {
    char pad_00[0x18];
    float field_18;
} LookupStats;

typedef struct LookupEntry {
    char pad_000[0x52C];
    LookupStats *field_52C;
} LookupEntry;

/* ------------------------------------------------------------ units */

/* Built by func_089EFC90. Reset func_089EFD80, release func_089EFDB8. */
typedef struct UnitF0C68 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    char pad_14[8];
    PartEFC58 field_1C;
    PartF8670 field_44;
    unsigned char field_7C[0x98];       /* built by func_08A0E398 */
    unsigned char field_114[0x34];      /* built by func_08A08178 */
} UnitF0C68;

/* Base of UnitF1B20 and UnitFAAC0: constructor func_089F12F0, destructor
 * func_089F1304, operator new func_089F1338, operator delete func_089F1354. */
typedef struct UnitF13D8 {
    void *vtable;
} UnitF13D8;

/* Built by func_089F1488. Reset func_089F153C, release func_089F1564. */
typedef struct UnitF1B20 {
    UnitF13D8 base;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    PartF1FE0 field_14;
    int field_28;
    int field_2C;
    int field_30;
} UnitF1B20;

/* Built by func_089F2990. Reset func_089F2A8C, release func_089F2AC0. */
typedef struct UnitF3350 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    int field_14;
    int field_18;
    unsigned char field_1C[0x2C];       /* built by func_08A10AC8 */
    PartF8670 field_48;
    PartF2908 field_80;
    char pad_1EC[4];
    unsigned char field_1F0[0x34];      /* built by func_08A08178 */
    int field_224;
    int field_228;
    int field_22C;
    int field_230;
} UnitF3350;

/* Built by func_089F3E88. Reset func_089F3FF8, release func_089F404C. */
typedef struct UnitF4ED0 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    char pad_14[0xC];
    PartF3E68 field_20;
    unsigned char field_3C[0x2C];       /* built by func_08A10AC8 */
    PartF8670 field_68[2];
    char pad_D8[8];
    PartF3820 field_E0[2];
    unsigned char field_360[2][0x34];   /* built by func_08A08178 */
} UnitF4ED0;

/* Built by func_089F65F0. Reset func_089F66CC, release func_089F6700. */
typedef struct UnitF7040 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    unsigned char field_14;
    char pad_15[3];
    int field_18;
    unsigned char field_1C[0x2C];       /* built by func_08A10AC8 */
    unsigned char field_48[0x34];       /* built by func_08A08178 */
    PartF8670 field_7C;
    int field_B4;
    int field_B8;
    int field_BC;
    int field_C0;
} UnitF7040;

/* Built by func_089F7108; same members as UnitF7040. Reset func_089F71E4,
 * release func_089F7218. */
typedef struct UnitF7B80 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    char pad_14[8];
    unsigned char field_1C[0x2C];       /* built by func_08A10AC8 */
    unsigned char field_48[0x34];       /* built by func_08A08178 */
    PartF8670 field_7C;
} UnitF7B80;

/* Built by func_089F8E18. Reset func_089F8F18, release func_089F8F48. */
typedef struct UnitF97A8 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    int field_14;
    PartF8E08 field_18;
    PartF8670 field_A8;
    unsigned char field_E0[2][0x34];    /* built by func_08A08178 */
    int field_148;
    int field_14C;
    int field_150;
    int field_154;
} UnitF97A8;

/* Built by func_089F9948. Reset func_089F9A4C, release func_089F9A78. */
typedef struct UnitFA2C8 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    unsigned char field_14[0x28];       /* built by func_08A01788 */
    unsigned char field_3C[0x30];       /* built by func_08A16758 */
    unsigned char field_6C[0x24];       /* built by func_08A01DD0 */
    unsigned char field_90[0x3C];       /* built by func_08A006A8 */
    unsigned char field_CC[0x34];       /* built by func_08A08178 */
} UnitFA2C8;

/* Built by func_089FA3D0 on the UnitF13D8 base. Reset func_089FA4A8,
 * release func_089FA4C8. */
typedef struct UnitFAAC0 {
    UnitF13D8 base;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    unsigned char field_10[2][0x118];   /* built by func_08A174F0 */
    int field_240;
    int field_244;
    int field_248;
} UnitFAAC0;

/* Built by func_089FAC40. Reset func_089FAD30, release func_089FAD64. */
typedef struct UnitFB568 {
    void *vtable;
    unsigned char field_04;
    char pad_05[3];
    int field_08;
    unsigned char field_0C;
    char pad_0D[3];
    float field_10;
    int field_14;
    int field_18;
    unsigned char field_1C[0x2C];       /* built by func_08A10AC8 */
    PartF8670 field_48;
    PartF63E0 field_80;
    unsigned char field_C8[0x34];       /* built by func_08A08178 */
    int field_FC;
    int field_100;
    int field_104;
    int field_108;
} UnitFB568;

/* ------------------------------------------------------------ vtables */

extern unsigned char D_089E7988[];
extern unsigned char D_089E8D08[];
extern unsigned char D_089E9568[];
extern unsigned char D_089EA518[];
extern unsigned char D_089EB850[];
extern unsigned char D_089EBB88[];
extern unsigned char D_089ECED0[];
extern unsigned char D_089EE950[];
extern unsigned char D_089EF178[];
extern unsigned char D_089EF188[];
extern unsigned char D_089EF198[];
extern unsigned char D_089EF1A8[];
extern unsigned char D_089EF1B8[];
extern unsigned char D_089EF1C8[];
extern unsigned char D_089EFC58[];
extern unsigned char D_089F0C68[];
extern unsigned char D_089F12D8[];
extern unsigned char D_089F13D8[];
extern unsigned char D_089F1B20[];
extern unsigned char D_089F1FE0[];
extern unsigned char D_089F2908[];
extern unsigned char D_089F3350[];
extern unsigned char D_089F3820[];
extern unsigned char D_089F3E68[];
extern unsigned char D_089F4ED0[];
extern unsigned char D_089F5628[];
extern unsigned char D_089F5E58[];
extern unsigned char D_089F63E0[];
extern unsigned char D_089F65E0[];
extern unsigned char D_089F7040[];
extern unsigned char D_089F7B80[];
extern unsigned char D_089F81A8[];
extern unsigned char D_089F8670[];
extern unsigned char D_089F8E08[];
extern unsigned char D_089F97A8[];
extern unsigned char D_089FA2C8[];
extern unsigned char D_089FAAC0[];
extern unsigned char D_089FB568[];

/* ------------------------------------------------------------ functions */

/* PartE7988 */
unsigned int func_089E7018(PartE7988 *self);
void func_089E73D8(PartE7988 *self, int flags);
void func_089E7430(PartE7988 *self);
void func_089E7450(PartE7988 *self);
int func_089E77B8(PartE7988 *self);

/* RootPart1DC */
int func_089E6FDC(void *holder);
int func_089E8028(RootPart1DC *self);
void *func_089E8044(RootPart1DC *self);
void *func_089E804C(RootPart1DC *self);

/* PartE8D08 */
void func_089E8270(PartE8D08 *self, int flags);
void func_089E831C(PartE8D08 *self);
unsigned char func_089E856C(PartE8D08 *self);
unsigned char func_089E8594(PartE8D08 *self);
int func_089E85BC(PartE8D08 *self);
void *func_089E85DC(PartE8D08 *self);
int func_089E8CA8(PartE8D08 *self);
int func_089DF854(void *counter, int amount);

/* int cell */
void func_089E8EE8(int *cell, int value);
int func_089E8F50(int *cell);

/* PartE9568 */
void func_089E8FA0(PartE9568 *self, int flags);
void func_089E9024(PartE9568 *self);

/* Lookup */
LookupEntry *func_089E95B0(unsigned int index);
float func_089E97FC(unsigned int index);

/* PartEA518 */
PartEA518 *func_089E9E10(PartEA518 *self);
void func_089E9E68(PartEA518 *self, int flags);
void func_089E9EF4(PartEA518 *self);
void func_089E9F04(PartEA518 *self);
void func_089EA40C(PartEA518 *self);
int func_089EA448(void);
void *func_089EA530(void *entry);
void func_089EA5F0(void *entry, int flags);
void func_0887A61C(void *block);
void func_0887A644(void *block, int flags);
unsigned char func_0887AFB0(void *block);
void func_0887AD1C(void *block);

/* PartEB850 */
PartEB850 *func_089EB798(PartEB850 *self);
void func_089EB7D0(PartEB850 *self, int flags);
void func_089EB804(PartEB850 *self);

/* PartEBB88 */
PartEBB88 *func_089EB860(PartEBB88 *self);
void func_089EB898(PartEBB88 *self, int flags);
void func_089EB8F0(PartEBB88 *self);
void func_089EB904(PartEBB88 *self);

/* PartECED0 */
void func_089EBBF0(PartECED0 *self, int flags);
void func_089EBCC8(PartECED0 *self);
unsigned char func_089EC004(PartECED0 *self);
unsigned char func_089EC210(PartECED0 *self);
unsigned char func_089EC238(PartECED0 *self);
unsigned char func_089EC260(PartECED0 *self);
unsigned char func_089EC288(PartECED0 *self);
unsigned char func_089EC2B0(PartECED0 *self);
unsigned char func_089EC2F8(PartECED0 *self);
unsigned char func_089EC318(PartECED0 *self);
unsigned char func_089EC338(PartECED0 *self);
int func_089EC73C(PartECED0 *self);

/* PartEE950 */
PartEE950 *func_089ED898(PartEE950 *self);
void func_089ED8D0(PartEE950 *self, int flags);
void func_089ED928(PartEE950 *self);
void func_089ED974(PartEE950 *self);

/* PartEF178 */
PartEF178 *func_089EEA00(PartEF178 *self);
void func_089EECCC(PartEF178 *self);
void func_089EECD4(PartEF178 *self);
void *func_08A21C40(void *object);
void func_08A21CE4(void *object);

/* PartEFC58 */
PartEFC58 *func_089EF290(PartEFC58 *self);
void func_089EF2C8(PartEFC58 *self, int flags);
void func_089EF320(PartEFC58 *self);
void func_089EF350(PartEFC58 *self);
unsigned char func_089EF7E4(PartEFC58 *self);

/* UnitF0C68 */
UnitF0C68 *func_089EFC90(UnitF0C68 *self);
void func_089EFCEC(UnitF0C68 *self, int flags);
void func_089EFD80(UnitF0C68 *self);
void func_089EFDB8(UnitF0C68 *self);
int func_089F0C24(UnitF0C68 *self);
int func_089F0E90(void);
void func_08AFCA78(void);

/* PartF12D8 */
PartF12D8 *func_089F0EB0(PartF12D8 *self);
void func_089F0EE8(PartF12D8 *self, int flags);
void func_089F0F40(PartF12D8 *self);
void func_089F0F58(PartF12D8 *self);
unsigned char func_089F1274(PartF12D8 *self);

/* UnitF13D8 */
UnitF13D8 *func_089F12F0(UnitF13D8 *self);
void func_089F1304(UnitF13D8 *self, int flags);
void *func_089F1338(unsigned int size);
void func_089F1354(void *object);

/* UnitF1B20 */
UnitF1B20 *func_089F1488(UnitF1B20 *self);
void func_089F14CC(UnitF1B20 *self, int flags);
void func_089F153C(UnitF1B20 *self);
void func_089F1564(UnitF1B20 *self);
int func_089F1ADC(UnitF1B20 *self);

/* PartF1FE0 */
PartF1FE0 *func_089F1BD8(PartF1FE0 *self);
void func_089F1C10(PartF1FE0 *self, int flags);
void func_089F1C68(PartF1FE0 *self);
void func_089F1C80(PartF1FE0 *self);
unsigned char func_089F1FB8(PartF1FE0 *self);

/* PartF2908 */
PartF2908 *func_089F2008(PartF2908 *self);
void func_089F20C4(PartF2908 *self);
void func_089F2108(PartF2908 *self);

/* UnitF3350 */
UnitF3350 *func_089F2990(UnitF3350 *self);
void func_089F2A8C(UnitF3350 *self);
int func_089F3270(UnitF3350 *self);
int func_089F3330(UnitF3350 *self);

/* PartF3820 */
PartF3820 *func_089F3418(PartF3820 *self);
void func_089F3458(PartF3820 *self, int flags);
void func_089F34BC(PartF3820 *self);
void func_089F34D4(PartF3820 *self);
unsigned char func_089F37F8(PartF3820 *self);

/* PartF3E68 */
PartF3E68 *func_089F3858(PartF3E68 *self);
void func_089F3890(PartF3E68 *self, int flags);
void func_089F38E8(PartF3E68 *self);
void func_089F3908(PartF3E68 *self);

/* UnitF4ED0 */
UnitF4ED0 *func_089F3E88(UnitF4ED0 *self);
void func_089F3F1C(UnitF4ED0 *self, int flags);
void func_089F3FF8(UnitF4ED0 *self);
void func_089F404C(UnitF4ED0 *self);
int func_089F4E20(UnitF4ED0 *self);

/* PartF5628 */
PartF5628 *func_089F4FB8(PartF5628 *self);
void func_089F4FF0(PartF5628 *self, int flags);
void func_089F5098(PartF5628 *self);
unsigned char func_089F5604(PartF5628 *self);

/* PartF5E58 */
PartF5E58 *func_089F5700(PartF5E58 *self);
void func_089F5750(PartF5E58 *self, int flags);
void func_089F57C8(PartF5E58 *self);
void func_089F5808(PartF5E58 *self);
unsigned char func_089F5C54(PartF5E58 *self);

/* PartF63E0 */
PartF63E0 *func_089F5E70(PartF63E0 *self);
void func_089F5EB0(PartF63E0 *self, int flags);
void func_089F5F14(PartF63E0 *self);
void func_089F5F44(PartF63E0 *self);
unsigned char func_089F6360(PartF63E0 *self);

/* PartF65E0 */
PartF65E0 *func_089F63F0(PartF65E0 *self);
void func_089F6430(PartF65E0 *self, int flags);
void func_089F6488(PartF65E0 *self);
void func_089F6490(PartF65E0 *self);
int func_089F64AC(PartF65E0 *self);
void *func_08AEAD20(void *object);
void func_08AEAF84(void *object, int arg1, int arg2, int arg3, int arg4, int arg5);

/* UnitF7040 */
UnitF7040 *func_089F65F0(UnitF7040 *self);
void func_089F6644(UnitF7040 *self, int flags);
void func_089F66CC(UnitF7040 *self);
void func_089F6700(UnitF7040 *self);
int func_089F701C(UnitF7040 *self);

/* UnitF7B80 */
UnitF7B80 *func_089F7108(UnitF7B80 *self);
void func_089F715C(UnitF7B80 *self, int flags);
void func_089F71E4(UnitF7B80 *self);
void func_089F7218(UnitF7B80 *self);
int func_089F7B5C(UnitF7B80 *self);

/* PartF81A8 */
PartF81A8 *func_089F7C48(PartF81A8 *self);
void func_089F7C80(PartF81A8 *self, int flags);
void func_089F7CD8(PartF81A8 *self);
void func_089F7D0C(PartF81A8 *self);
unsigned char func_089F8184(PartF81A8 *self);

/* PartF8670 */
PartF8670 *func_089F8208(PartF8670 *self);
void func_089F8248(PartF8670 *self, int flags);
void func_089F82AC(PartF8670 *self);
void func_089F82C8(PartF8670 *self);
unsigned char func_089F8644(PartF8670 *self);

/* PartF8E08 */
PartF8E08 *func_089F8680(PartF8E08 *self);
void func_089F86B8(PartF8E08 *self, int flags);
void func_089F8710(PartF8E08 *self);
void func_089F8788(PartF8E08 *self);

/* UnitF97A8 */
UnitF97A8 *func_089F8E18(UnitF97A8 *self);
void func_089F8E7C(UnitF97A8 *self, int flags);
void func_089F8F18(UnitF97A8 *self);
void func_089F8F48(UnitF97A8 *self);
int func_089F96E4(UnitF97A8 *self);
int func_089F9784(UnitF97A8 *self);

/* UnitFA2C8 */
UnitFA2C8 *func_089F9948(UnitFA2C8 *self);
void func_089F99AC(UnitFA2C8 *self, int flags);
void func_089F9A4C(UnitFA2C8 *self);
void func_089F9A78(UnitFA2C8 *self);
int func_089FA210(UnitFA2C8 *self);
int func_089FA2A8(UnitFA2C8 *self);
unsigned char func_08A01A9C(void *object);

/* UnitFAAC0 */
UnitFAAC0 *func_089FA3D0(UnitFAAC0 *self);
void func_089FA424(UnitFAAC0 *self, int flags);
void func_089FA4A8(UnitFAAC0 *self);
void func_089FA4C8(UnitFAAC0 *self);
int func_089FAAA0(UnitFAAC0 *self);

/* UnitFB568 */
UnitFB568 *func_089FAC40(UnitFB568 *self);
void func_089FAD30(UnitFB568 *self);

/* Constructors and destructors of member classes outside this region. */
void *func_08A0E398(void *self);
void func_08A0E3D8(void *self, int flags);
void *func_08A08178(void *self);
void func_08A081B8(void *self, int flags);
void *func_08A10AC8(void *self);
void func_08A10B00(void *self, int flags);
void *func_08A01788(void *self);
void func_08A017C0(void *self, int flags);
void *func_08A16758(void *self);
void func_08A16790(void *self, int flags);
void *func_08A01DD0(void *self);
void func_08A01E08(void *self, int flags);
void *func_08A006A8(void *self);
void func_08A006E0(void *self, int flags);
void *func_08A174F0(void *self);
void func_08A17540(void *self, int flags);

#ifdef __cplusplus
}
#endif

#endif
