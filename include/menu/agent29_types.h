#ifndef DISSIDIA_MENU_AGENT29_TYPES_H
#define DISSIDIA_MENU_AGENT29_TYPES_H

/* Types and prototypes shared by agent29's matches (menu 0x089DDF24..0x08A1E410).
 * Builds on the menu region 1 readability header. */
#include "menu/auto_region1.h"

#ifdef __cplusplus
/* A value kept in the frame across calls (see build/dc-results/stack-spill.md). */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

/* The pad object from func_088729A0 (vptr after 0x34 bytes of data). */
struct Pad29 {
    char data[0x34];
    virtual void unknown_00();
    virtual int Pressed(unsigned long long buttons);
};
extern "C" Pad29 *func_088729A0(int port);

/* A part of the arcade course view (vptr at 0: virtuals only). */
struct ViewPart29 {
    virtual void unknown_00(int flags);
    virtual void unknown_01();
    virtual void unknown_02();
    virtual void Show();
    virtual void Update(float time);
    virtual void Place(void *model);
    virtual void Refresh();
    virtual void unknown_07();
    virtual int IsDone();
};

/* Arcade course view (see src/menu/view_open_089ea804.cpp). */
struct CourseView29 {
    char pad_00[0x14];
    void *models[5];     /* +0x14: frame, title, left, right, ... */
    void *texts[2];      /* +0x28 */
    ViewPart29 *parts[3];
    int mode;            /* +0x3C */
    int page;            /* +0x40 */
    void *heap;          /* +0x44 */
};

/* Flag tests that SNC inlined from in-class helpers. */
struct Flags29 {
    static bool Has(unsigned int flags, unsigned int mask) {
        return (flags & mask) != 0;
    }
};

extern "C" {
#endif

/* Model handle field +0xE (written as a 16-bit state). */
#define MENU_MODEL_STATE(model) (*(short *)((char *)(model) + 0xE))

/* Words of a work block that the region headers do not name yet. */
#define WORK_INT(work, offset) (*((int *)(work) + (offset) / 4))
#define WORK_BYTE(work, offset) (*((unsigned char *)(work) + (offset)))
#define WORK_U16(work, offset) (*((unsigned short *)(work) + (offset) / 2))
#define WORK_SHORT(work, offset) (*((short *)(work) + (offset) / 2))
#define WORK_SBYTE(work, offset) (*((signed char *)(work) + (offset)))

/* Background music player kept by a screen (size not known). */
typedef struct MenuSound29 {
    int id;          /* playing */
    char pad_04[0xC];
} MenuSound29;
MENU1_SIZE(MenuSound29, 0x10);
void func_088D54DC(MenuSound29 *sound);
void func_088D55AC(MenuSound29 *sound, void *heap, void *buffer);
void func_088D541C(MenuSound29 *sound);
void func_088D54A4(MenuSound29 *sound);
void func_088D5318(MenuSound29 *sound, int id, int size);
void func_088D53BC(MenuSound29 *sound);
void func_088D52AC(MenuSound29 *sound, void *heap, int size);
void func_088D5528(MenuSound29 *sound, void *heap);

/* ScreenE75C4 (the start menu: its heap is named START_MENU_BUF) with the
 * fields after its work pointer; same layout as ScreenE75C4. */
typedef struct ScreenE75C4Ex {
    char field_000[0x100];
    MenuTask task;
    void *field_134; /* the heap's buffer */
    void *heap;
    void *page;      /* +0x13C: block of the current page, from the heap */
    WorkE75C4 *work;
    MenuSound29 sound;
    int sound_next;  /* +0x154: the track to switch to */
} ScreenE75C4Ex;
MENU1_AT(ScreenE75C4Ex, work, 0x140);
MENU1_AT(ScreenE75C4Ex, sound, 0x144);

/* Page table at D_089E71C0, indexed by the work block's +0xB1C. */
typedef struct Page71C0 {
    void (*init)(void);
    void (*start)(void *block, void (*done)(void));
    int size;  /* of the block allocated for the page */
    int sound;
} Page71C0;
MENU1_SIZE(Page71C0, 0x10);
extern Page71C0 D_089E71C0[];

/* Parts list of a row: handle indices, ended by 0xFFFF. */
typedef struct RowPart {
    unsigned short handle; /* index into the work block's handles */
    unsigned short mark;   /* index into the work block's handles */
    char pad_04[0x2];
    unsigned short entry;  /* index into D_089E7488 */
} RowPart;

/* Row table at D_089E7518 (7 rows). */
typedef struct Row7518 {
    unsigned short icon;   /* index into the work block's handles */
    unsigned short handle; /* index into the work block's handles */
    unsigned short frame;  /* index into the work block's handles */
    char pad_06[0x2];
    unsigned short entry;  /* index into D_089E7488 */
    unsigned short label;  /* index into the handles; 0xFFFF: no script */
    const unsigned short *script;
    char pad_10[0x4];
    RowPart *parts;
} Row7518;
MENU1_SIZE(Row7518, 0x18);
extern Row7518 D_089E7518[];

/* Gate table at D_089E7434, indexed by the work block's +0xB20. */
typedef struct Gate7434 {
    unsigned short field_00;
    unsigned short light; /* 0xFFFF: none */
    const unsigned short *script;
    char pad_08[0x4];
} Gate7434;
MENU1_SIZE(Gate7434, 0xC);
extern Gate7434 D_089E7434[];

/* Entry table at D_089E7488 (0x12 entries): which text shows it. */
typedef struct Entry7488 {
    unsigned short enabled;
    char pad_02[0x2];
    unsigned short text_on;  /* texts index while enabled (0xFFFF: none) */
    unsigned short text_off; /* texts index otherwise (0xFFFF: none) */
} Entry7488;
MENU1_SIZE(Entry7488, 0x8);
void func_0887B924(void *text); /* show */
extern Entry7488 D_089E7488[];

/* Course chooser of the arcade menu (+0x14C). */
typedef struct ArcadeCourse29 {
    void *view;     /* +0x00 */
    void *list;     /* +0x04 */
    int state;      /* +0x08 */
    int next;       /* +0x0C: state to go to after the current one */
    int field_10;
    int id;         /* +0x14 */
    int field_18;
} ArcadeCourse29;

/* ScreenE9D38 (the arcade menu: its heap is named ARCADE_MENU) with the
 * fields the region header leaves in padding; same layout as ScreenE9D38. */
typedef struct ScreenE9D38Ex {
    char pad_00[0x100];
    MenuTask task;
    void *heap;          /* +0x134 */
    void *buffer;        /* +0x138: the heap's memory */
    PartE8E78 *field_13C;
    void *field_140;
    void *field_144;
    unsigned int field_148;
    ArcadeCourse29 course; /* +0x14C */
    char pad_168[0x4];
    int field_16C;
    int field_170;
    char pad_174[0x4];
    unsigned int field_178;
    char pad_17C[0x4];
    MenuSound29 sound;       /* +0x180 */
    unsigned char field_190; /* set while the talk event plays */
    unsigned char field_191; /* set once the music started */
} ScreenE9D38Ex;
MENU1_AT(ScreenE9D38Ex, heap, 0x134);
MENU1_AT(ScreenE9D38Ex, field_13C, 0x13C);
MENU1_AT(ScreenE9D38Ex, course, 0x14C);
MENU1_AT(ScreenE9D38Ex, field_16C, 0x16C);
MENU1_AT(ScreenE9D38Ex, field_178, 0x178);
MENU1_AT(ScreenE9D38Ex, sound, 0x180);

/* Block of the save data (D_089D712C) at +0x3B7A4; Global9D712C does not
 * reach that far. */
typedef struct Story29 {
    char pad_000[0x1A4];
    unsigned int flags; /* bit 0: the arcade event was seen */
} Story29;
#define SAVE_STORY(save) ((Story29 *)((char *)(save) + 0x3B7A4))
void func_089E7E60(ScreenE9D38Ex *self);
void func_089E0BDC(ScreenE75C4 *screen, int arg);
void func_088E9958(int arg);
void func_088E980C(const char *name, int arg);
void func_088E991C(int arg, void (*done)(void));

#ifdef __cplusplus
}
/* Back to the start menu (D_089E75C4); the zero is a named local. */
struct StartMenu29 {
    static void Open() {
        int none = 0;
        func_089E0BDC(D_089E75C4, none);
    }
};
extern "C" {
#endif

/* main-module functions used here, not declared in auto_region1.h */
void func_088D3A04(void *layout);
void func_0887B1F4(void *messages, unsigned int arg);
void func_08885220(void *object);
int func_088F0A78(char arg);
int func_0887E718(void *object);

/* ScreenE75C4 (0x089E0AC0..0x089E3D80) */
int func_089E1CA4(ScreenE75C4 *self);
void func_089E1DD8(ScreenE75C4 *self);
int func_089E21C8(ScreenE75C4 *self);
void func_089E2CD0(ScreenE75C4 *self);
void func_089E3094(ScreenE75C4 *self, int arg);
void func_089E5C18(ScreenE75C4 *self);
int func_089E5E7C(ScreenE75C4 *self);
void func_089E2E04(ScreenE75C4 *self);
int func_089E2CB0(ScreenE75C4 *self);
void func_089E2698(ScreenE75C4 *self);
int func_088F1474(void);
void func_089E10C4(ScreenE75C4 *self, int size);
void *func_0885C6CC(void *heap, int size, int arg1, int arg2, int one, int arg3);
void func_089E3128(ScreenE75C4 *self, int arg, void *layout, void *handles, void *texts);
void func_089E3710(ScreenE75C4 *self, void *layout, void *handles, void *texts);

/* screen fades (see src/menu/fade_start_089e12bc.c) */
typedef struct FadeColor29 {
    float r;
    float g;
    float b;
    float a;
} FadeColor29;
int func_088D4C5C(void);
void func_08873D34(float time, int kind, void *callback, int arg, int one, int two);
void func_08873DB0(FadeColor29 *color, int mode);

/* Global menu request block (0x09B6D830). */
typedef struct MenuRequest {
    int state;
    int value;
    int result;
    int pending;
} MenuRequest;
extern MenuRequest D_09B6D830;

#ifdef __cplusplus
}

/* Inlined allocation from a menu heap (func_0885C6CC). Its bool parameter
 * makes SNC set the flag argument before the size, as retail. */
struct Heap29 {
    static void *Alloc(void *heap, int size, bool clear) {
        return func_0885C6CC(heap, size, 0, 0, clear, 0);
    }
};
#endif

#endif
