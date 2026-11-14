#ifndef DISSIDIA_MENU_AUTO_REGION4_H
#define DISSIDIA_MENU_AUTO_REGION4_H

/* Classes behind the automatically matched functions in menu
 * 0x08AF7760..0x08B58F64 (readability pass, menu region 4: the tail of the
 * overlay).
 *
 * The range is a run of task screens (a 0x100-byte block, a MenuTask at
 * +0x100, the task built by func_08864FF0 and handed its callbacks by
 * func_0886503C) and their panels. Each screen keeps its state in a work
 * block reached through one pointer field (+0x138, +0x13C, +0x14C ...)
 * that holds a layout, the model handles the scripts drive, texts and the
 * messages object, as in the earlier regions.
 *
 * Structs are named after the singleton pointer that holds the screen
 * (ScreenB1CB00 lives in D_08B1CB00) or after their vtable or first
 * function here. ScreenB01CF8X / WorkB01CF8X are the fuller layout of
 * region 3's ScreenB01CF8 (whose WorkB01CF8 lacks the fields used here and
 * starts its texts at texts[0x39]); functions declared with ScreenB01CF8 in
 * earlier headers keep it. Handles and texts are indexed from the work
 * block's arrays (handles[0xE2] is the handle at work + 0x404 + 0xE2 * 4).
 * Fields whose use is not plain keep their offset as their name. Types and
 * prototypes of auto_region1.h .. auto_region3.h are reused.
 *
 * Generated from a field spec: pads and assertions are written by script. */

#include "menu/auto_region3.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MENU4_OFFSET(type, field) ((unsigned int)&((type *)0)->field)
#define MENU4_ASSERT(name, cond) typedef char menu4_assert_##name[(cond) ? 1 : -1]
#define MENU4_SIZE(type, size) MENU4_ASSERT(size_##type, sizeof(type) == (size))
#define MENU4_AT(type, field, offset) \
    MENU4_ASSERT(at_##type##_##field, MENU4_OFFSET(type, field) == (offset))

/* ------------------------------------------------------------ shared pieces */

/* The full context of the panel callbacks func_08AF5C34..func_08AF63CC
 * (region 3's PanelCtx5D20 names only the fields its functions read).
 * field_00 .. field_08 select the panel; the scripts at +0x44 .. +0x60
 * are played as it opens and closes. */
typedef struct PanelCtx {
    int field_00;
    int field_04;
    int field_08;
    MenuLayout *layout;
    MenuModel **handles;
    void **texts;
    char pad_18[0x2C];
    const short *field_44; /* closing layout script */
    const void *field_48;
    const void *field_4C;
    const void *field_50;
    const unsigned short *field_54; /* closing text script */
    int field_58;
    const void *field_5C;
    const void *field_60;
} PanelCtx;
MENU4_AT(PanelCtx, field_00, 0x0);
MENU4_AT(PanelCtx, field_04, 0x4);
MENU4_AT(PanelCtx, field_08, 0x8);
MENU4_AT(PanelCtx, layout, 0xC);
MENU4_AT(PanelCtx, handles, 0x10);
MENU4_AT(PanelCtx, texts, 0x14);
MENU4_AT(PanelCtx, field_44, 0x44);
MENU4_AT(PanelCtx, field_48, 0x48);
MENU4_AT(PanelCtx, field_4C, 0x4C);
MENU4_AT(PanelCtx, field_50, 0x50);
MENU4_AT(PanelCtx, field_54, 0x54);
MENU4_AT(PanelCtx, field_58, 0x58);
MENU4_AT(PanelCtx, field_5C, 0x5C);
MENU4_AT(PanelCtx, field_60, 0x60);

/* Block handed to func_08AF9880 / func_08AF9934 / func_08AF99CC: a row of
 * model handles, the selected index and a state. */
typedef struct Block9880 {
    char pad_00[0x4];
    MenuModel **handles;
    char pad_08[0xC];
    int index;
    char pad_18[0xDC];
    int state;
    char field_F8;
} Block9880;
MENU4_AT(Block9880, handles, 0x4);
MENU4_AT(Block9880, index, 0x14);
MENU4_AT(Block9880, state, 0xF4);
MENU4_AT(Block9880, field_F8, 0xF8);

/* Record returned by func_088BB88C. */
typedef struct RecordBB88C {
    char pad_00[0xD];
    unsigned char field_0D;
} RecordBB88C;
MENU4_AT(RecordBB88C, field_0D, 0xD);

/* ------------------------------------------------------------ ScreenB01CF8 (region 3), fuller layout */

/* Work block of ScreenB01CF8 (+0x140), with the fields used here. */
typedef struct WorkB01CF8X {
    void *field_00;
    int state; /* 3 / 4 and 1 / 7 tested by func_08AF9F1C / func_08AF9F68 */
    char pad_08[0xC];
    void *field_14; /* a block from the screen's heap */
    void *field_18; /* released through D_089D6998 */
    MenuLayout layout;
    MenuModel *handles[0x120];
    void *messages;
    void *field_8A0; /* a second messages object */
    int field_8A4;
    void *texts[0x5D];
    char pad_A1C[0x124];
    unsigned int flags;
    char pad_B44[0x20];
    void *field_B64; /* a block from the screen's heap */
    char pad_B68[0xA0];
    PanelAF08A4 *panel;
    char pad_C0C[0x20];
    int field_C2C;
    int field_C30;
    char pad_C34[0x21A4];
    MenuPrompt prompt;
    unsigned short field_2E04;
    char pad_2E06[0x32];
    MenuCursorView field_2E38;
    MenuCursor field_2E64;
    int field_2E90;
    char pad_2E94[0x10];
} WorkB01CF8X;
MENU4_SIZE(WorkB01CF8X, 0x2EA4);
MENU4_AT(WorkB01CF8X, field_00, 0x0);
MENU4_AT(WorkB01CF8X, state, 0x4);
MENU4_AT(WorkB01CF8X, field_14, 0x14);
MENU4_AT(WorkB01CF8X, field_18, 0x18);
MENU4_AT(WorkB01CF8X, layout, 0x1C);
MENU4_AT(WorkB01CF8X, handles, 0x41C);
MENU4_AT(WorkB01CF8X, messages, 0x89C);
MENU4_AT(WorkB01CF8X, field_8A0, 0x8A0);
MENU4_AT(WorkB01CF8X, field_8A4, 0x8A4);
MENU4_AT(WorkB01CF8X, texts, 0x8A8);
MENU4_AT(WorkB01CF8X, flags, 0xB40);
MENU4_AT(WorkB01CF8X, field_B64, 0xB64);
MENU4_AT(WorkB01CF8X, panel, 0xC08);
MENU4_AT(WorkB01CF8X, field_C2C, 0xC2C);
MENU4_AT(WorkB01CF8X, field_C30, 0xC30);
MENU4_AT(WorkB01CF8X, prompt, 0x2DD8);
MENU4_AT(WorkB01CF8X, field_2E04, 0x2E04);
MENU4_AT(WorkB01CF8X, field_2E38, 0x2E38);
MENU4_AT(WorkB01CF8X, field_2E64, 0x2E64);
MENU4_AT(WorkB01CF8X, field_2E90, 0x2E90);

/* ScreenB01CF8 with the fuller work block. */
typedef struct ScreenB01CF8X {
    unsigned char block[0x100];
    MenuTask task;
    char pad_134[0x4];
    void *heap;
    char pad_13C[0x4];
    WorkB01CF8X *work;
} ScreenB01CF8X;
MENU4_SIZE(ScreenB01CF8X, 0x144);
MENU4_AT(ScreenB01CF8X, block, 0x0);
MENU4_AT(ScreenB01CF8X, task, 0x100);
MENU4_AT(ScreenB01CF8X, heap, 0x138);
MENU4_AT(ScreenB01CF8X, work, 0x140);

/* Object read by func_08AFA878. */
typedef struct ObjAFA878 {
    char pad_00[0x24];
    int field_24;
} ObjAFA878;
MENU4_AT(ObjAFA878, field_24, 0x24);

/* ------------------------------------------------------------ 0x08B02144..0x08B0461C */

/* Work block of the object behind func_08B02144 / func_08B02268 (+0x67C). */
typedef struct Work02144 {
    MenuLayout layout;
    MenuModel *handles[0x53];
    void *texts[0x3B];
    char pad_638[0x4];
    int field_63C;
    int field_640;
} Work02144;
MENU4_AT(Work02144, layout, 0x0);
MENU4_AT(Work02144, handles, 0x400);
MENU4_AT(Work02144, texts, 0x54C);
MENU4_AT(Work02144, field_63C, 0x63C);
MENU4_AT(Work02144, field_640, 0x640);

typedef struct Screen02144 {
    char pad_00[0x67C];
    Work02144 *work;
} Screen02144;
MENU4_SIZE(Screen02144, 0x680);
MENU4_AT(Screen02144, work, 0x67C);

/* ------------------------------------------------------------ 0x08B04CB8..0x08B10424: ScreenB1CB00 */

/* Work block of the screen in D_08B1CB00 (+0x138). handles[0xE2..0xE6] and
 * texts[0x110..0x114] are a row of five picked by field_24768;
 * handles[0xF8..0xFC] / [0xFD..0x101] a second row picked by field_24814. */
typedef struct WorkB1CB00 {
    char pad_00[0x4];
    MenuLayout layout;
    MenuModel *handles[0x120];
    void *texts[0x115];
    char pad_CD8[0x1E7A8];
    unsigned short field_1F480;
    char pad_1F482[0x3E];
    unsigned char field_1F4C0[4]; /* length not known */
    char pad_1F4C4[0x208];
    Block9880 field_1F6CC;
    char pad_1F7C8[0xD0];
    int state;
    unsigned int flags;
    char pad_1F8A0[0xC];
    unsigned char field_1F8AC[4]; /* length not known */
    char pad_1F8B0[0x104];
    int field_1F9B4;
    int field_1F9B8;
    int field_1F9BC;
    char pad_1F9C0[0x4];
    int field_1F9C4;
    char pad_1F9C8[0x54];
    int field_1FA1C;
    int field_1FA20;
    char pad_1FA24[0x4D44];
    int field_24768;
    char pad_2476C[0x24];
    MenuCursorView field_24790;
    MenuCursorView field_247BC;
    char pad_247E8[0x2C];
    int field_24814;
    unsigned char field_24818;
    char pad_24819[0x1A3];
    unsigned char field_249BC[4]; /* length not known */
    char pad_249C0[0x200];
    unsigned char field_24BC0[4]; /* length not known */
    char pad_24BC4[0x8C];
    unsigned char field_24C50[0x34];
    void *field_24C84;
    char pad_24C88[0x8];
    MenuCallback field_24C90;
    MenuCallback field_24C94;
} WorkB1CB00;
MENU4_AT(WorkB1CB00, layout, 0x4);
MENU4_AT(WorkB1CB00, handles, 0x404);
MENU4_AT(WorkB1CB00, texts, 0x884);
MENU4_AT(WorkB1CB00, field_1F480, 0x1F480);
MENU4_AT(WorkB1CB00, field_1F4C0, 0x1F4C0);
MENU4_AT(WorkB1CB00, field_1F6CC, 0x1F6CC);
MENU4_AT(WorkB1CB00, state, 0x1F898);
MENU4_AT(WorkB1CB00, flags, 0x1F89C);
MENU4_AT(WorkB1CB00, field_1F8AC, 0x1F8AC);
MENU4_AT(WorkB1CB00, field_1F9B4, 0x1F9B4);
MENU4_AT(WorkB1CB00, field_1F9B8, 0x1F9B8);
MENU4_AT(WorkB1CB00, field_1F9BC, 0x1F9BC);
MENU4_AT(WorkB1CB00, field_1F9C4, 0x1F9C4);
MENU4_AT(WorkB1CB00, field_1FA1C, 0x1FA1C);
MENU4_AT(WorkB1CB00, field_1FA20, 0x1FA20);
MENU4_AT(WorkB1CB00, field_24768, 0x24768);
MENU4_AT(WorkB1CB00, field_24790, 0x24790);
MENU4_AT(WorkB1CB00, field_247BC, 0x247BC);
MENU4_AT(WorkB1CB00, field_24814, 0x24814);
MENU4_AT(WorkB1CB00, field_24818, 0x24818);
MENU4_AT(WorkB1CB00, field_249BC, 0x249BC);
MENU4_AT(WorkB1CB00, field_24BC0, 0x24BC0);
MENU4_AT(WorkB1CB00, field_24C50, 0x24C50);
MENU4_AT(WorkB1CB00, field_24C84, 0x24C84);
MENU4_AT(WorkB1CB00, field_24C90, 0x24C90);
MENU4_AT(WorkB1CB00, field_24C94, 0x24C94);

/* Screen in D_08B1CB00 (task screen). */
typedef struct ScreenB1CB00 {
    unsigned char block[0x100];
    MenuTask task;
    char pad_134[0x4];
    WorkB1CB00 *work;
} ScreenB1CB00;
MENU4_SIZE(ScreenB1CB00, 0x13C);
MENU4_AT(ScreenB1CB00, block, 0x0);
MENU4_AT(ScreenB1CB00, task, 0x100);
MENU4_AT(ScreenB1CB00, work, 0x138);

/* ------------------------------------------------------------ 0x08B1CB58..0x08B1F598: ScreenB22E98 */

/* Work block of the screen in D_08B22E98 (+0x138). */
typedef struct WorkB22E98 {
    char pad_00[0x4];
    MenuLayout layout;
    MenuModel *handles[0x120];
    void *texts[1]; /* length not known */
    char pad_888[0x7790];
    unsigned char field_8018[0x34];
    unsigned char field_804C[0x34];
    int field_8080[2]; /* field_00 of the panel context, picked by field_80F8 */
    PanelCtx ctx;
    char pad_80EC[0xC];
    int field_80F8;
} WorkB22E98;
MENU4_AT(WorkB22E98, layout, 0x4);
MENU4_AT(WorkB22E98, handles, 0x404);
MENU4_AT(WorkB22E98, texts, 0x884);
MENU4_AT(WorkB22E98, field_8018, 0x8018);
MENU4_AT(WorkB22E98, field_804C, 0x804C);
MENU4_AT(WorkB22E98, field_8080, 0x8080);
MENU4_AT(WorkB22E98, ctx, 0x8088);
MENU4_AT(WorkB22E98, field_80F8, 0x80F8);

typedef struct ScreenB22E98 {
    unsigned char block[0x100];
    MenuTask task;
    char pad_134[0x4];
    WorkB22E98 *work;
} ScreenB22E98;
MENU4_SIZE(ScreenB22E98, 0x13C);
MENU4_AT(ScreenB22E98, block, 0x0);
MENU4_AT(ScreenB22E98, task, 0x100);
MENU4_AT(ScreenB22E98, work, 0x138);

/* ------------------------------------------------------------ 0x08B23300..0x08B23CF4: ScreenB243C0 */

typedef struct WorkB243C0 {
    char pad_00[0x4];
    MenuLayout layout;
    MenuModel *handles[0x120];
    void *texts[1]; /* length not known */
} WorkB243C0;
MENU4_AT(WorkB243C0, layout, 0x4);
MENU4_AT(WorkB243C0, handles, 0x404);
MENU4_AT(WorkB243C0, texts, 0x884);

/* Screen in D_08B243C0 (task screen). */
typedef struct ScreenB243C0 {
    unsigned char block[0x100];
    MenuTask task;
    char pad_134[0x4];
    WorkB243C0 *work;
} ScreenB243C0;
MENU4_SIZE(ScreenB243C0, 0x13C);
MENU4_AT(ScreenB243C0, block, 0x0);
MENU4_AT(ScreenB243C0, task, 0x100);
MENU4_AT(ScreenB243C0, work, 0x138);

/* ------------------------------------------------------------ 0x08B2459C..0x08B24E78: ScreenB25044 */

typedef struct WorkB25044 {
    MenuLayout layout;
    char pad_400[0x508];
    void *messages;
    int state;
} WorkB25044;
MENU4_AT(WorkB25044, layout, 0x0);
MENU4_AT(WorkB25044, messages, 0x908);
MENU4_AT(WorkB25044, state, 0x90C);

/* Screen in D_08B25044 (task screen). */
typedef struct ScreenB25044 {
    unsigned char block[0x100];
    MenuTask task;
    char pad_134[0x4];
    void *heap;
    WorkB25044 *work;
} ScreenB25044;
MENU4_SIZE(ScreenB25044, 0x140);
MENU4_AT(ScreenB25044, block, 0x0);
MENU4_AT(ScreenB25044, task, 0x100);
MENU4_AT(ScreenB25044, heap, 0x138);
MENU4_AT(ScreenB25044, work, 0x13C);

/* ------------------------------------------------------------ 0x08B255A0..0x08B28000: Panel26C88 */

/* Work block of the panel behind func_08B255A0 .. func_08B271C8 (its
 * first field). */
typedef struct Work26C88 {
    MenuLayout *layout;
    char pad_04[0xD0];
    MenuModel *field_D4;
    char pad_D8[0x10];
    void *messages;
    char pad_EC[0xC0];
    unsigned int flags;
    char pad_1B0[0xEC];
    short field_29C[1]; /* length not known */
    char pad_29E[0x2D16];
    void *texts[6];
    MenuPrompt prompt;
} Work26C88;
MENU4_AT(Work26C88, layout, 0x0);
MENU4_AT(Work26C88, field_D4, 0xD4);
MENU4_AT(Work26C88, messages, 0xE8);
MENU4_AT(Work26C88, flags, 0x1AC);
MENU4_AT(Work26C88, field_29C, 0x29C);
MENU4_AT(Work26C88, texts, 0x2FB4);
MENU4_AT(Work26C88, prompt, 0x2FCC);

typedef struct Panel26C88 {
    Work26C88 *work;
} Panel26C88;
MENU4_AT(Panel26C88, work, 0x0);

/* ------------------------------------------------------------ 0x08B28798..0x08B29788: ScreenB29CF8 */

typedef struct WorkB29CF8 {
    char pad_00[0x404];
    void *messages;
    char pad_408[0xC];
    int field_414; /* handed to func_08B2BA5C */
    char pad_418[0x168];
    void *field_580; /* a layout entry */
    char pad_584[0x3FC];
    MenuModel *field_980;
    char pad_984[0x47C];
    void *texts[6];
    MenuPrompt prompt;
} WorkB29CF8;
MENU4_AT(WorkB29CF8, messages, 0x404);
MENU4_AT(WorkB29CF8, field_414, 0x414);
MENU4_AT(WorkB29CF8, field_580, 0x580);
MENU4_AT(WorkB29CF8, field_980, 0x980);
MENU4_AT(WorkB29CF8, texts, 0xE00);
MENU4_AT(WorkB29CF8, prompt, 0xE18);

/* Screen in D_08B29CF8 (task screen). */
typedef struct ScreenB29CF8 {
    unsigned char block[0x100];
    MenuTask task;
    void *field_134; /* released with func_0888208C */
    void *heap;
    char pad_13C[0x10];
    WorkB29CF8 *work;
} ScreenB29CF8;
MENU4_SIZE(ScreenB29CF8, 0x150);
MENU4_AT(ScreenB29CF8, block, 0x0);
MENU4_AT(ScreenB29CF8, task, 0x100);
MENU4_AT(ScreenB29CF8, field_134, 0x134);
MENU4_AT(ScreenB29CF8, heap, 0x138);
MENU4_AT(ScreenB29CF8, work, 0x14C);

/* ------------------------------------------------------------ 0x08B29F3C..0x08B30838: three panels */

typedef int (*MenuIntCallback)(void);

/* Work block of Obj2F8F4 (+0x9D8): sixteen ids in a 2-column grid
 * (field_938 * 2 + field_964 picks one) and five picked rows. */
typedef struct Work2F8F4 {
    unsigned int flags;
    char pad_04[0xC];
    MenuModel *handles[0x120];
    void *texts[1]; /* length not known */
    char pad_494[0x430];
    Pair8 rows[5];
    int field_8EC; /* row picked */
    char pad_8F0[0x48];
    int field_938;
    char pad_93C[0x28];
    int field_964;
    char pad_968[0x28];
    int field_990[0x10];
    MenuIntCallback field_9D0;
} Work2F8F4;
MENU4_AT(Work2F8F4, flags, 0x0);
MENU4_AT(Work2F8F4, handles, 0x10);
MENU4_AT(Work2F8F4, texts, 0x490);
MENU4_AT(Work2F8F4, rows, 0x8C4);
MENU4_AT(Work2F8F4, field_8EC, 0x8EC);
MENU4_AT(Work2F8F4, field_938, 0x938);
MENU4_AT(Work2F8F4, field_964, 0x964);
MENU4_AT(Work2F8F4, field_990, 0x990);
MENU4_AT(Work2F8F4, field_9D0, 0x9D0);

typedef struct Obj2F8F4 {
    char pad_00[0x9D8];
    Work2F8F4 *work;
} Obj2F8F4;
MENU4_AT(Obj2F8F4, work, 0x9D8);

/* Block at Work2D604 +0x62C handed to func_0892A768 .. func_0892A7AC;
 * func_08B30804 copies the rows of Work2F8F4 into rows[i].field_04. */
typedef struct Block62C {
    char pad_00[0x20];
    Pair8 rows[5];
} Block62C;
MENU4_SIZE(Block62C, 0x48);
MENU4_AT(Block62C, rows, 0x20);

/* Work block of Obj2D604 (+0x844). */
typedef struct Work2D604 {
    unsigned int flags;
    int state;
    char pad_08[0x4];
    int field_0C;
    unsigned char field_10;
    char pad_11[0x7];
    MenuLayout *layout;
    MenuModel *handles[0x120];
    void *texts[1]; /* length not known */
    char pad_4A0[0x88];
    Obj2F8F4 *field_528;
    char pad_52C[0x38];
    void *field_564[5];
    char pad_578[0xB4];
    Block62C field_62C;
    MenuCursorView cursor;
    int field_6A0;
    char pad_6A4[0x20];
    void *field_6C4;
    char pad_6C8[0x168];
    int field_830;
} Work2D604;
MENU4_AT(Work2D604, flags, 0x0);
MENU4_AT(Work2D604, state, 0x4);
MENU4_AT(Work2D604, field_0C, 0xC);
MENU4_AT(Work2D604, field_10, 0x10);
MENU4_AT(Work2D604, layout, 0x18);
MENU4_AT(Work2D604, handles, 0x1C);
MENU4_AT(Work2D604, texts, 0x49C);
MENU4_AT(Work2D604, field_528, 0x528);
MENU4_AT(Work2D604, field_564, 0x564);
MENU4_AT(Work2D604, field_62C, 0x62C);
MENU4_AT(Work2D604, cursor, 0x674);
MENU4_AT(Work2D604, field_6A0, 0x6A0);
MENU4_AT(Work2D604, field_6C4, 0x6C4);
MENU4_AT(Work2D604, field_830, 0x830);

typedef struct Obj2D604 {
    char pad_00[0x844];
    Work2D604 *work;
} Obj2D604;
MENU4_AT(Obj2D604, work, 0x844);

/* Work block of Obj29F3C (+0xA18). */
typedef struct Work29F3C {
    unsigned int flags;
    int state;
    char pad_08[0x8];
    int field_10;
    char pad_14[0x4];
    MenuLayout *layout;
    MenuModel *handles[0x120];
    void *texts[1]; /* length not known */
    char pad_4A0[0x268];
    Obj2D604 *field_708;
    char pad_70C[0x38];
    MenuCursor cursor;
    unsigned char field_770[4]; /* length not known */
    char pad_774[0x18];
    unsigned char field_78C[0x90]; /* handed to func_08B58B64 */
    unsigned char field_81C[0x34];
    void *field_850;
    char pad_854[0x8];
    MenuCallback field_85C;
    MenuCallback field_860;
    int field_864;
    char pad_868[0x1A0];
    unsigned char field_A08;
} Work29F3C;
MENU4_AT(Work29F3C, flags, 0x0);
MENU4_AT(Work29F3C, state, 0x4);
MENU4_AT(Work29F3C, field_10, 0x10);
MENU4_AT(Work29F3C, layout, 0x18);
MENU4_AT(Work29F3C, handles, 0x1C);
MENU4_AT(Work29F3C, texts, 0x49C);
MENU4_AT(Work29F3C, field_708, 0x708);
MENU4_AT(Work29F3C, cursor, 0x744);
MENU4_AT(Work29F3C, field_770, 0x770);
MENU4_AT(Work29F3C, field_78C, 0x78C);
MENU4_AT(Work29F3C, field_81C, 0x81C);
MENU4_AT(Work29F3C, field_850, 0x850);
MENU4_AT(Work29F3C, field_85C, 0x85C);
MENU4_AT(Work29F3C, field_860, 0x860);
MENU4_AT(Work29F3C, field_864, 0x864);
MENU4_AT(Work29F3C, field_A08, 0xA08);

typedef struct Obj29F3C {
    char pad_00[0xA18];
    Work29F3C *work;
} Obj29F3C;
MENU4_AT(Obj29F3C, work, 0xA18);

/* Fields of D_089D712C past auto_region1.h's Global9D712C and region 3's
 * Global9D712CView. */
typedef struct Global9D712CView4 {
    char pad_00[0x10CC8];
    int field_10CC8;
    char pad_10CCC[0x2E87C];
    unsigned char field_3F548[4]; /* a table (func_089200D8 / func_089203BC) */
    char pad_3F54C[0xF0];
    unsigned char field_3F63C[4]; /* handed to func_08920978 */
} Global9D712CView4;
MENU4_AT(Global9D712CView4, field_10CC8, 0x10CC8);
MENU4_AT(Global9D712CView4, field_3F548, 0x3F548);
MENU4_AT(Global9D712CView4, field_3F63C, 0x3F63C);

/* MenuTask with the first word that func_08B3C554 reads. Declared as the
 * member type where it is read (a cast of the member address does not
 * fold the offset into the load); calls cast back to MenuTask. */
typedef struct MenuTaskView {
    int field_00;
    char pad_04[0x30];
} MenuTaskView;
MENU4_SIZE(MenuTaskView, 0x34);
MENU4_AT(MenuTaskView, field_00, 0x0);

/* ------------------------------------------------------------ 0x08B318F0..0x08B326F8: ScreenB32D90 */

typedef struct WorkB32D90 {
    char pad_00[0x40C];
    int field_40C; /* handed to func_08B2BA5C */
    char pad_410[0x164];
    void *field_574; /* a layout entry */
    char pad_578[0x3FC];
    MenuModel *field_974;
    char pad_978[0x47C];
    void *texts[6];
    MenuPrompt prompt;
} WorkB32D90;
MENU4_AT(WorkB32D90, field_40C, 0x40C);
MENU4_AT(WorkB32D90, field_574, 0x574);
MENU4_AT(WorkB32D90, field_974, 0x974);
MENU4_AT(WorkB32D90, texts, 0xDF4);
MENU4_AT(WorkB32D90, prompt, 0xE0C);

/* Screen in D_08B32D90 (task screen). */
typedef struct ScreenB32D90 {
    unsigned char block[0x100];
    MenuTask task;
    char pad_134[0x4];
    WorkB32D90 *work;
} ScreenB32D90;
MENU4_SIZE(ScreenB32D90, 0x13C);
MENU4_AT(ScreenB32D90, block, 0x0);
MENU4_AT(ScreenB32D90, task, 0x100);
MENU4_AT(ScreenB32D90, work, 0x138);

/* Class with its vtable at +0x544 (deleting destructor func_08B344E0;
 * base destructor func_088310BC). */
typedef struct Obj344E0 {
    char pad_00[0x544];
    void *vtable;
} Obj344E0;
MENU4_AT(Obj344E0, vtable, 0x544);

/* ------------------------------------------------------------ 0x08B34C18..0x08B3D760: ScreenB41CC0 */

/* What ScreenB41CC0.field_C4 points to: the state of the screen. */
typedef struct StateB41CC0 {
    int state;
    unsigned int flags;
    char pad_08[0x4];
    void *field_0C;
    void *field_10;
    void *field_14;
    void *field_18;
    char pad_1C[0xC];
    int field_28;
    char pad_2C[0x94];
    void *field_C0;
    char pad_C4[0x8];
    unsigned char field_CC;
} StateB41CC0;
MENU4_AT(StateB41CC0, state, 0x0);
MENU4_AT(StateB41CC0, flags, 0x4);
MENU4_AT(StateB41CC0, field_0C, 0xC);
MENU4_AT(StateB41CC0, field_10, 0x10);
MENU4_AT(StateB41CC0, field_14, 0x14);
MENU4_AT(StateB41CC0, field_18, 0x18);
MENU4_AT(StateB41CC0, field_28, 0x28);
MENU4_AT(StateB41CC0, field_C0, 0xC0);
MENU4_AT(StateB41CC0, field_CC, 0xCC);

/* What ScreenB41CC0.field_C8 points to: the parts the screen drives. */
typedef struct PartsB41CC0 {
    void *messages; /* released with func_0887B1F4 */
    char pad_04[0x8];
    void *field_0C;
    void *field_10;
    char pad_14[0x4];
    void *field_18;
    void *field_1C;
    char pad_20[0x4];
    void *field_24;
    char pad_28[0x8];
    void *field_30;
    void *field_34;
} PartsB41CC0;
MENU4_AT(PartsB41CC0, messages, 0x0);
MENU4_AT(PartsB41CC0, field_0C, 0xC);
MENU4_AT(PartsB41CC0, field_10, 0x10);
MENU4_AT(PartsB41CC0, field_18, 0x18);
MENU4_AT(PartsB41CC0, field_1C, 0x1C);
MENU4_AT(PartsB41CC0, field_24, 0x24);
MENU4_AT(PartsB41CC0, field_30, 0x30);
MENU4_AT(PartsB41CC0, field_34, 0x34);

/* Screen in D_08B41CC0 (constructor func_08B34C18; its task is at +0x5C). */
typedef struct ScreenB41CC0 {
    char pad_00[0xC];
    int field_0C;
    char pad_10[0x28];
    int field_38;
    int field_3C;
    char pad_40[0x4];
    unsigned int flags;
    char pad_48[0x10];
    void *field_58; /* released with func_08882158 */
    MenuTaskView task;
    int field_90;
    int field_94;
    MenuCallback field_98[3]; /* set by func_08B3CAF0 */
    void *field_A4; /* a block from the heap of func_08B42B40 */
    char pad_A8[0x4];
    int field_AC;
    char pad_B0[0x4];
    int field_B4;
    int field_B8;
    int field_BC;
    void *field_C0;
    StateB41CC0 *field_C4;
    PartsB41CC0 *field_C8;
} ScreenB41CC0;
MENU4_AT(ScreenB41CC0, field_0C, 0xC);
MENU4_AT(ScreenB41CC0, field_38, 0x38);
MENU4_AT(ScreenB41CC0, field_3C, 0x3C);
MENU4_AT(ScreenB41CC0, flags, 0x44);
MENU4_AT(ScreenB41CC0, field_58, 0x58);
MENU4_AT(ScreenB41CC0, task, 0x5C);
MENU4_AT(ScreenB41CC0, field_90, 0x90);
MENU4_AT(ScreenB41CC0, field_94, 0x94);
MENU4_AT(ScreenB41CC0, field_98, 0x98);
MENU4_AT(ScreenB41CC0, field_A4, 0xA4);
MENU4_AT(ScreenB41CC0, field_AC, 0xAC);
MENU4_AT(ScreenB41CC0, field_B4, 0xB4);
MENU4_AT(ScreenB41CC0, field_B8, 0xB8);
MENU4_AT(ScreenB41CC0, field_BC, 0xBC);
MENU4_AT(ScreenB41CC0, field_C0, 0xC0);
MENU4_AT(ScreenB41CC0, field_C4, 0xC4);
MENU4_AT(ScreenB41CC0, field_C8, 0xC8);

/* Screen in D_08B42088 (constructor func_08B41F00; its task is at +4). */
typedef struct ScreenB42088 {
    void *field_00; /* released with func_08882158 */
    MenuTask task;
    int field_38; /* the kind handed to func_08B3C900 */
} ScreenB42088;
MENU4_AT(ScreenB42088, field_00, 0x0);
MENU4_AT(ScreenB42088, task, 0x4);
MENU4_AT(ScreenB42088, field_38, 0x38);

/* ------------------------------------------------------------ 0x08B42234..0x08B43674: small classes */

typedef struct Obj42234 {
    char pad_00[0x4];
    unsigned char field_04;
} Obj42234;
MENU4_AT(Obj42234, field_04, 0x4);

/* A heap and a block taken from it (func_08B4229C .. func_08B423BC). */
typedef struct Block4229C {
    void *heap;
    int field_04;
    int field_08;
    void *block;
} Block4229C;
MENU4_SIZE(Block4229C, 0x10);
MENU4_AT(Block4229C, heap, 0x0);
MENU4_AT(Block4229C, field_04, 0x4);
MENU4_AT(Block4229C, field_08, 0x8);
MENU4_AT(Block4229C, block, 0xC);

typedef struct Ids4278C {
    short field_00;
    short field_02;
    short field_04;
    char pad_06[0xA];
    short field_10[0x20];
} Ids4278C;
MENU4_AT(Ids4278C, field_00, 0x0);
MENU4_AT(Ids4278C, field_02, 0x2);
MENU4_AT(Ids4278C, field_04, 0x4);
MENU4_AT(Ids4278C, field_10, 0x10);

/* A heap and one block from it (func_08B4280C / func_08B42A58,
 * func_08B43374 / func_08B43400, func_08B43674). */
typedef struct HeapBlock {
    void *heap;
    void *block;
} HeapBlock;
MENU4_SIZE(HeapBlock, 0x8);
MENU4_AT(HeapBlock, heap, 0x0);
MENU4_AT(HeapBlock, block, 0x4);

/* An object released with func_0888208C and a heap (func_08B42AA0 /
 * func_08B42B08); D_08B42C28 points to one. */
typedef struct Heap42AA0 {
    void *object;
    void *heap;
} Heap42AA0;
MENU4_SIZE(Heap42AA0, 0x8);
MENU4_AT(Heap42AA0, object, 0x0);
MENU4_AT(Heap42AA0, heap, 0x4);

/* Class with the vtable D_08B43260 (constructor func_08B42C30); the
 * fields past +8 are placed on it by address only. */
typedef struct Obj42C30 {
    void *vtable;
    int field_04;
    int field_08;
    char pad_0C[0x2426];
    unsigned short field_2432;
    char pad_2434[0xE];
    unsigned short field_2442;
    int field_2444;
} Obj42C30;
MENU4_AT(Obj42C30, vtable, 0x0);
MENU4_AT(Obj42C30, field_04, 0x4);
MENU4_AT(Obj42C30, field_08, 0x8);
MENU4_AT(Obj42C30, field_2432, 0x2432);
MENU4_AT(Obj42C30, field_2442, 0x2442);
MENU4_AT(Obj42C30, field_2444, 0x2444);

/* ------------------------------------------------------------ 0x08B436D8..0x08B45808: small classes */

typedef struct Name439C8 {
    short field_00;
    short field_02;
    char pad_04[0xA];
    char field_0E[0x22];
} Name439C8;
MENU4_SIZE(Name439C8, 0x30);
MENU4_AT(Name439C8, field_00, 0x0);
MENU4_AT(Name439C8, field_02, 0x2);
MENU4_AT(Name439C8, field_0E, 0xE);

/* Class with its vtable at +0xC (constructor func_08B43DD8). */
typedef struct Obj43DD8 {
    char pad_00[0x4];
    int field_04;
    unsigned int flags;
    void *vtable;
} Obj43DD8;
MENU4_AT(Obj43DD8, field_04, 0x4);
MENU4_AT(Obj43DD8, flags, 0x8);
MENU4_AT(Obj43DD8, vtable, 0xC);

typedef struct Obj43F00 {
    char pad_00[0x10];
    unsigned char field_10[4]; /* handed to func_089A34C0 (length not known) */
} Obj43F00;
MENU4_AT(Obj43F00, field_10, 0x10);

typedef struct Obj45414 {
    int field_00;
    int *field_04;
} Obj45414;
MENU4_SIZE(Obj45414, 0x8);
MENU4_AT(Obj45414, field_00, 0x0);
MENU4_AT(Obj45414, field_04, 0x4);

typedef struct Obj44024 {
    char pad_00[0x4];
    Obj45414 *field_04;
} Obj44024;
MENU4_AT(Obj44024, field_04, 0x4);

/* A heap (from func_08B42B40) and four words (func_08B4440C / func_08B44750). */
typedef struct List4440C {
    void *heap;
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
} List4440C;
MENU4_SIZE(List4440C, 0x14);
MENU4_AT(List4440C, heap, 0x0);
MENU4_AT(List4440C, field_04, 0x4);
MENU4_AT(List4440C, field_08, 0x8);
MENU4_AT(List4440C, field_0C, 0xC);
MENU4_AT(List4440C, field_10, 0x10);

typedef struct Obj44F38 {
    void *field_00;
} Obj44F38;
MENU4_SIZE(Obj44F38, 0x4);
MENU4_AT(Obj44F38, field_00, 0x0);

/* ------------------------------------------------------------ 0x08B45BB8..0x08B46E34: Net45BB8 */

/* The block Net45BB8.data points to (from the heap of func_08B42B40).
 * field_20 / field_24 / field_28 are four-byte objects handed by address
 * to their own functions (func_08B4A26C .., func_08B4863C ..,
 * func_08B45A98 ..); field_1C is the object of the func_08920xxx calls. */
typedef struct NetData45BB8 {
    char pad_00[0x4];
    int field_04;
    char pad_08[0xC];
    int field_14;
    int field_18;
    void *field_1C;
    void *field_20;
    void *field_24;
    void *field_28;
    unsigned char field_2C[4]; /* length not known */
    char pad_30[0x114];
    int field_144;
    int field_148;
    char pad_14C[0x4];
    int field_150;
    char pad_154[0xC];
    int field_160;
    int field_164; /* index into field_170 */
    int field_168;
    int field_16C;
    int field_170[1]; /* length not known */
} NetData45BB8;
MENU4_AT(NetData45BB8, field_04, 0x4);
MENU4_AT(NetData45BB8, field_14, 0x14);
MENU4_AT(NetData45BB8, field_18, 0x18);
MENU4_AT(NetData45BB8, field_1C, 0x1C);
MENU4_AT(NetData45BB8, field_20, 0x20);
MENU4_AT(NetData45BB8, field_24, 0x24);
MENU4_AT(NetData45BB8, field_28, 0x28);
MENU4_AT(NetData45BB8, field_2C, 0x2C);
MENU4_AT(NetData45BB8, field_144, 0x144);
MENU4_AT(NetData45BB8, field_148, 0x148);
MENU4_AT(NetData45BB8, field_150, 0x150);
MENU4_AT(NetData45BB8, field_160, 0x160);
MENU4_AT(NetData45BB8, field_164, 0x164);
MENU4_AT(NetData45BB8, field_168, 0x168);
MENU4_AT(NetData45BB8, field_16C, 0x16C);
MENU4_AT(NetData45BB8, field_170, 0x170);

/* Object built by func_08B45BB8 (the network object ScreenB41CC0's state
 * keeps in field_10). */
typedef struct Net45BB8 {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
    void *field_10; /* owner handed to func_088825D4 with field_14 */
    void *field_14;
    NetData45BB8 *data;
} Net45BB8;
MENU4_SIZE(Net45BB8, 0x1C);
MENU4_AT(Net45BB8, field_00, 0x0);
MENU4_AT(Net45BB8, field_04, 0x4);
MENU4_AT(Net45BB8, field_08, 0x8);
MENU4_AT(Net45BB8, field_0C, 0xC);
MENU4_AT(Net45BB8, field_10, 0x10);
MENU4_AT(Net45BB8, field_14, 0x14);
MENU4_AT(Net45BB8, data, 0x18);

/* ------------------------------------------------------------ 0x08B4A5CC..0x08B4AE74 */

typedef struct Data4AE44 {
    char pad_00[0x20];
    Pair8 *rows;
} Data4AE44;
MENU4_AT(Data4AE44, rows, 0x20);

typedef struct Obj4AE44 {
    Data4AE44 *data;
} Obj4AE44;
MENU4_AT(Obj4AE44, data, 0x0);

/* ------------------------------------------------------------ 0x08B4C010..0x08B50AB8: widgets */

/* The widgets below derive from one of two bases of the main module:
 *   func_0892ABC0 (reset func_0892AC2C): vtable at +0x24, the derived
 *     fields from +0x28 (state at +0x28, a model set id at +0x2C that
 *     func_088DAA68 makes handles from and func_088CF970 releases);
 *   func_08931D68 (reset func_08931DA0, func_08931E18 draws it): a model
 *     and an index at +4 / +8 whose matrix and colour the part copies,
 *     vtable at +0xC, the derived fields from +0x10.
 * Their scripts are played with no layout (func_088D3F00(0, handles, ...)). */
typedef struct Widget4C010 {
    char pad_00[0x24];
    void *vtable;
    int state;
    int id;
    MenuModel *handles[2];
    void *texts[2];
    int field_40;
    int field_44;
    int field_48;
    int field_4C;
    int field_50;
    int field_54;
} Widget4C010;
MENU4_SIZE(Widget4C010, 0x58);
MENU4_AT(Widget4C010, vtable, 0x24);
MENU4_AT(Widget4C010, state, 0x28);
MENU4_AT(Widget4C010, id, 0x2C);
MENU4_AT(Widget4C010, handles, 0x30);
MENU4_AT(Widget4C010, texts, 0x38);
MENU4_AT(Widget4C010, field_40, 0x40);
MENU4_AT(Widget4C010, field_44, 0x44);
MENU4_AT(Widget4C010, field_48, 0x48);
MENU4_AT(Widget4C010, field_4C, 0x4C);
MENU4_AT(Widget4C010, field_50, 0x50);
MENU4_AT(Widget4C010, field_54, 0x54);

typedef struct Widget4C7F8 {
    char pad_00[0x24];
    void *vtable;
    int state;
    int id;
    MenuModel *handles[1];
} Widget4C7F8;
MENU4_AT(Widget4C7F8, vtable, 0x24);
MENU4_AT(Widget4C7F8, state, 0x28);
MENU4_AT(Widget4C7F8, id, 0x2C);
MENU4_AT(Widget4C7F8, handles, 0x30);

typedef struct Part4CA98 {
    char pad_00[0x4];
    void *model;
    int index;
    void *vtable;
    MenuModel *handles[1];
    void *texts[3];
} Part4CA98;
MENU4_SIZE(Part4CA98, 0x20);
MENU4_AT(Part4CA98, model, 0x4);
MENU4_AT(Part4CA98, index, 0x8);
MENU4_AT(Part4CA98, vtable, 0xC);
MENU4_AT(Part4CA98, handles, 0x10);
MENU4_AT(Part4CA98, texts, 0x14);

typedef struct Part4CEDC {
    char pad_00[0x4];
    void *model;
    int index;
    void *vtable;
    MenuModel *handles[3];
    void *texts[4];
    unsigned short script[0x14]; /* text script played by func_08B4D464 */
} Part4CEDC;
MENU4_SIZE(Part4CEDC, 0x54);
MENU4_AT(Part4CEDC, model, 0x4);
MENU4_AT(Part4CEDC, index, 0x8);
MENU4_AT(Part4CEDC, vtable, 0xC);
MENU4_AT(Part4CEDC, handles, 0x10);
MENU4_AT(Part4CEDC, texts, 0x1C);
MENU4_AT(Part4CEDC, script, 0x2C);

typedef struct Part4D4DC {
    char pad_00[0x4];
    void *model;
    int index;
    void *vtable;
    MenuModel *handles[0xC];
    void *texts[0x13];
    int field_8C;
    int field_90;
    int field_94;
    int field_98;
    int field_9C;
    char pad_A0[0x2];
    unsigned char field_A2;
    char pad_A3[0x1];
} Part4D4DC;
MENU4_SIZE(Part4D4DC, 0xA4);
MENU4_AT(Part4D4DC, model, 0x4);
MENU4_AT(Part4D4DC, index, 0x8);
MENU4_AT(Part4D4DC, vtable, 0xC);
MENU4_AT(Part4D4DC, handles, 0x10);
MENU4_AT(Part4D4DC, texts, 0x40);
MENU4_AT(Part4D4DC, field_8C, 0x8C);
MENU4_AT(Part4D4DC, field_90, 0x90);
MENU4_AT(Part4D4DC, field_94, 0x94);
MENU4_AT(Part4D4DC, field_98, 0x98);
MENU4_AT(Part4D4DC, field_9C, 0x9C);
MENU4_AT(Part4D4DC, field_A2, 0xA2);

typedef struct Widget4DF58 {
    char pad_00[0x24];
    void *vtable;
    int state;
    int field_2C[2];
    MenuModel *handles[1];
    int field_38;
    int field_3C;
    unsigned char field_40[0x24]; /* a part built by func_08931E7C */
    Part4CA98 field_64;
    Part4CEDC field_84;
    Part4D4DC field_D8;
} Widget4DF58;
MENU4_AT(Widget4DF58, vtable, 0x24);
MENU4_AT(Widget4DF58, state, 0x28);
MENU4_AT(Widget4DF58, field_2C, 0x2C);
MENU4_AT(Widget4DF58, handles, 0x34);
MENU4_AT(Widget4DF58, field_38, 0x38);
MENU4_AT(Widget4DF58, field_3C, 0x3C);
MENU4_AT(Widget4DF58, field_40, 0x40);
MENU4_AT(Widget4DF58, field_64, 0x64);
MENU4_AT(Widget4DF58, field_84, 0x84);
MENU4_AT(Widget4DF58, field_D8, 0xD8);

/* Element of Widget4F194's two arrays (constructor func_08B4ECC8). */
typedef struct Elem4ECC8 {
    int state;
    MenuModel *handles[5];
    short script[0x14]; /* layout script played by func_08B4EFFC */
} Elem4ECC8;
MENU4_SIZE(Elem4ECC8, 0x40);
MENU4_AT(Elem4ECC8, state, 0x0);
MENU4_AT(Elem4ECC8, handles, 0x4);
MENU4_AT(Elem4ECC8, script, 0x18);

typedef struct Widget4F194 {
    char pad_00[0x24];
    void *vtable;
    char pad_28[0x4];
    int id;
    char pad_30[0x8];
    MenuModel *field_38;
    Elem4ECC8 field_3C[4];
    Elem4ECC8 field_13C[2];
} Widget4F194;
MENU4_AT(Widget4F194, vtable, 0x24);
MENU4_AT(Widget4F194, id, 0x2C);
MENU4_AT(Widget4F194, field_38, 0x38);
MENU4_AT(Widget4F194, field_3C, 0x3C);
MENU4_AT(Widget4F194, field_13C, 0x13C);

/* Element of Widget504DC's array (constructor func_08B4FE70, reset
 * func_08B4FE98): handles placed with the matrix of model's entry index. */
typedef struct Elem4FE98 {
    int state;
    MenuModel *handles[5];
    void *model;
    int index;
    int field_20;
} Elem4FE98;
MENU4_SIZE(Elem4FE98, 0x24);
MENU4_AT(Elem4FE98, state, 0x0);
MENU4_AT(Elem4FE98, handles, 0x4);
MENU4_AT(Elem4FE98, model, 0x18);
MENU4_AT(Elem4FE98, index, 0x1C);
MENU4_AT(Elem4FE98, field_20, 0x20);

typedef struct Widget504DC {
    char pad_00[0x24];
    void *vtable;
    char pad_28[0x4];
    int id;
    MenuModel *handles[2];
    Elem4FE98 field_38[5];
} Widget504DC;
MENU4_AT(Widget504DC, vtable, 0x24);
MENU4_AT(Widget504DC, id, 0x2C);
MENU4_AT(Widget504DC, handles, 0x30);
MENU4_AT(Widget504DC, field_38, 0x38);

/* ------------------------------------------------------------ 0x08B516E4..0x08B51FD4: Obj516E4 */

/* Object behind func_08B516E4 .. func_08B51FD4: five parts of the main
 * module (each built, run and released by its own func_088xxxxx group)
 * and a state (0 off, 1 .. 4 the step running). The part sizes are the
 * gaps between them. */
typedef struct Obj516E4 {
    char pad_00[0x4];
    void *field_04; /* owner handed to func_088825D4 */
    int field_08;
    int field_0C;
    char pad_10[0x1];
    unsigned char field_11;
    char pad_12[0x2];
    int state;
    char pad_18[0x4];
    unsigned char field_1C[0xA4]; /* func_088DE378 / func_088DE6D4 / func_088DE7B0 / func_088DE804 */
    unsigned char field_C0[0x27C]; /* func_088D6DBC / func_088D7574 / func_088D7684 */
    unsigned char field_33C[0x44]; /* func_088CD4C8 / func_088CDA34 / func_088CDB04 / func_088CDD7C */
    unsigned char field_380[0x14]; /* func_088CE3D8 / func_088CE724 / func_088CE72C / func_088CE7F4 */
    unsigned char field_394[4]; /* func_0892F5AC / func_0892F600 / func_0892F768 / func_0892F7D0 (length not known) */
} Obj516E4;
MENU4_AT(Obj516E4, field_04, 0x4);
MENU4_AT(Obj516E4, field_08, 0x8);
MENU4_AT(Obj516E4, field_0C, 0xC);
MENU4_AT(Obj516E4, field_11, 0x11);
MENU4_AT(Obj516E4, state, 0x14);
MENU4_AT(Obj516E4, field_1C, 0x1C);
MENU4_AT(Obj516E4, field_C0, 0xC0);
MENU4_AT(Obj516E4, field_33C, 0x33C);
MENU4_AT(Obj516E4, field_380, 0x380);
MENU4_AT(Obj516E4, field_394, 0x394);

/* ------------------------------------------------------------ 0x08B52288..0x08B53984: more widgets (base func_0892ABC0) */

typedef struct Widget52288 {
    char pad_00[0x24];
    void *vtable;
    int state;
    int id;
    MenuModel *handles[1];
} Widget52288;
MENU4_AT(Widget52288, vtable, 0x24);
MENU4_AT(Widget52288, state, 0x28);
MENU4_AT(Widget52288, id, 0x2C);
MENU4_AT(Widget52288, handles, 0x30);

typedef struct Widget525B0 {
    char pad_00[0x24];
    void *vtable;
    int state;
    int id;
    MenuModel *handles[3];
    void *texts[2];
    unsigned char field_44;
    unsigned char field_45;
    char pad_46[0x2];
    int field_48;
    int field_4C;
    int field_50;
} Widget525B0;
MENU4_SIZE(Widget525B0, 0x54);
MENU4_AT(Widget525B0, vtable, 0x24);
MENU4_AT(Widget525B0, state, 0x28);
MENU4_AT(Widget525B0, id, 0x2C);
MENU4_AT(Widget525B0, handles, 0x30);
MENU4_AT(Widget525B0, texts, 0x3C);
MENU4_AT(Widget525B0, field_44, 0x44);
MENU4_AT(Widget525B0, field_45, 0x45);
MENU4_AT(Widget525B0, field_48, 0x48);
MENU4_AT(Widget525B0, field_4C, 0x4C);
MENU4_AT(Widget525B0, field_50, 0x50);

typedef struct Widget52EB0 {
    char pad_00[0x24];
    void *vtable;
    int state;
    int id;
    MenuModel *handles[2];
    void *texts[10];
    int field_60; /* frames waited (up to 0x5A) */
} Widget52EB0;
MENU4_AT(Widget52EB0, vtable, 0x24);
MENU4_AT(Widget52EB0, state, 0x28);
MENU4_AT(Widget52EB0, id, 0x2C);
MENU4_AT(Widget52EB0, handles, 0x30);
MENU4_AT(Widget52EB0, texts, 0x38);
MENU4_AT(Widget52EB0, field_60, 0x60);

typedef struct Widget535C8 {
    char pad_00[0x24];
    void *vtable;
    char pad_28[0x4];
    int id;
    MenuModel *handles[2];
    void *texts[1]; /* length not known */
} Widget535C8;
MENU4_AT(Widget535C8, vtable, 0x24);
MENU4_AT(Widget535C8, id, 0x2C);
MENU4_AT(Widget535C8, handles, 0x30);
MENU4_AT(Widget535C8, texts, 0x38);

/* ------------------------------------------------------------ 0x08B53B88..0x08B5439C: ScreenB54058, ScreenB54C38 */

/* Screen in D_08B54058 (constructor func_08B53B88). */
typedef struct ScreenB54058 {
    unsigned char block[0x100];
    MenuTask task;
    int field_134;
    int field_138;
    int field_13C;
    unsigned char field_140;
    char pad_141[0x3];
} ScreenB54058;
MENU4_SIZE(ScreenB54058, 0x144);
MENU4_AT(ScreenB54058, block, 0x0);
MENU4_AT(ScreenB54058, task, 0x100);
MENU4_AT(ScreenB54058, field_134, 0x134);
MENU4_AT(ScreenB54058, field_138, 0x138);
MENU4_AT(ScreenB54058, field_13C, 0x13C);
MENU4_AT(ScreenB54058, field_140, 0x140);

/* Object built by func_08B54C40 (ScreenB54C38.field_13C). */
typedef struct Obj54C40 {
    unsigned char field_00[0x24];
    unsigned char field_24[4];
    unsigned char field_28[8]; /* built by func_0887B1E4 */
    int field_30;
    void *field_34;
    int field_38;
    int field_3C;
    char pad_40[0x120];
    int field_160;
    char pad_164[0x8];
    unsigned char field_16C;
    unsigned char field_16D;
    char pad_16E[0x16];
    int state;
} Obj54C40;
MENU4_AT(Obj54C40, field_00, 0x0);
MENU4_AT(Obj54C40, field_24, 0x24);
MENU4_AT(Obj54C40, field_28, 0x28);
MENU4_AT(Obj54C40, field_30, 0x30);
MENU4_AT(Obj54C40, field_34, 0x34);
MENU4_AT(Obj54C40, field_38, 0x38);
MENU4_AT(Obj54C40, field_3C, 0x3C);
MENU4_AT(Obj54C40, field_160, 0x160);
MENU4_AT(Obj54C40, field_16C, 0x16C);
MENU4_AT(Obj54C40, field_16D, 0x16D);
MENU4_AT(Obj54C40, state, 0x184);

/* Object embedded in ScreenB54C38 at +0x150: a vtable (D_08B54B80) and
 * the screen; func_08B54348 .. func_08B5439C forward to the screen. */
typedef struct Link54150 {
    void *vtable;
    struct ScreenB54C38 *owner;
} Link54150;
MENU4_SIZE(Link54150, 0x8);
MENU4_AT(Link54150, vtable, 0x0);
MENU4_AT(Link54150, owner, 0x4);

/* Screen in D_08B54C38 (constructor func_08B54060). */
typedef struct ScreenB54C38 {
    unsigned char block[0x100];
    MenuTask task;
    int field_134;
    int field_138;
    Obj54C40 *field_13C;
    int field_140;
    int field_144;
    unsigned int field_148;
    unsigned char field_14C;
    unsigned char field_14D;
    unsigned char field_14E;
    char pad_14F[0x1];
    Link54150 link;
    int field_158;
} ScreenB54C38;
MENU4_SIZE(ScreenB54C38, 0x15C);
MENU4_AT(ScreenB54C38, block, 0x0);
MENU4_AT(ScreenB54C38, task, 0x100);
MENU4_AT(ScreenB54C38, field_134, 0x134);
MENU4_AT(ScreenB54C38, field_138, 0x138);
MENU4_AT(ScreenB54C38, field_13C, 0x13C);
MENU4_AT(ScreenB54C38, field_140, 0x140);
MENU4_AT(ScreenB54C38, field_144, 0x144);
MENU4_AT(ScreenB54C38, field_148, 0x148);
MENU4_AT(ScreenB54C38, field_14C, 0x14C);
MENU4_AT(ScreenB54C38, field_14D, 0x14D);
MENU4_AT(ScreenB54C38, field_14E, 0x14E);
MENU4_AT(ScreenB54C38, link, 0x150);
MENU4_AT(ScreenB54C38, field_158, 0x158);

/* ------------------------------------------------------------ 0x08B550C8..0x08B58F64: small classes */

typedef struct Model550C8 {
    MenuModel *model;
} Model550C8;
MENU4_SIZE(Model550C8, 0x4);
MENU4_AT(Model550C8, model, 0x0);

typedef struct Obj55244 {
    unsigned char field_00[0x14];
    unsigned char field_14[0xC];
} Obj55244;
MENU4_SIZE(Obj55244, 0x20);
MENU4_AT(Obj55244, field_00, 0x0);
MENU4_AT(Obj55244, field_14, 0x14);

typedef struct Obj55734 {
    char pad_00[0x28];
    int field_28[1]; /* length not known */
} Obj55734;
MENU4_AT(Obj55734, field_28, 0x28);

typedef struct Obj558B0 {
    unsigned char field_00[8];
    unsigned char field_08[0x30];
} Obj558B0;
MENU4_SIZE(Obj558B0, 0x38);
MENU4_AT(Obj558B0, field_00, 0x0);
MENU4_AT(Obj558B0, field_08, 0x8);

typedef struct Obj563F0 {
    int field_00;
    unsigned char field_04[4];
    unsigned char field_08[0xC];
    int field_14;
    unsigned char field_18[0x28];
    int field_40;
    char pad_44[0x12C];
    int field_170;
} Obj563F0;
MENU4_SIZE(Obj563F0, 0x174);
MENU4_AT(Obj563F0, field_00, 0x0);
MENU4_AT(Obj563F0, field_04, 0x4);
MENU4_AT(Obj563F0, field_08, 0x8);
MENU4_AT(Obj563F0, field_14, 0x14);
MENU4_AT(Obj563F0, field_18, 0x18);
MENU4_AT(Obj563F0, field_40, 0x40);
MENU4_AT(Obj563F0, field_170, 0x170);

typedef struct Obj56CB0 {
    char pad_00[0x4];
    MenuModel *field_04;
    char pad_08[0x4];
    void *field_0C;
    void *field_10;
} Obj56CB0;
MENU4_AT(Obj56CB0, field_04, 0x4);
MENU4_AT(Obj56CB0, field_0C, 0xC);
MENU4_AT(Obj56CB0, field_10, 0x10);

typedef struct Obj5743C {
    MenuModel *field_00;
    char pad_04[0x4];
    void *field_08;
    char pad_0C[0x28];
    int field_34;
} Obj5743C;
MENU4_AT(Obj5743C, field_00, 0x0);
MENU4_AT(Obj5743C, field_08, 0x8);
MENU4_AT(Obj5743C, field_34, 0x34);

typedef struct Data9D7260 {
    char pad_00[0x2B80];
    unsigned char field_2B80;
} Data9D7260;
MENU4_AT(Data9D7260, field_2B80, 0x2B80);

/* What D_089D7260 points to (declared void * in auto_region1.h). */
typedef struct Global9D7260View {
    char pad_00[0x14];
    Data9D7260 *field_14;
} Global9D7260View;
MENU4_AT(Global9D7260View, field_14, 0x14);

/* The request func_08B58B64 hands to D_08B59230 (state at +0xC is region
 * 2's Data59230.field_0C). */
typedef struct Request59230 {
    unsigned int flags;
    char pad_04[0x8];
    int state;
} Request59230;
MENU4_SIZE(Request59230, 0x10);
MENU4_AT(Request59230, flags, 0x0);
MENU4_AT(Request59230, state, 0xC);

/* What D_08B59230 points to, with its request typed. */
typedef struct Obj59230X {
    Request59230 *request;
} Obj59230X;
MENU4_AT(Obj59230X, request, 0x0);

/* ------------------------------------------------------------ prototypes */

void func_088310BC(void *self, int flags);
void func_0883133C(void *self);
void func_08831360(void *self);
void func_08864DB8(void);
void func_0887B1F4(void *object, unsigned int arg);
void func_0887E6E0(void *owner, void *object);
void func_0887E6FC(void *owner);
unsigned char func_0887E718(void *owner);
void func_088965F4(int object, char arg);
void func_088A28E8(int arg1, char arg2);
void func_088A3F2C(char arg);
int func_088A9DE8(void *object, int arg1, int arg2, int arg3, int arg4);
void func_088ACA20(Global9D712C *object);
void func_088AFB08(void *object);
RecordBB88C *func_088BB88C(unsigned int id);
void func_088CD4C8(void *part);
int func_088CDA34(void *part);
int func_088CDB04(void *part);
void *func_088CDD7C(void *part);
void func_088CE3D8(void *part);
void *func_088CE724(void *part);
int func_088CE72C(void *part);
int func_088CE7F4(void *part);
void func_088CF970(void *owner, int id);
void func_088D4664(void *handles, void *texts, const unsigned short *script);
void func_088D6DBC(void *part);
int func_088D7574(void *part);
void *func_088D7684(void *part);
void func_088D9AF4(void *model, unsigned int *color);
void func_088DAB7C(int id, void *model);
void func_088DE378(void *part);
int func_088DE6D4(void *part, int arg1, int arg2, int arg3);
void func_088DE7B0(void *part);
void *func_088DE804(void *part);
int func_088FE438(void *object);
void func_088FE9CC(void);
void func_0891705C(void *object);
void func_08917BB4(void);
void func_08917BD4(void);
int *func_089200D8(void *table);
unsigned int *func_089200E0(void *table, int index);
void func_08920978(void *table);
void func_08920A3C(void *object, int arg);
void func_08920A7C(void *object, int arg);
int func_08920AC8(void *object, int arg);
void func_08920D18(void *object);
void func_089211C8(void *object);
int func_08921284(void *object);
int func_089212E8(void *object);
void func_08921374(void *object);
void func_08921728(void *object, int id);
void func_089231A8(void *object);
int func_089233C4(int id);
void func_089246C4(int id);
void func_08925574(void *row, unsigned char flag);
void func_08925624(void *row, unsigned char flag);
void func_0892A13C(void *info);
void func_0892A768(Block62C *block, int index, void *value, int arg);
int func_0892A790(Block62C *block);
int func_0892A7AC(Block62C *block);
void func_0892ABC0(void *self);
void func_0892AC2C(void *self);
void func_0892BD10(void *object);
void func_0892D2C0(void *object, void *messages, int kind, int value);
void func_0892D2DC(void *object, void *messages, int kind, int value);
void func_0892D380(void *object, void *messages, int kind, int value);
void func_0892F5AC(void *part);
void func_0892F600(void *part, int arg1, int arg2, int arg3, int arg4);
int func_0892F768(void *part);
void func_0892F7D0(void *part, void *info, int arg1, int arg2);
void func_08931D68(void *self);
void func_08931DA0(void *self);
void func_08931DB0(void *self);
void func_08931E18(void *self);
void func_08931E7C(void *self);
void func_08931EB8(void *self);
int func_0899D2F8(int value);
void func_089A12A4(int arg1, int arg2, int arg3, int arg4, int arg5);
void func_089A34C0(void *object);
void func_089DE910(void *object, char arg);
void func_089DE94C(void *object);
unsigned char func_08A81204(void *object);
void func_08A814D4(void *object, char arg1, int arg2);
void func_08ABC158(ScreenC4560 *self, int arg1, int arg2, int arg3);
void func_08ABC208(ScreenC4560 *self, int arg1, int arg2, int arg3, int arg4);
int func_08AF2470(ScreenB01CF8X *self, int arg);
int func_08AF251C(ScreenB01CF8X *self, int arg);
void func_08AF30BC(ScreenB01CF8X *self, int arg);
int func_08AF45E8(ScreenB01CF8X *self);
int func_08AF47E0(ScreenB01CF8X *self);
void func_08AF5C34(void *self, PanelCtx *ctx, short motion);
void func_08AF7654(MenuLayout *layout, MenuModel **handles, void *arg);
void func_08AF7760(ScreenB01CF8X *self);
void func_08AF7794(ScreenB01CF8X *self);
void func_08AF7BA8(ScreenB01CF8X *self);
void func_08AF7BFC(ScreenB01CF8X *self, int size);
void func_08AF7C74(ScreenB01CF8X *self);
void func_08AF7D94(ScreenB01CF8 *self);
void func_08AF7E5C(ScreenB01CF8 *self);
void func_08AF7ECC(ScreenB01CF8X *self);
void func_08AF7FF4(ScreenB01CF8X *self);
void func_08AF9CE0(void *self, short *values);
int func_08AF9F68(ScreenB01CF8X *self);
void func_08AFA074(ScreenB01CF8X *self);
void func_08AFA39C(ScreenB01CF8X *self);
void func_08AFA6DC(ScreenB01CF8X *self, int arg);
void func_08AFA7D0(int arg);
void func_08AFA7F4(int arg);
void func_08AFA818(int arg);
void func_08AFA83C(int arg);
int func_08AFA878(void *self, ObjAFA878 *object);
int func_08AFA880(ScreenB01CF8X *self);
void func_08AFAB28(ScreenB01CF8X *self);
void func_08AFAB94(ScreenB01CF8X *self);
void func_08AFABD4(ScreenB01CF8X *self);
void func_08AFAD60(ScreenB01CF8X *self);
void func_08AFAE60(ScreenB01CF8X *self);
void func_08AFB320(ScreenB01CF8X *self);
void func_08AFB44C(void);
void func_08AFB46C(int arg1, int arg2, int arg3, int arg4);
void func_08AFB4A0(void);
void func_08AFB4C0(int arg1, int arg2, int arg3, int arg4);
void func_08AFB4F8(void);
void func_08AFB524(int arg1, int arg2, int arg3, int arg4);
int func_08B02144(Screen02144 *self);
void func_08B02268(Screen02144 *self);
void func_08B02638(Screen02144 *self);
void func_08B02C20(void *self, int arg, char *text);
void func_08B02D34(void *self, int arg, int value, int flag);
void func_08B03584(Screen02144 *self, int arg1, int arg2);
void func_08B03718(PanelBase *self, unsigned int flags);
const short *func_08B045B8(void);
const short *func_08B045C4(void);
const short *func_08B04610(void);
const short *func_08B0461C(void);
void func_08B04CB8(void *self, int arg1, int arg2, int arg3, int arg4);
void func_08B05024(ScreenB1CB00 *self);
void func_08B052B8(ScreenB1CB00 *self);
void func_08B06110(ScreenB1CB00 *self);
void func_08B06958(ScreenB1CB00 *self);
void func_08B069A8(ScreenB1CB00 *self);
void func_08B06A38(ScreenB1CB00 *self);
void func_08B06AB4(ScreenB1CB00 *self);
void func_08B06AD4(ScreenB1CB00 *self);
unsigned char func_08B06BA0(ScreenB1CB00 *self);
void func_08B06FDC(ScreenB1CB00 *self);
void func_08B070B0(ScreenB1CB00 *self);
void func_08B0712C(ScreenB1CB00 *self);
void func_08B07220(ScreenB1CB00 *self);
void func_08B07274(ScreenB1CB00 *self);
void func_08B072A8(ScreenB1CB00 *self);
void func_08B072D4(ScreenB1CB00 *self);
void func_08B07760(ScreenB1CB00 *self);
void func_08B077BC(ScreenB1CB00 *self);
void func_08B07828(ScreenB1CB00 *self);
void func_08B0785C(ScreenB1CB00 *self, char arg);
void func_08B078F0(ScreenB1CB00 *self);
void func_08B07944(ScreenB1CB00 *self);
void func_08B07A44(ScreenB1CB00 *self);
void func_08B08D04(ScreenB1CB00 *self);
void func_08B0909C(ScreenB1CB00 *self);
void func_08B0910C(ScreenB1CB00 *self);
void func_08B09238(ScreenB1CB00 *self);
void func_08B092B8(ScreenB1CB00 *self);
void func_08B0973C(ScreenB1CB00 *self);
void func_08B09770(ScreenB1CB00 *self);
void func_08B097A4(ScreenB1CB00 *self);
void func_08B09A28(ScreenB1CB00 *self);
void func_08B0B63C(ScreenB1CB00 *self);
void func_08B0B964(void);
void func_08B0B984(void);
void func_08B0B9A4(void);
void func_08B0B9C4(ScreenB1CB00 *self);
void func_08B0B9E4(ScreenB1CB00 *self);
void func_08B0BBAC(ScreenB1CB00 *self);
void func_08B0BF70(ScreenB1CB00 *self);
void func_08B0BFA4(ScreenB1CB00 *self);
void func_08B0BFD8(ScreenB1CB00 *self);
ScreenB1CB00 *func_08B0C00C(ScreenB1CB00 *self);
void func_08B0C074(ScreenB1CB00 *self);
void func_08B0C1F0(ScreenB1CB00 *self);
void func_08B0C27C(ScreenB1CB00 *self, int arg);
void func_08B0C340(void);
void func_08B0C360(void);
void func_08B0C380(int arg);
void func_08B0D6E8(ScreenB1CB00 *self);
void func_08B0D710(ScreenB1CB00 *self);
void func_08B0D8CC(ScreenB1CB00 *self);
unsigned char func_08B0DF10(ScreenB1CB00 *self);
void func_08B0DF3C(ScreenB1CB00 *self);
void func_08B0DFD0(ScreenB1CB00 *self);
int func_08B0E058(ScreenB1CB00 *self);
void func_08B0E084(ScreenB1CB00 *self);
void func_08B0E0DC(ScreenB1CB00 *self);
void func_08B0E1B4(ScreenB1CB00 *self);
void func_08B0E1D4(ScreenB1CB00 *self);
void func_08B0E32C(ScreenB1CB00 *self);
void func_08B0E384(ScreenB1CB00 *self);
void func_08B0E53C(ScreenB1CB00 *self);
int func_08B0E630(ScreenB1CB00 *self);
void func_08B0F1D4(ScreenB1CB00 *self, char arg);
void func_08B0F3BC(void);
void func_08B0F470(ScreenB1CB00 *self);
void func_08B0F48C(ScreenB1CB00 *self);
void func_08B0F4A8(void);
void func_08B0F60C(ScreenB1CB00 *self);
void func_08B0F660(ScreenB1CB00 *self);
void func_08B0F6B0(ScreenB1CB00 *self);
void func_08B0FA00(ScreenB1CB00 *self);
unsigned char func_08B0FBD8(ScreenB1CB00 *self);
void func_08B0FF54(ScreenB1CB00 *self, int index, void *out);
int func_08B100F0(ScreenB1CB00 *self);
void func_08B10248(ScreenB1CB00 *self);
void func_08B102F0(ScreenB1CB00 *self);
void func_08B103A4(ScreenB1CB00 *self);
void func_08B103E4(void);
void func_08B10404(void);
void *func_08B10424(void);
void func_08B11D58(ScreenB1CB00 *self);
void func_08B13A84(ScreenB1CB00 *self);
void func_08B197C0(ScreenB1CB00 *self);
void func_08B1B1A4(ScreenB1CB00 *self, int arg);
void func_08B1CB58(void *self, int arg1, int arg2, int arg3, int arg4);
void func_08B1CB8C(void *self, int id);
void func_08B1CBB4(ScreenB22E98 *self);
void func_08B1CBF8(ScreenB22E98 *self, int arg);
void func_08B1CCB0(ScreenB22E98 *self);
void func_08B1CDF0(ScreenB22E98 *self);
void func_08B1CE1C(ScreenB22E98 *self);
void func_08B1CE5C(ScreenB22E98 *self);
void func_08B1D774(ScreenB22E98 *self);
void func_08B1D84C(ScreenB22E98 *self);
void func_08B1D8A4(ScreenB22E98 *self);
void func_08B1E764(ScreenB22E98 *self);
void func_08B1E830(ScreenB22E98 *self);
void func_08B1E950(ScreenB22E98 *self);
void func_08B1E980(ScreenB22E98 *self);
ScreenB22E98 *func_08B1F0BC(ScreenB22E98 *self);
void func_08B1F124(ScreenB22E98 *self);
void func_08B1F1D4(ScreenB22E98 *self);
void func_08B1F3E4(ScreenB22E98 *self);
void func_08B1F470(ScreenB22E98 *self, int arg);
void func_08B1F534(void);
void func_08B1F554(void);
void func_08B1F574(int arg);
void *func_08B1F598(void);
void func_08B2041C(ScreenB22E98 *self);
void func_08B23300(ScreenB243C0 *self);
void func_08B23378(ScreenB243C0 *self);
void func_08B23478(ScreenB243C0 *self);
int func_08B234F0(void);
ScreenB243C0 *func_08B238F8(ScreenB243C0 *self);
void func_08B23960(ScreenB243C0 *self);
void func_08B23A04(ScreenB243C0 *self);
void func_08B23B88(ScreenB243C0 *self);
void func_08B23C00(ScreenB243C0 *self, int arg);
void func_08B23CB4(void);
void func_08B23CD4(void);
void func_08B23CF4(int arg);
void func_08B2459C(void);
void func_08B245B8(ScreenB25044 *self);
void func_08B24674(void);
void func_08B24958(ScreenB25044 *self);
void func_08B249AC(void);
void func_08B249CC(void);
void func_08B249EC(void);
ScreenB25044 *func_08B24A0C(ScreenB25044 *self);
void func_08B24A74(ScreenB25044 *self);
void func_08B24BA8(ScreenB25044 *self);
void func_08B24D10(ScreenB25044 *self);
void func_08B24D90(ScreenB25044 *self, int arg);
void func_08B24E38(void);
void func_08B24E58(void);
void func_08B24E78(int arg);
void func_08B2513C(Panel26C88 *self);
void func_08B255A0(Panel26C88 *self);
void func_08B257AC(Panel26C88 *self, int id, int arg);
void func_08B26C88(Panel26C88 *self);
void func_08B26ED4(Panel26C88 *self);
void func_08B26F5C(Panel26C88 *self);
void func_08B26FB4(Panel26C88 *self, int value);
void func_08B26FE0(Panel26C88 *self, unsigned int flags);
void func_08B2716C(Panel26C88 *self);
void func_08B2718C(Panel26C88 *self);
int func_08B271C8(Panel26C88 *self);
void func_08B27A98(Panel26C88 *self, int id, int arg);
const short *func_08B27FA8(void);
const short *func_08B27FF4(void);
const short *func_08B28000(void);
void func_08B28798(ScreenB29CF8 *self);
void func_08B289D8(ScreenB29CF8 *self);
void func_08B28B34(ScreenB29CF8 *self);
void func_08B28BB4(ScreenB29CF8 *self);
void func_08B28C6C(ScreenB29CF8 *self);
void func_08B28CA4(void);
void func_08B28CC4(void);
void func_08B28CE4(void);
void func_08B292D4(ScreenB29CF8 *self);
int func_08B2934C(ScreenB29CF8 *self, int arg);
void func_08B29458(ScreenB29CF8 *self);
void func_08B294E0(ScreenB29CF8 *self);
void func_08B29538(ScreenB29CF8 *self);
void func_08B29558(ScreenB29CF8 *self, char arg);
void func_08B29598(ScreenB29CF8 *self);
void func_08B295B8(ScreenB29CF8 *self);
void func_08B295D8(ScreenB29CF8 *self, int value);
void func_08B29614(ScreenB29CF8 *self);
void func_08B29668(void);
void func_08B29688(void);
void func_08B296A8(void);
void func_08B296C8(void);
ScreenB29CF8 *func_08B296E8(ScreenB29CF8 *self);
void func_08B29788(int arg);
int func_08B29F3C(Obj29F3C *self);
int func_08B2A118(Obj29F3C *self);
int func_08B2A308(Obj29F3C *self);
void func_08B2A61C(Obj29F3C *self);
void func_08B2A82C(Obj29F3C *self);
void func_08B2A970(Obj29F3C *self);
void func_08B2AA04(Obj29F3C *self);
void func_08B2ABF8(Obj29F3C *self);
void func_08B2AE90(Obj29F3C *self);
void func_08B2AEE0(Obj29F3C *self);
void func_08B2AF98(Obj29F3C *self);
void func_08B2AFF0(Obj29F3C *self);
void func_08B2B048(Obj29F3C *self);
void func_08B2B064(Obj29F3C *self);
void func_08B2B0A4(Obj29F3C *self);
void func_08B2B0C4(Obj29F3C *self);
void func_08B2B0E4(Obj29F3C *self);
void func_08B2B158(Obj29F3C *self);
void func_08B2B1E0(Obj29F3C *self);
void func_08B2B22C(Obj29F3C *self);
int func_08B2B574(Obj29F3C *self, MenuLayout *layout, MenuModel **handles, void **texts);
void func_08B2BAEC(void);
void func_08B2BB0C(void);
void func_08B2BC40(Obj29F3C *self);
int func_08B2BCCC(Obj29F3C *self);
void func_08B2C908(Obj29F3C *self, MenuLayout *layout, MenuModel **handles, void **texts);
int func_08B2D604(Obj2D604 *self);
int func_08B2D654(Obj2D604 *self);
void func_08B2D7A4(Obj2D604 *self);
int func_08B2D840(Obj2D604 *self);
void func_08B2D860(Obj2D604 *self);
void func_08B2D8D0(Obj2D604 *self);
void func_08B2D928(Obj2D604 *self);
void func_08B2DA74(Obj2D604 *self);
void func_08B2DC3C(Obj2D604 *self);
void func_08B2DE2C(Obj2D604 *self);
void func_08B2DE4C(Obj2D604 *self);
void func_08B2DE8C(Obj2D604 *self);
void func_08B2DEC4(Obj2D604 *self);
void func_08B2DEE4(Obj2D604 *self);
void func_08B2DF50(Obj2D604 *self, int index, void *value);
void func_08B2DF74(Obj2D604 *self);
void func_08B2DF94(Obj2D604 *self);
void func_08B2E134(Obj2D604 *self);
int func_08B2E680(Obj2D604 *self);
void func_08B2E73C(Obj2D604 *self);
void func_08B2E780(Obj2D604 *self, unsigned char value);
unsigned char func_08B2E790(Obj2D604 *self);
unsigned char func_08B2F898(Obj2F8F4 *self, int id);
void func_08B2F8F4(Obj2F8F4 *self);
int func_08B2F98C(Obj2F8F4 *self);
void func_08B301D8(Obj2F8F4 *self);
void func_08B301F8(Obj2F8F4 *self);
void func_08B30334(Obj2F8F4 *self);
void func_08B30394(Obj2F8F4 *self);
void func_08B304F8(Obj2F8F4 *self);
unsigned char func_08B3052C(Obj2F8F4 *self);
int func_08B3071C(Obj2F8F4 *self);
void func_08B30804(Obj2F8F4 *self, Block62C *block);
void func_08B30838(Obj2F8F4 *self);
void func_08B318F0(ScreenB32D90 *self);
void func_08B31A84(ScreenB32D90 *self);
void func_08B31BA8(ScreenB32D90 *self);
void func_08B31CA4(void);
void func_08B31CC4(void);
void func_08B31CE4(void);
void func_08B324B8(ScreenB32D90 *self);
void func_08B32530(ScreenB32D90 *self);
void func_08B325C4(ScreenB32D90 *self);
void func_08B3261C(ScreenB32D90 *self);
void func_08B3263C(ScreenB32D90 *self, char arg);
void func_08B3267C(ScreenB32D90 *self);
void func_08B3269C(ScreenB32D90 *self);
void func_08B326BC(ScreenB32D90 *self, int value);
ScreenB32D90 *func_08B326F8(ScreenB32D90 *self);
void func_08B342D4(void *object, int arg);
void func_08B344E0(Obj344E0 *self, unsigned int flags);
void func_08B34544(Obj344E0 *self);
void func_08B34570(Obj344E0 *self);
ScreenB41CC0 *func_08B34C18(ScreenB41CC0 *self);
void func_08B35030(ScreenB41CC0 *self);
void func_08B353DC(ScreenB41CC0 *self);
void func_08B35598(ScreenB41CC0 *self);
void func_08B3561C(ScreenB41CC0 *self);
void func_08B35680(ScreenB41CC0 *self);
void func_08B35760(ScreenB41CC0 *self);
void func_08B35838(ScreenB41CC0 *self);
void func_08B358EC(ScreenB41CC0 *self);
void func_08B35A08(ScreenB41CC0 *self);
void func_08B35A44(ScreenB41CC0 *self);
void func_08B35A64(ScreenB41CC0 *self);
void func_08B35ABC(ScreenB41CC0 *self);
void func_08B35CF0(ScreenB41CC0 *self, char arg);
void func_08B35EE0(void);
void func_08B36490(void **self);
void func_08B36654(ScreenB41CC0 *self);
void func_08B36AEC(ScreenB41CC0 *self);
void func_08B36B38(ScreenB41CC0 *self);
void func_08B36C0C(ScreenB41CC0 *self);
void func_08B378BC(ScreenB41CC0 *self);
void func_08B37A0C(ScreenB41CC0 *self);
void func_08B37A4C(ScreenB41CC0 *self);
void func_08B37AA4(ScreenB41CC0 *self);
void func_08B37B24(ScreenB41CC0 *self);
void func_08B38CC4(ScreenB41CC0 *self, int arg);
void func_08B38CFC(ScreenB41CC0 *self, int arg);
void func_08B38EF8(ScreenB41CC0 *self);
void func_08B390D4(ScreenB41CC0 *self, int arg);
void func_08B397F8(ScreenB41CC0 *self);
void func_08B39834(ScreenB41CC0 *self);
void func_08B39C9C(ScreenB41CC0 *self, int arg);
void func_08B39E40(ScreenB41CC0 *self, int id);
void func_08B3A0DC(ScreenB41CC0 *self, int value);
void func_08B3A1CC(ScreenB41CC0 *self, int arg);
void func_08B3A3D8(ScreenB41CC0 *self, int arg);
void func_08B3AA8C(ScreenB41CC0 *self);
void func_08B3AAA8(void);
int func_08B3AAC0(ScreenB41CC0 *self);
void func_08B3B4DC(ScreenB41CC0 *self);
void func_08B3B51C(void);
void func_08B3B6B0(ScreenB41CC0 *self);
void func_08B3B764(ScreenB41CC0 *self, char arg);
void func_08B3B978(ScreenB41CC0 *self);
void func_08B3BF1C(ScreenB41CC0 *self);
void func_08B3BF58(ScreenB41CC0 *self);
void func_08B3C554(ScreenB41CC0 *self, int arg1, int arg2);
void func_08B3C8B0(ScreenB41CC0 *self);
void func_08B3C900(int kind, int arg1, int arg2, int arg3);
void func_08B3C944(ScreenB41CC0 *self, int arg);
void func_08B3C9DC(void);
void func_08B3CA54(void);
void func_08B3CA8C(ScreenB41CC0 *self, void *info);
void func_08B3CAD8(void);
void func_08B3CAF0(int index, MenuCallback callback);
void func_08B3CBD8(void);
void func_08B3CC50(void);
void func_08B3CC6C(void);
void func_08B3CDDC(void);
void func_08B3CEB0(void);
void func_08B3CED0(void);
void func_08B3CF00(void);
void func_08B3CF20(void);
void func_08B3CF40(void);
void func_08B3CFA4(ScreenB41CC0 *self);
void func_08B3D3BC(void);
void func_08B3D3DC(void);
void func_08B3D40C(void);
void func_08B3D5E0(void);
void func_08B3D5FC(void);
void func_08B3D6EC(void);
void func_08B3D720(void);
void func_08B3D760(void);
void func_08B40590(ScreenB41CC0 *self);
void func_08B40C80(ScreenB41CC0 *self);
void func_08B40E60(ScreenB41CC0 *self, int kind, int arg1, int arg2, int arg3);
void func_08B41E80(void);
void func_08B41EA0(void);
void func_08B41EC0(void);
void func_08B41EE0(void);
ScreenB42088 *func_08B41F00(ScreenB42088 *self);
void func_08B41F44(ScreenB42088 *self, int kind);
void func_08B4201C(ScreenB42088 *self);
void func_08B42038(ScreenB42088 *self);
void func_08B42060(ScreenB42088 *self);
void func_08B421C8(void **objects, int index, int arg);
Obj42234 *func_08B42234(Obj42234 *self);
void func_08B4229C(Block4229C *self, void *heap);
void func_08B422B8(Block4229C *self);
Block4229C *func_08B42368(Block4229C *self);
void func_08B423BC(Block4229C *self);
void func_08B423D0(Block4229C *self);
Ids4278C *func_08B42764(Ids4278C *self);
void func_08B4278C(Ids4278C *self);
void func_08B4280C(HeapBlock *self);
HeapBlock *func_08B42A04(HeapBlock *self);
void func_08B42A58(HeapBlock *self);
Heap42AA0 *func_08B42AA0(Heap42AA0 *self);
void func_08B42B08(Heap42AA0 *self);
void *func_08B42B40(void);
void func_08B42B88(void *block);
void func_08B42BEC(void *block);
Obj42C30 *func_08B42C30(Obj42C30 *self);
void func_08B42C68(Obj42C30 *self);
void func_08B42D14(Obj42C30 *self);
void func_08B42D78(Obj42C30 *self, int value);
void func_08B42ECC(Obj42C30 *self);
void func_08B42F80(Obj42C30 *self);
void *func_08B4331C(void *self);
void func_08B43344(void *self);
HeapBlock *func_08B43374(HeapBlock *self);
void func_08B43400(HeapBlock *self);
HeapBlock *func_08B43620(HeapBlock *self);
void func_08B43674(HeapBlock *self);
void func_08B436D8(HeapBlock *self);
Name439C8 *func_08B43974(Name439C8 *self);
void func_08B439C8(Name439C8 *self);
HeapBlock *func_08B439F4(HeapBlock *self);
void func_08B43A80(HeapBlock *self);
Obj43DD8 *func_08B43DD8(Obj43DD8 *self);
void func_08B43E28(void);
void func_08B43E44(void *block);
void func_08B43E68(Obj43DD8 *self);
void func_08B43E78(Obj43DD8 *self);
void func_08B43F00(Obj43F00 *self);
void func_08B44024(Obj44024 *self);
void func_08B443B8(Obj43F00 *self);
void func_08B4440C(List4440C *self);
void func_08B44578(void);
void func_08B44614(void);
void func_08B446EC(void);
void func_08B44724(void *block);
List4440C *func_08B44750(List4440C *self);
Obj44F38 *func_08B44F10(Obj44F38 *self);
void func_08B44F38(Obj44F38 *self);
void func_08B44FF8(void);
int func_08B45200(int arg);
void *func_08B45260(void *self);
void func_08B45288(void *self);
void func_08B45380(void);
int func_08B45414(Obj45414 *self);
int *func_08B4541C(Obj45414 *self);
Obj45414 *func_08B45444(Obj45414 *self);
void func_08B45798(Obj45414 *self);
int func_08B45808(Obj45414 *self, int index);
void func_08B45A14(void);
void func_08B45A1C(void *block);
void func_08B45A98(void *block);
void func_08B45AB4(void *block);
void func_08B45AD0(void *block);
Net45BB8 *func_08B45BB8(Net45BB8 *self);
void func_08B45BE0(Net45BB8 *self);
void func_08B45C04(Net45BB8 *self, int arg1, int arg2, int arg3, int arg4, int arg5);
void func_08B45C90(Net45BB8 *self, int arg1, int arg2, int arg3, int arg4, int arg5);
void func_08B45E0C(Net45BB8 *self);
void func_08B45E4C(Net45BB8 *self);
void func_08B45EC0(Net45BB8 *self);
void *func_08B45EE0(Net45BB8 *self);
void *func_08B45EEC(Net45BB8 *self);
void *func_08B45EF8(Net45BB8 *self);
void func_08B45F04(Net45BB8 *self);
void func_08B45F24(Net45BB8 *self);
void func_08B45F44(Net45BB8 *self, int value);
void func_08B46080(Net45BB8 *self);
void func_08B460A0(Net45BB8 *self);
void func_08B460C4(Net45BB8 *self);
void func_08B46584(Net45BB8 *self, int value);
void func_08B46698(Net45BB8 *self, char arg);
void func_08B46724(Net45BB8 *self, int arg);
void func_08B46840(Net45BB8 *self);
void func_08B469DC(Net45BB8 *self);
int func_08B46A00(Net45BB8 *self);
int func_08B46A3C(Net45BB8 *self);
void func_08B46AEC(Net45BB8 *self);
void func_08B46B30(Net45BB8 *self);
void func_08B46D9C(Net45BB8 *self);
int func_08B46DB8(Net45BB8 *self);
int func_08B46DC4(Net45BB8 *self);
int func_08B46DDC(Net45BB8 *self);
int func_08B46DE8(Net45BB8 *self);
void func_08B46E34(Net45BB8 *self);
void func_08B46E7C(Net45BB8 *self);
void func_08B46F00(Net45BB8 *self);
int func_08B477E8(Net45BB8 *self, int id, void *out, int arg);
int func_08B47880(Net45BB8 *self, int id, int *ids, int count);
void func_08B4863C(void *object);
void func_08B4869C(void *object);
void func_08B48A0C(void *object);
void func_08B48A78(void *object);
int func_08B48AF0(void *object);
int func_08B48B30(void *object);
void func_08B48CC0(void *object);
int func_08B48D5C(void *object, int value);
void func_08B491FC(void *object, int arg);
void func_08B49290(void *object, int arg);
void func_08B49324(void *object, int arg);
int func_08B493B8(void *object, int id);
int func_08B495D4(void *object, int id, int *out);
void func_08B4A26C(void *object);
void func_08B4A2F4(void *object);
void func_08B4A37C(void *object);
void func_08B4A3FC(void *object);
void func_08B4A5CC(void *self, int arg, float *vector, unsigned char flag);
int func_08B4A6A8(void *self, int arg, int direction, unsigned char flag);
int func_08B4A7D8(void *self, int arg, int direction, unsigned char flag);
void func_08B4AE44(Obj4AE44 *self, int index, unsigned char flag);
void func_08B4AE74(Obj4AE44 *self, int index, unsigned char flag);
Widget4C010 *func_08B4C010(Widget4C010 *self);
void func_08B4C0B4(Widget4C010 *self);
void func_08B4C194(Widget4C010 *self, int arg1, int arg2, int arg3, int arg4, int arg5);
void func_08B4C20C(Widget4C010 *self);
void func_08B4C27C(Widget4C010 *self);
void func_08B4C330(Widget4C010 *self);
void func_08B4C500(Widget4C010 *self);
void func_08B4C580(Widget4C010 *self);
void func_08B4C5D4(Widget4C010 *self, int arg1, int arg2, int arg3, int arg4);
Widget4C7F8 *func_08B4C7F8(Widget4C7F8 *self);
void func_08B4C89C(Widget4C7F8 *self);
void func_08B4C938(Widget4C7F8 *self);
void func_08B4C9C0(Widget4C7F8 *self);
void func_08B4C9E4(Widget4C7F8 *self);
Part4CA98 *func_08B4CA98(Part4CA98 *self);
void func_08B4CAD4(Part4CA98 *self);
void func_08B4CC90(Part4CA98 *self);
void func_08B4CE60(Part4CA98 *self);
Part4CEDC *func_08B4CEDC(Part4CEDC *self);
void func_08B4CF18(Part4CEDC *self);
void func_08B4D188(Part4CEDC *self);
void func_08B4D464(Part4CEDC *self);
Part4D4DC *func_08B4D4DC(Part4D4DC *self);
void func_08B4D518(Part4D4DC *self);
void func_08B4D728(Part4D4DC *self);
int func_08B4D754(Part4D4DC *self);
void func_08B4D75C(Part4D4DC *self);
void func_08B4D764(Part4D4DC *self);
void func_08B4D770(Part4D4DC *self);
void func_08B4DA70(Part4D4DC *self);
void func_08B4DEDC(Part4D4DC *self);
Widget4DF58 *func_08B4DF58(Widget4DF58 *self);
void func_08B4E054(Widget4DF58 *self);
void func_08B4E794(Widget4DF58 *self);
int func_08B4E7D4(void *object);
void func_08B4E81C(Widget4DF58 *self);
void func_08B4E838(Widget4DF58 *self);
void func_08B4E854(Widget4DF58 *self);
void func_08B4E870(Widget4DF58 *self);
void func_08B4E9F4(Widget4DF58 *self);
Elem4ECC8 *func_08B4ECC8(Elem4ECC8 *self);
void func_08B4ECF0(Elem4ECC8 *self);
void func_08B4ED40(Elem4ECC8 *self);
void func_08B4EFFC(Elem4ECC8 *self);
void func_08B4F030(Elem4ECC8 *self);
void func_08B4F0D0(Elem4ECC8 *self);
Widget4F194 *func_08B4F194(Widget4F194 *self);
void func_08B4F270(Widget4F194 *self);
void func_08B4F5A0(Widget4F194 *self);
int func_08B4F5C4(void *object);
void func_08B4F634(Widget4F194 *self);
int func_08B4F9E8(void *self, int value, int divisor);
Elem4FE98 *func_08B4FE70(Elem4FE98 *self);
void func_08B4FE98(Elem4FE98 *self);
void func_08B4FF54(Elem4FE98 *self);
void func_08B4FFAC(Elem4FE98 *self);
void func_08B50400(Elem4FE98 *self);
Widget504DC *func_08B504DC(Widget504DC *self);
void func_08B50598(Widget504DC *self);
void func_08B508C0(void *object, int arg1, char arg2);
void func_08B509D0(Widget504DC *self);
void func_08B50AB8(Widget504DC *self);
void func_08B50DE4(Widget504DC *self);
void func_08B50E64(Widget504DC *self);
int func_08B51020(void *self, int value, int divisor);
void func_08B516E4(Obj516E4 *self);
void func_08B51838(Obj516E4 *self, int arg);
void *func_08B51968(Obj516E4 *self);
void func_08B51970(Obj516E4 *self);
void func_08B51A30(void);
void func_08B51CC0(Obj516E4 *self);
void func_08B51CFC(Obj516E4 *self);
void func_08B51D34(Obj516E4 *self);
void func_08B51D74(Obj516E4 *self, int arg);
void func_08B51DCC(Obj516E4 *self);
void func_08B51E14(Obj516E4 *self);
void func_08B51E64(Obj516E4 *self);
void func_08B51EA4(Obj516E4 *self);
void func_08B51EEC(Obj516E4 *self);
void func_08B51F50(Obj516E4 *self);
void func_08B51FD4(Obj516E4 *self);
void func_08B520C8(Obj516E4 *self);
Widget52288 *func_08B52288(Widget52288 *self);
void func_08B5232C(Widget52288 *self);
void func_08B523D0(Widget52288 *self);
void func_08B523FC(Widget52288 *self);
void func_08B52420(Widget52288 *self);
void func_08B52444(Widget52288 *self);
void func_08B5247C(Widget52288 *self);
Widget525B0 *func_08B525B0(Widget525B0 *self);
void func_08B52654(Widget525B0 *self);
void func_08B527E8(Widget525B0 *self);
void func_08B52898(Widget525B0 *self);
void func_08B52990(Widget525B0 *self);
void func_08B52A24(Widget525B0 *self);
void func_08B52A54(Widget525B0 *self);
void func_08B52A94(Widget525B0 *self, unsigned char arg1, unsigned char arg2, unsigned int arg3, unsigned int arg4, unsigned int arg5, unsigned int arg6, unsigned int arg7, unsigned char arg8);
void func_08B52B48(Widget525B0 *self, unsigned char arg1, unsigned char arg2, unsigned char arg3);
void func_08B52C20(Widget525B0 *self);
void func_08B52C74(Widget525B0 *self, unsigned char arg1, unsigned char arg2, unsigned int arg3, unsigned int arg4, unsigned int arg5, unsigned int arg6, unsigned int arg7);
void func_08B52D54(Widget525B0 *self);
Widget52EB0 *func_08B52EB0(Widget52EB0 *self);
void func_08B52F54(Widget52EB0 *self);
void func_08B53090(Widget52EB0 *self);
unsigned char func_08B530A0(void *object);
void func_08B530C4(void *object);
void func_08B53184(Widget52EB0 *self);
void func_08B531CC(Widget52EB0 *self);
void func_08B53250(Widget52EB0 *self);
void func_08B532A4(Widget52EB0 *self);
void func_08B532D0(Widget52EB0 *self);
Widget535C8 *func_08B535C8(Widget535C8 *self);
void func_08B53604(Widget535C8 *self);
void func_08B537A0(Widget535C8 *self);
void func_08B53824(Widget535C8 *self);
void func_08B538C8(Widget535C8 *self);
void func_08B53984(Widget535C8 *self);
ScreenB54058 *func_08B53B88(ScreenB54058 *self);
void func_08B53BD4(ScreenB54058 *self);
void func_08B53CD0(ScreenB54058 *self);
void func_08B53DD8(ScreenB54058 *self);
void func_08B53F60(ScreenB54058 *self);
void func_08B53FA4(void);
void func_08B53FC4(void);
void func_08B53FE4(void);
void func_08B54004(void);
ScreenB54C38 *func_08B54060(ScreenB54C38 *self);
void func_08B540D4(ScreenB54C38 *self);
void func_08B54184(ScreenB54C38 *self, unsigned int value, unsigned char arg1, unsigned char arg2, unsigned char arg3);
void func_08B54200(ScreenB54C38 *self);
void func_08B5421C(ScreenB54C38 *self);
void func_08B54278(ScreenB54C38 *self);
void func_08B54294(ScreenB54C38 *self);
void func_08B542B0(ScreenB54C38 *self);
void func_08B542CC(ScreenB54C38 *self);
void func_08B542E8(void);
void func_08B54308(void);
void func_08B54328(void);
void func_08B54348(Link54150 *self);
void func_08B54364(Link54150 *self);
void func_08B54380(Link54150 *self);
void func_08B5439C(Link54150 *self);
void func_08B543D0(ScreenB54C38 *self);
Obj54C40 *func_08B54C40(Obj54C40 *self);
void func_08B54F20(Obj54C40 *self);
void func_08B54F90(Obj54C40 *self);
void func_08B54F9C(Obj54C40 *self);
void func_08B5501C(Obj54C40 *self);
void func_08B5504C(Obj54C40 *self);
void func_08B55058(Obj54C40 *self);
void func_08B55074(Obj54C40 *self);
void func_08B55090(Obj54C40 *self);
void func_08B550AC(void *self, Obj54C40 *object);
Model550C8 *func_08B550C8(Model550C8 *self);
void func_08B550D4(Model550C8 *self, int id);
void func_08B55138(Model550C8 *self, void *source, int index);
void func_08B55184(void *object, int arg);
Obj55244 *func_08B55244(Obj55244 *self);
void func_08B55578(void);
void func_08B555EC(Obj55734 *self, int index, int value);
void func_08B55734(Obj55734 *self, int index);
void func_08B5575C(void);
Obj558B0 *func_08B558B0(Obj558B0 *self);
void func_08B56394(void *self, MenuModel *model, void *source, int index);
Obj563F0 *func_08B563F0(Obj563F0 *self);
void func_08B56A7C(void *object);
void func_08B56CB0(Obj56CB0 *self);
void func_08B56CCC(Obj56CB0 *self);
void func_08B57420(void);
void func_08B5743C(Obj5743C *self);
void func_08B57470(void);
void func_08B575B4(void *self, MenuModel *model, void *source, int index);
void func_08B57600(void *self, void *text, void *source, int index);
void func_08B5810C(unsigned char value);
void func_08B58370(void);
void func_08B58420(void);
void func_08B58538(unsigned char value);
void func_08B58568(void);
void func_08B586CC(void);
void *func_08B58B30(void *self);
void func_08B58B70(Obj59230X *self);
void func_08B58B78(Obj59230X *self, int arg1, int arg2);
void func_08B58F64(int arg1, int arg2);

extern int D_089D6AE8;
extern void *D_089D7830;
extern void *D_089E09C0;
extern ScreenB1CB00 *D_08B1CB00;
extern ScreenB22E98 *D_08B22E98;
extern ScreenB243C0 *D_08B243C0;
extern ScreenB25044 *D_08B25044;
extern ScreenB29CF8 *D_08B29CF8;
extern Obj29F3C *D_08B2D264;
extern ScreenB32D90 *D_08B32D90;
extern ScreenB41CC0 *D_08B41CC0;
extern ScreenB42088 *D_08B42088;
extern Heap42AA0 *D_08B42C28;
extern void **D_08B44F08;
extern ScreenB54058 *D_08B54058;
extern ScreenB54C38 *D_08B54C38;

#ifdef __cplusplus
}
#endif

#endif /* DISSIDIA_MENU_AUTO_REGION4_H */
