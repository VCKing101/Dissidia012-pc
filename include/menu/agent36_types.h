#ifndef DISSIDIA_MENU_AGENT36_TYPES_H
#define DISSIDIA_MENU_AGENT36_TYPES_H

/* Types and prototypes shared by agent36's matches (menu 0x08A1E894..0x08A51208).
 * Standalone: the structs here describe the same objects as auto_region1.h
 * from the fields these functions use. */

#define A36_OFFSET(type, field) ((unsigned int)&((type *)0)->field)
#define A36_ASSERT(name, cond) typedef char a36_assert_##name[(cond) ? 1 : -1]
#define A36_AT(type, field, offset) A36_ASSERT(at_##type##_##field, A36_OFFSET(type, field) == (offset))
#define A36_SIZE(type, size) A36_ASSERT(size_##type, sizeof(type) == (size))

#ifdef __cplusplus
/* A value kept in the frame across calls (see build/dc-results/stack-spill.md). */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

/* An entry of a member-function table as SNC lays a member-function pointer
 * out: built in place from plain functions, then called through the real
 * pointer type (see src/menu/name_step_08a3e278.cpp). */
struct Step36 {
    short delta;
    short index;
    void *target;
};

/* The object type the step tables are called on (data, then the vptr). */
class Stepper36 {
public:
    char data[4];
    virtual void unknown_00();
};
typedef void (Stepper36::*StepFn36)();

/* Flag tests that SNC inlined from in-class helpers. */
struct Flags36 {
    static bool Has(unsigned int flags, unsigned int mask) {
        return (flags & mask) != 0;
    }
};

extern "C" {
#endif

/* Cursor block set up by func_088D46E4 (see MenuCursor in auto_region1.h). */
typedef struct Cursor36 {
    unsigned int flags;
    int index;                /* +0x04: current entry */
    char pad_08[0x8];
    int top;                  /* +0x10: first entry shown */
    char pad_14[0x18];
} Cursor36;
A36_SIZE(Cursor36, 0x2C);

/* Item reference (func_088B97E8, func_088C1998). */
typedef struct ItemRef36 {
    int field_00;
    int field_04;
} ItemRef36;
ItemRef36 func_088B97E8(int id);
int func_088C1998(ItemRef36 *ref);

/* Model handle (see MenuModel in auto_region1.h). */
typedef struct Model36 {
    char pad_00[0xC];
    short field_0C;
    short field_0E;           /* state */
    char pad_10[0x10];
    unsigned short field_20;
    char pad_22[0xE];
    int field_30;
    int field_34;
    int field_38;
} Model36;
A36_AT(Model36, field_30, 0x30);

/* ------------------------------------------------------------ menu root (D_08A23FF0) */

/* A save record of the menu root's list (0x24C bytes). */
typedef struct SaveRecord36 {
    unsigned short flags;   /* bit 0: in use */
    unsigned short slot;    /* +0x02: save slot */
    char pad_04[0x248];
} SaveRecord36;
A36_SIZE(SaveRecord36, 0x24C);

/* Work block of the menu root (+0x154); see RootWork in auto_region1.h
 * and SaveList29 (src/menu/save_order_08a1c2f8.c). */
typedef struct RootWork36 {
    char pad_00000[0x88C];
    void *messages;           /* +0x88C */
    char pad_00890[0x2328];
    unsigned int list_flags;  /* +0x2BB8 */
    int used;                 /* +0x2BBC: records in use */
    SaveRecord36 records[26]; /* +0x2BC0 */
    int count;                /* +0x6778 */
    short order[0x1C];        /* +0x677C */
    char pad_67B4[0x1B0];
    void *net;                /* +0x6964 */
    char pad_6968[0x24F8];
    unsigned int net_flags;   /* +0x8E60 */
    char pad_8E64[0x1CD24];
    int mode;                 /* +0x25B88: index into D_08A23DB4 */
    char pad_25B8C[0x30];
    char field_25BBC[0x4];    /* block handed to func_08A1E6B4 */
    char pad_25BC0[0x90];
    int pose_kind;            /* +0x25C50: index into D_08A23FB0, -1: none */
    char pad_25C54[0x8];
    int pose;                 /* +0x25C5C */
} RootWork36;
A36_AT(RootWork36, pose_kind, 0x25C50);
A36_AT(RootWork36, pose, 0x25C5C);

/* Pose kinds of the menu root (D_08A23FB0, 0x14-byte rows). */
typedef struct PoseKind36 {
    int field_00;
    int model;                /* -1: none */
    int field_08;
    const short *close_script; /* +0x0C: for func_088D3EA0 */
    int field_10;
} PoseKind36;
A36_SIZE(PoseKind36, 0x14);
extern PoseKind36 D_08A23FB0[];
A36_AT(RootWork36, messages, 0x88C);
A36_AT(RootWork36, records, 0x2BC0);
A36_AT(RootWork36, order, 0x677C);
A36_AT(RootWork36, net, 0x6964);
A36_AT(RootWork36, net_flags, 0x8E60);
A36_AT(RootWork36, mode, 0x25B88);
A36_AT(RootWork36, field_25BBC, 0x25BBC);

/* The last part of a save slot (0x154 bytes, copied whole). */
typedef struct SaveSummary36 {
    int words[0x55];
} SaveSummary36;
A36_SIZE(SaveSummary36, 0x154);

/* One save slot (0x10A8 bytes; 25 of them at save +0x1FECC). */
typedef struct SaveSlot36 {
    char head[0xC];
    char body[0x1B];         /* +0x0C */
    unsigned char flags;     /* +0x27 */
    char pad_28[0xF2C];
    SaveSummary36 summary;   /* +0xF54 */
} SaveSlot36;
A36_AT(SaveSlot36, summary, 0xF54);
A36_SIZE(SaveSlot36, 0x10A8);

/* Effect block of the menu root (+0x13C, 0x3D38 bytes from the heap). */
typedef struct RootEffect36 {
    char pad_0000[0x2C90];
    SaveSlot36 save_info;    /* +0x2C90: the save being loaded or written */
} RootEffect36;
A36_AT(RootEffect36, save_info, 0x2C90);

/* Options block of the menu root (0x6B0C bytes, copied whole). */
typedef struct RootOptions36 {
    char pad_0000[0xE];
    unsigned char field_0E;   /* 1: passed on to func_08B58640 */
    char pad_000F[0x1];
    int data[0x1ABF];
} RootOptions36;
A36_SIZE(RootOptions36, 0x6B0C);

/* Second effect block of the menu root (+0x140, 0x97A0 bytes). */
typedef struct RootEffect36b {
    char pad_0000[0x2C90];
    RootOptions36 options;    /* +0x2C90 */
} RootEffect36b;

typedef struct MenuRoot36 {
    char pad_000[0x138];
    void *heap;               /* +0x138 */
    RootEffect36 *effect;     /* +0x13C */
    RootEffect36b *effect2;   /* +0x140 */
    char pad_144[0x10];
    RootWork36 *work;         /* +0x154 */
} MenuRoot36;
A36_AT(MenuRoot36, effect, 0x13C);
A36_AT(MenuRoot36, work, 0x154);

/* Mode table of the menu root (D_08A23DB4, 0x18-byte rows). */
typedef struct RootMode36 {
    int extra;                    /* nonzero: func_08A1E6B4 runs */
    int text;                     /* 1: func_08A1EA3C runs */
    char pad_08[0x8];
    const short *layout_script;   /* +0x10: for func_088D3F00 */
    const unsigned short *text_script; /* +0x14: for func_088D44EC */
} RootMode36;
A36_SIZE(RootMode36, 0x18);
extern RootMode36 D_08A23DB4[];

/* Save data (D_089D712C). */
/* A name slot of the save data (0x138 bytes). */
typedef struct NameSlot36 {
    char data[0x138];
} NameSlot36;

/* A set of four slots in a character record (0xC6 bytes). */
typedef struct SlotGroup36 {
    char data[0xC6];
} SlotGroup36;

/* Character record returned by func_088CB164. */
typedef struct Chara36 {
    char pad_00[0x10];
    SlotGroup36 groups[5];     /* +0x10 */
} Chara36;

/* Player profile in the save data (+0x1FDD8). */
typedef struct Profile36 {
    char pad_00[0xC];
    char time[0xC];            /* +0x0C: copied into the options (+0x164) */
    char pad_18[0xC];
    unsigned short item;       /* +0x24 */
    char pad_26[0xA];
    char name[0x1A];           /* +0x30: copied into the options (+0x2E) */
    char pad_4A[0xAA];
} Profile36;
A36_SIZE(Profile36, 0xF4);

typedef struct Save36 {
    char pad_00000[0xF58];
    char charas[0xE788];       /* +0xF58: read by func_088CB1EC / func_088CB164 */
    char items[0x106F8];       /* +0xF6E0: read by func_088B912C */
    Profile36 profile;         /* +0x1FDD8 */
    SaveSlot36 slots[25];      /* +0x1FECC */
    int slot_count;            /* +0x39F34 */
    NameSlot36 names[1];       /* +0x39F38 */
} Save36;
A36_AT(Save36, names, 0x39F38);
A36_AT(Save36, items, 0xF6E0);
A36_AT(Save36, profile, 0x1FDD8);
A36_AT(Save36, slots, 0x1FECC);
A36_AT(Save36, slots, 0x1FECC);
extern Save36 *D_089D712C;
void func_088ACA10(Save36 *save);
int func_088CB1EC(void *charas, int chara);
Chara36 *func_088CB164(void *charas, int chara);
int func_088B7B18(SlotGroup36 *group, int slot);
void func_088B765C(SlotGroup36 *group, int slot, int value);
int func_088C8158(ItemRef36 *ref, NameSlot36 *name, int arg);
void func_088F53F4(SaveSlot36 *slots, int slot);
void func_088F7BA0(void *net, int slot);
void func_088F7D8C(void *net, int slot, SaveSlot36 *info);
void func_088F5584(SaveSlot36 *slots, int slot, SaveSlot36 *info);
void func_088F5198(SaveSlot36 *info);
void func_088F5DCC(SaveSlot36 *to, SaveSlot36 *from);
int func_088F5EA0(void *to, void *from);
void func_088F526C(SaveSlot36 *slots, SaveSlot36 *info);
void func_088F55D0(SaveSlot36 *slots, int slot);
void func_088F55F4(SaveSlot36 *slots, int slot);
void func_088F7DE0(void *net, int slot);
void func_088F7E2C(void *net, int slot);
extern void *D_089D7260;
void func_088AEFA0(void *object);
void func_08B5810C(unsigned char value);
void func_088AF044(void *object);
void func_08B58640(void *heap, int on);
void func_08B58538(unsigned char value);

/* ------------------------------------------------------------ Screen28560 (vtable D_08A28560) */

/* A row of Screen28560's list (0x10 bytes from +0x1E94). */
typedef struct Row28560 {
    unsigned char field_00;
    signed char slot;                /* +0x01: name slot in the save data */
    char pad_02[0x6];
    int field_08;
    unsigned short id;               /* +0x0C */
    char pad_0E[0x2];
} Row28560;
A36_SIZE(Row28560, 0x10);

/* An entry of Screen28560 (8 bytes from +0x25AC). */
typedef struct Entry28560 {
    unsigned char flags;             /* bit 0: has a name */
    unsigned char arg;
    char pad_02[0x6];
} Entry28560;
A36_SIZE(Entry28560, 0x8);

/* Request block of Screen28560 (+0x244C), handed to func_08AF55B8. */
typedef struct Request28560 {
    int field_00;
    int id;                          /* +0x04 */
    char pad_08[0x44];
    ItemRef36 *name;                 /* +0x4C */
} Request28560;
A36_AT(Request28560, name, 0x4C);

/* Item panel of Screen28560 (+0x249C), handed to func_08AFF218. */
typedef struct Panel28560 {
    int item;                        /* +0x00 */
    void *handles;                   /* +0x04 */
    void *texts;                     /* +0x08 */
    void *messages;                  /* +0x0C */
    int field_10;
    int x;                           /* +0x14 */
    int y;                           /* +0x18 */
    char pad_1C[0xE0];
    int field_FC;                    /* +0xFC */
} Panel28560;
A36_SIZE(Panel28560, 0x100);

/* Cursor of Screen28560 (+0x2044). */
typedef struct Cursor28560 {
    int field_00;
    int index;                       /* +0x04: into the entries */
    char pad_08[0x24];
} Cursor28560;

/* Work block of Screen28560 (+0x28F0); see Work28560 in auto_region1.h. */
typedef struct Work28560x {
    void *layout[0x100];             /* +0x000 */
    Model36 *handles[0x120];         /* +0x400 */
    void *texts[0x57B];              /* +0x880 */
    int state;                       /* +0x1E6C */
    unsigned int flags;              /* +0x1E70 */
    char pad_1E74[0x20];
    Row28560 rows[19];               /* +0x1E94 */
    char pad_1FC4[0x80];
    Cursor28560 cursor;              /* +0x2044 */
    char field_2070[0x1CC];          /* +0x2070: handed to func_088D4B74 */
    short scripts[8][8];             /* +0x223C */
    unsigned short text_scripts[8][0x10]; /* +0x22BC */
    char pad_23BC[0x50];
    int kind;                        /* +0x240C: index into D_08A28AE8, -1: none */
    char pad_2410[0x8];
    int part;                        /* +0x2418 */
    char pad_241C[0x30];
    Request28560 request;            /* +0x244C */
    Panel28560 panel;                /* +0x249C */
    int row;                         /* +0x259C */
    char pad_25A0[0x4];
    int timer;                       /* +0x25A4 */
    unsigned char field_25A8;
    char pad_25A9[0x3];
    Entry28560 entries[100];         /* +0x25AC */
    ItemRef36 ref;                   /* +0x28CC */
} Work28560x;
A36_AT(Work28560x, texts, 0x880);
A36_AT(Work28560x, state, 0x1E6C);
A36_AT(Work28560x, rows, 0x1E94);
A36_AT(Work28560x, cursor, 0x2044);
A36_AT(Work28560x, field_2070, 0x2070);
A36_AT(Work28560x, scripts, 0x223C);
A36_AT(Work28560x, text_scripts, 0x22BC);
A36_AT(Work28560x, kind, 0x240C);
A36_AT(Work28560x, part, 0x2418);
A36_AT(Work28560x, request, 0x244C);
A36_AT(Work28560x, row, 0x259C);
A36_AT(Work28560x, timer, 0x25A4);
A36_AT(Work28560x, entries, 0x25AC);
A36_AT(Work28560x, ref, 0x28CC);
A36_AT(Work28560x, panel, 0x249C);

typedef struct Screen28560x {
    void *vtable;
    char pad_04[0x28EC];
    Work28560x *work;                /* +0x28F0 */
} Screen28560x;
A36_AT(Screen28560x, work, 0x28F0);

void func_088D4B74(void *block, int arg);
void func_08A24E20(Screen28560x *self, Model36 **handles, void **texts);

/* Kinds of Screen28560 (D_08A28AE8, 0x14-byte rows). */
typedef struct Kind28560 {
    char pad_00[0x8];
    const short *close_script;       /* +0x08: for func_088D3EA0 */
    char pad_0C[0x4];
    const unsigned short *parts;     /* +0x10: indexed by the work block's part */
} Kind28560;
A36_SIZE(Kind28560, 0x14);
extern Kind28560 D_08A28AE8[];
extern void *D_08B01CF8;
void func_08AF55B8(void *object, Request28560 *request);
void func_08AFF218(void *object, Panel28560 *panel);
int func_08A256C8(Screen28560x *self);
void func_08A245C0(Screen28560x *self);
void func_08A246F4(Screen28560x *self);
void func_08A26C48(Screen28560x *self, int arg);

/* message box object (D_089D69E8) */
extern void *D_089D69E8;
int func_08885938(void *box);
void func_08885988(void *box);
int func_08885608(void *box, int arg);
void func_088854E8(void *box, int kind, unsigned short id);
void func_088853A8(void *box, int arg);
void func_08885890(void *box, unsigned char arg);
void func_0888550C(void *box, void *messages, int arg, int id);

/* ------------------------------------------------------------ unlock tables (D_08A29BF0) */

/* Header of the unlock table: item counts of the five groups. */
typedef struct UnlockHeader36 {
    char pad_00[0x2];
    unsigned short counts[5];
    char pad_0C[0x4];
} UnlockHeader36;
A36_SIZE(UnlockHeader36, 0x10);

/* An unlock item (0x1C bytes). */
typedef struct Unlock36 {
    char pad_00[0x8];
    unsigned short need;      /* +0x08: amount of the kind's resource needed */
    unsigned short id;        /* +0x0A */
    char pad_0C[0x2];
    unsigned short needs_id;  /* +0x0E: must be known first */
    char pad_10[0x2];
    unsigned short kind;      /* +0x12 */
    char pad_14[0x8];
} Unlock36;
A36_SIZE(Unlock36, 0x1C);

extern UnlockHeader36 *D_08A29BF0;
extern unsigned int *D_089D726C;  /* known-id bit set */
#ifdef __cplusplus
/* Id tests that SNC inlined (see src/menu/known_count_08a29840.cpp). */
struct Id36 {
    static bool IsNone(unsigned short id) {
        return id == 0xFFFF;
    }
    static bool IsKnown(unsigned int id) {
        return (D_089D726C[id >> 5] & (1 << (id & 0x1F))) != 0;
    }
    static void SetKnown(unsigned int id) {
        unsigned int *word = &D_089D726C[id >> 5];
        *word |= 1 << (id & 0x1F);
    }
};
#endif
extern Unlock36 *D_09B6D9F8[5];
int func_08A29B5C(unsigned short id);   /* known */
unsigned short func_08A2969C(int group); /* item count of a group */
void *func_08935828(void *dst, int value, int size); /* memset */

#ifdef __cplusplus
/* Kinds whose items count towards a resource (inlined). */
struct UnlockKind36 {
    static unsigned char Counts(unsigned short kind) {
        int counts = 0;
        if (kind == 1 || (kind > 2 && kind < 14)) {
            counts = 1;
        }
        return counts;
    }
};
#endif

/* A script command (4 halfwords). */
typedef struct ScriptCmd36 {
    short op;
    short arg1;
    short arg2;
    short arg3;
} ScriptCmd36;

/* Work block of the unlock list screen (+0x138). */
typedef struct UnlockWork36 {
    void *layout[0x100];      /* +0x000 */
    void *handles[0x120];     /* +0x400 */
    char pad_0880[0x3B8];
    ScriptCmd36 cmds[32][4];  /* +0xC38: four commands per row */
    char pad_1038[0x10];
    unsigned int flags[0x27]; /* +0x1048: per row */
    int list[0x100];          /* +0x10E4: indices of the items shown */
    Unlock36 *items;          /* +0x14E4 */
    int group;                /* +0x14E8 */
    char pad_14EC[0x20];
    int count;                /* +0x150C: rows in use */
    char pad_1510[0x14];
    int have[8];              /* +0x1524 */
} UnlockWork36;
A36_AT(UnlockWork36, cmds, 0xC38);
A36_AT(UnlockWork36, flags, 0x1048);
A36_AT(UnlockWork36, list, 0x10E4);
A36_AT(UnlockWork36, items, 0x14E4);
A36_AT(UnlockWork36, have, 0x1524);

typedef struct UnlockScreen36 {
    char pad_000[0x138];
    UnlockWork36 *work;       /* +0x138 */
} UnlockScreen36;
void func_088D3CF0(void *layout, void *handles, ScriptCmd36 *script);

/* Work block of the grid page (+0x138): the same block as UnlockWork36
 * with five commands per row from +0xD18 instead of four from +0xC38. */
typedef struct GridWork36 {
    void *layout[0x100];      /* +0x000 */
    void *handles[0x120];     /* +0x400 */
    char pad_0880[0xC];
    void *messages;           /* +0x88C */
    void *texts[0x88];        /* +0x890 */
    char pad_0AB0[0x2];
    char buffer[0x126];       /* +0xAB2: number text */
    Cursor36 cursor;          /* +0xBD8: cursor block (func_088D46E4) */
    char pad_0C04[0x114];
    ScriptCmd36 cells[20][5]; /* +0xD18 */
    char pad_1038[0x10];
    unsigned int flags[0x31]; /* +0x1048: per row */
    char pad_110C[0x3EC];
    unsigned int field_14F8;  /* +0x14F8 */
    char pad_14FC[0x4];
    int field_1500;           /* +0x1500 */
    int field_1504;           /* +0x1504 */
    char pad_1508[0x4];
    int count;                /* +0x150C: rows in use */
    int columns;              /* +0x1510 */
    int field_1514;           /* +0x1514 */
    char pad_1518[0x40];
    unsigned char field_1558; /* +0x1558 */
} GridWork36;
A36_AT(GridWork36, cursor, 0xBD8);
A36_AT(GridWork36, field_14F8, 0x14F8);
A36_AT(GridWork36, field_1558, 0x1558);
A36_AT(GridWork36, buffer, 0xAB2);
A36_AT(GridWork36, field_1500, 0x1500);
A36_AT(GridWork36, texts, 0x890);
A36_AT(GridWork36, cells, 0xD18);
A36_AT(GridWork36, flags, 0x1048);
A36_AT(GridWork36, count, 0x150C);
A36_AT(GridWork36, columns, 0x1510);

typedef struct GridScreen36 {
    char pad_000[0x138];
    GridWork36 *work;         /* +0x138 */
} GridScreen36;
void func_088D428C(void *handles, void *texts, ScriptCmd36 *script, int arg);
void func_08A2A858(GridScreen36 *self, ScriptCmd36 *cmd);
void func_088D3B88(void *layout, void *handles, ScriptCmd36 *script, int arg, int count);
void func_088D3C24(void *layout, void *handles, ScriptCmd36 *script, int count);
void func_088DB10C(void *entry);
void func_0885C538(void *heap, void *block, int arg);
void func_088D46E4(Cursor36 *cursor, int a, int b, int c, int d, int e);
void func_08A2B328(GridScreen36 *self);
int func_089347E8(char *buffer, const char *format, ...);  /* sprintf */
const char *func_0887B500(void *messages, int id, int value);
void *func_08A2BA30(GridScreen36 *self, int kind, char **lines, int count);
void func_08A2A9E4(GridScreen36 *self, void *arg, int row);
int func_08A2A614(GridScreen36 *self, int row);
int func_08A2A6D8(GridScreen36 *self, int row);
int func_08A2A74C(GridScreen36 *self, int row);
int func_08A2A798(GridScreen36 *self, int row);
int func_08A2A7E4(GridScreen36 *self, int row);
void func_08A2A3A0(UnlockScreen36 *self, ScriptCmd36 *cmd);
int func_08A2A044(UnlockScreen36 *self, int row);
int func_08A2A04C(UnlockScreen36 *self, int row);
int func_08A2A11C(UnlockScreen36 *self, int row);
int func_08A2A1EC(UnlockScreen36 *self, int row);
int func_08A2A288(UnlockScreen36 *self, int row);
int func_08A2A2D0(UnlockScreen36 *self, int row);
int func_08A29BC0(unsigned short kind); /* resource index of a kind */
int func_08A2AD24(void *self, int chara); /* level of chara, -1: the highest */

/* Ids made known at a character level (D_08A2E8AC, ended by chara <= 0). */
typedef struct LevelGate36 {
    int chara;
    int level;
    unsigned int id;
} LevelGate36;
extern LevelGate36 D_08A2E8AC[];

/* ------------------------------------------------------------ shop/library pages (+0x6DC work) */

/* Record returned by the tab lookups (D_08A358F4). */
typedef struct TabRecord36 {
    char pad_00[0x2];
    unsigned short name;      /* +0x02: message id */
    unsigned short text;      /* +0x04: message id */
    unsigned short body;      /* +0x06: message id of the long text */
    unsigned short need;      /* +0x08: id that must be known, 0xFFFF: none */
    char pad_0A[0x2];
    unsigned short item_a;    /* +0x0C: 0xFFFF: none */
    signed char x_a;          /* +0x0E: offset of the mark for item a */
    signed char y_a;
    unsigned short item_b;    /* +0x10: 0xFFFF: none */
    signed char x_b;          /* +0x12 */
    signed char y_b;
} TabRecord36;

typedef TabRecord36 *(*TabLookup36)(int id);
extern TabLookup36 D_08A358F4[];

/* Work block of the tabbed pages (+0x6DC). */
/* A block taken from a heap, with its size. */
typedef struct HeapBlock36 {
    void *data;
    int size;
} HeapBlock36;

typedef struct TabWork36 {
    void **layout;            /* +0x00 */
    void **handles;           /* +0x04 */
    void *messages;           /* +0x08 */
    void *texts[0x16];        /* +0x0C */
    unsigned short shown[0x16]; /* +0x64 */
    Cursor36 cursor;          /* +0x90 */
    Cursor36 lines;           /* +0xBC: cursor over the lines of the long text */
    char pad_0E8[0x1C];
    char arrows[0x134];       /* +0x104: handed to func_088D4A68 */
    HeapBlock36 blocks[2];    /* +0x238 */
    char pad_248[0x80];
    int pos[0xA2];            /* +0x2C8: cursor position per list */
    unsigned short ids[2][0x50]; /* +0x550: per tab */
    char pad_690[0x8];
    int counts[2];            /* +0x698: entries per list */
    int field_6A0;            /* +0x6A0 */
    unsigned int flags;       /* +0x6A4 */
    int field_6A8;            /* +0x6A8 */
    char pad_6AC[0x4];
    int tab;                  /* +0x6B0 */
    char pad_6B4[0x8];
    int field_6BC;            /* +0x6BC */
    float view_6C0;           /* +0x6C0: camera values restored on close */
    float view_6C4;
    float view_6C8;
    float view_6CC;
    unsigned char field_6D0;  /* +0x6D0 */
    char pad_6D1[0x1];
    unsigned char field_6D2;  /* +0x6D2: arrows shown */
    unsigned char field_6D3;  /* +0x6D3 */
    unsigned char field_6D4;  /* +0x6D4: open the message box */
    unsigned char field_6D5;
    unsigned char field_6D6;
    unsigned char field_6D7;
} TabWork36;

typedef struct TabScreen36 {
    char pad_000[0x6DC];
    TabWork36 *work;          /* +0x6DC */
} TabScreen36;

#ifdef __cplusplus
/* Record of the current entry of a list, through the list's lookup (inlined). */
struct TabList36 {
    static TabRecord36 *Record(int list, int id) { return D_08A358F4[list](id); }
    static unsigned short Id(TabWork36 *work, int list) { return work->ids[list][work->cursor.index]; }
};
#endif
A36_AT(TabWork36, tab, 0x6B0);
A36_AT(TabWork36, lines, 0xBC);
A36_AT(TabWork36, arrows, 0x104);
A36_AT(TabWork36, field_6D2, 0x6D2);
A36_AT(TabWork36, blocks, 0x238);
A36_AT(TabWork36, field_6A0, 0x6A0);
A36_AT(TabWork36, flags, 0x6A4);
A36_AT(TabWork36, field_6BC, 0x6BC);
A36_AT(TabWork36, pos, 0x2C8);
A36_AT(TabWork36, ids, 0x550);
A36_AT(TabWork36, counts, 0x698);
A36_AT(TabWork36, field_6D6, 0x6D6);
int func_088B912C(void *table, int item);
TabRecord36 *func_08A3C164(unsigned short id);
const char *func_08A42E14(const char *line);  /* next line */
void func_088D4A68(void *arrows, Cursor36 *cursor, void *up, void *down, int a, int b, int c);
void func_08A2F600(TabScreen36 *self, const char *line, int row);
void func_088D4C64(Cursor36 *cursor, void **texts, unsigned short *shown, int a, int b, int c);
void *func_0887B3EC(void *messages, int kind, int arg, int id, int arg4, int arg5);

/* Battle setup tables (D_08A457EC +0x14C). */
/* A value shown with its text kind (8 bytes). */
typedef struct BattleValue36 {
    int value;
    unsigned short kind;      /* +0x04 */
    char pad_06[0x2];
} BattleValue36;

typedef struct BattleTable36 {
    void *layout[0x100];      /* +0x000 */
    void *handles[0x12B];     /* +0x400 */
    Cursor36 cursor;          /* +0x8AC */
    char pad_8D8[0x48];
    void *messages;           /* +0x920 */
    void *texts[0xC4];        /* +0x924: from 8 on, text and sprite pairs */
    BattleValue36 values[3];  /* +0xC34 */
    char pad_C4C[0x10];
    BattleValue36 shown[3];   /* +0xC5C */
    char buffer[0x1A8];       /* +0xC74 */
    char pad_E1C[0x18];
    void *field_E34;          /* +0xE34 */
    void *field_E38;          /* +0xE38 */
    char pad_E3C[0x4];
    unsigned int flags;       /* +0xE40 */
    int state;                /* +0xE44 */
    char pad_E48[0x8];
    int field_E50;            /* +0xE50 */
    int text_count;           /* +0xE54 */
    int text_ids[6];          /* +0xE58: index into D_08A455C0 */
    int field_E70[9];         /* +0xE70 */
    char text_E94[0x200];     /* +0xE94: split into lines by func_08A3C44C */
    char pad_1094[0x202];
    unsigned char field_1296; /* +0x1296 */
    char pad_1297[0x1];
    unsigned char field_1298; /* +0x1298 */
    unsigned char field_1299; /* +0x1299 */
    unsigned char field_129A; /* +0x129A */
    unsigned char field_129B; /* +0x129B */
    unsigned char field_129C; /* +0x129C */
} BattleTable36;
A36_AT(BattleTable36, cursor, 0x8AC);
A36_AT(BattleTable36, field_129A, 0x129A);
A36_AT(BattleTable36, texts, 0x924);
A36_AT(BattleTable36, shown, 0xC5C);
A36_AT(BattleTable36, buffer, 0xC74);
A36_AT(BattleTable36, text_count, 0xE54);
A36_AT(BattleTable36, messages, 0x920);
A36_AT(BattleTable36, field_E70, 0xE70);
typedef struct BattleGlobal36 {
    char pad_000[0x134];
    void *heap;               /* +0x134 */
    char pad_138[0x4];
    char sound[0x10];         /* +0x13C */
    BattleTable36 *table;     /* +0x14C */
} BattleGlobal36;
extern BattleGlobal36 *D_08A457EC;

void func_08A41818(BattleGlobal36 *battle, int arg);
void func_08A418E8(BattleGlobal36 *battle);

/* Camera of the main module (D_089D5EC0 +0x7E0). */
typedef struct CameraHost36 {
    char pad_000[0x710];
    void *heap;               /* +0x710 */
    char pad_714[0xCC];
    char camera[0x4];         /* +0x7E0 */
} CameraHost36;
extern CameraHost36 *D_089D5EC0;
void func_0887358C(void *camera, float value);
void func_08873594(void *camera, float a, float b);
void func_088735BC(void *camera, float value);
void func_088DA060(void *model);  /* free */
void *func_08A412A8(BattleGlobal36 *battle, int kind, char **lines, int count);
void *func_0887E534(void *text);  /* sprite of a text */
void func_08A41940(BattleGlobal36 *battle);

/* Pages of the tabbed screen (D_08A35878, 0x28-byte rows). */
typedef struct ListPage36 {
    int list;                 /* +0x00: index into the work block's lists */
    int kind;                 /* +0x04: message kind */
    int field_08;
    int entry;                /* +0x0C: index into the battle table */
    int field_10;             /* +0x10 */
    int field_14;             /* +0x14 */
    char pad_18[0x10];
} ListPage36;
A36_SIZE(ListPage36, 0x28);
extern ListPage36 D_08A35878[];
void func_08A41980(BattleGlobal36 *battle, int arg);
void func_08A4146C(BattleGlobal36 *battle, int arg);
void func_08A2EB10(TabScreen36 *self);
void func_08A2F430(TabScreen36 *self, ListPage36 *page);
void func_08A2FA6C(TabScreen36 *self, ListPage36 *page);
void func_08A327CC(TabScreen36 *self);
void func_08A419A4(BattleGlobal36 *battle);

/* Object system of the main module (D_089D4960): its heap at +4. */
typedef struct Objects36 {
    void *field_00;
    void *heap;               /* +0x04 */
    char pad_08[0x58];
    signed char busy;         /* +0x60 */
    unsigned char pending;    /* +0x61 */
} Objects36;
extern Objects36 *D_089D4960;
void func_08846628(void *object);   /* stop */

/* ------------------------------------------------------------ picker (+0x48 work) */

typedef struct PickEntry36 {
    char data[0x14];
} PickEntry36;

typedef struct PickList36 PickList36;

/* Model info (model +0x00): part ids from +0x1C, ended by a negative one. */
typedef struct PickModelInfo36 {
    char pad_00[0x1C];
    short parts[4];           /* +0x1C */
} PickModelInfo36;

typedef struct PickModel36 {
    PickModelInfo36 *info;
} PickModel36;

typedef struct PickWork36 {
    char pad_000[0x20];
    void *heap;               /* +0x20 */
    char pad_024[0x4];
    PickModel36 *model;       /* +0x28 */
    PickModel36 *parts[4];    /* +0x2C */
    PickList36 *list;         /* +0x3C: the entries' list object */
    PickEntry36 entries[13];  /* +0x40 */
    int count;                /* +0x144 */
    int index;                /* +0x148 */
    char pad_14C[0x18];
    int mode;                 /* +0x164 */
    int mode_count;           /* +0x168 */
    char pad_16C[0x4];
    int state;                /* +0x170 */
    unsigned int flags;       /* +0x174 */
    char pad_178[0x4];
    unsigned short modes[13]; /* +0x17C: 0xFFFF: empty */
    char pad_196[0x6];
    unsigned char forward;    /* +0x19C */
} PickWork36;

typedef struct Picker36 {
    char pad_00[0x48];
    PickWork36 *work;         /* +0x48 */
} Picker36;

A36_AT(PickWork36, count, 0x144);
A36_AT(PickWork36, mode, 0x164);
A36_AT(PickWork36, forward, 0x19C);

void func_08A34AB4(Picker36 *self, PickEntry36 *entry);
int func_08A31F34(Picker36 *self);
void *func_08831320(unsigned int size);      /* operator new */
void func_08805C6C(PickList36 *list, void *source);
/* Actor record of an id (func_088470B8). */
typedef struct Actor36 {
    char pad_00[0x6];
    signed char kind;         /* +0x06 */
} Actor36;
Actor36 *func_088470B8(int id);
void *func_088470DC(int table, int id, int arg);
int func_08A3200C(Picker36 *self);
int func_08A32300(Picker36 *self, int mode);
PickModel36 *func_088443A0(Objects36 *objects, int id, void *heap, int flags);
int func_08844BC4(int id);
void func_08A31688(Picker36 *self);

/* ------------------------------------------------------------ clock / choice screen (+0xF64 work) */

typedef struct ClockLink36 {
    int field_00;
    char block[0x2D];         /* +0x04: handed to func_0889F830 */
    unsigned char field_31;   /* +0x31 */
} ClockLink36;

enum { CLOCK_TEXT = 0x15, CLOCK_SPRITE = 0x16 };

typedef struct ClockWork36 {
    void *layout;             /* +0x000 */
    void **handles;           /* +0x004 */
    void *messages;           /* +0x008 */
    void *texts[0x17];        /* +0x00C: CLOCK_TEXT, CLOCK_SPRITE: the time */
    char pad_068[0x4];
    int choice;               /* +0x06C */
    char pad_070[0x84];
    int row;                  /* +0x0F4: index into list */
    char pad_0F8[0x24];
    ScriptCmd36 *script;      /* +0x11C */
    int field_120;            /* +0x120 */
    char pad_124[0x40];
    char lines[3][0x40];      /* +0x164: hours, minutes, seconds */
    char pad_224[0x4EC];
    unsigned short list[0x21]; /* +0x710 */
    unsigned short items[0x200]; /* +0x752 */
    char pad_B52[0x202];
    ClockLink36 *link;        /* +0xD54 */
    char pad_D58[0x4];
    int field_D5C;            /* +0xD5C: -1: none */
    char pad_D60[0x4];
    const char *name;         /* +0xD64 */
    void *block;              /* +0xD68: from D_089D5EC0's heap */
    char pad_D6C[0x4];
    int entry;                /* +0xD70: 0xFF: none */
    int field_D74;            /* +0xD74 */
    int field_D78;            /* +0xD78 */
    char pad_D7C[0x4];
    int session;              /* +0xD80: -1: none */
    char pad_D84[0x4];
    int field_D88;            /* +0xD88 */
    char pad_D8C[0x8];
    int field_D94;            /* +0xD94 */
    int state;                /* +0xD98 */
    unsigned int shown;       /* +0xD9C: seconds shown */
    char pad_DA0[0x14];
    unsigned char field_DB4;  /* +0xDB4 */
    unsigned char field_DB5;  /* +0xDB5 */
    unsigned char field_DB6;  /* +0xDB6 */
    unsigned char field_DB7;  /* +0xDB7 */
    char pad_DB8[0x4];
    char table[0x4];          /* +0xDBC: the screen's entries, right after the work block */
} ClockWork36;
A36_AT(ClockWork36, name, 0xD64);
A36_AT(ClockWork36, link, 0xD54);
A36_AT(ClockWork36, session, 0xD80);
A36_AT(ClockWork36, field_D88, 0xD88);
A36_AT(ClockWork36, state, 0xD98);
A36_AT(ClockWork36, block, 0xD68);
A36_AT(ClockWork36, list, 0x710);
A36_AT(ClockWork36, items, 0x752);
A36_AT(ClockWork36, row, 0xF4);
A36_AT(ClockWork36, table, 0xDBC);
A36_AT(ClockWork36, script, 0x11C);
A36_AT(ClockWork36, lines, 0x164);
A36_AT(ClockWork36, shown, 0xD9C);
A36_AT(ClockWork36, field_DB4, 0xDB4);

typedef struct ClockScreen36 {
    char pad_000[0xF64];
    ClockWork36 *work;        /* +0xF64 */
} ClockScreen36;

/* ------------------------------------------------------------ item list screen (+0x1274 work) */

/* A loading slot of the item list screen (0x90 bytes). */
typedef struct ShopSlot36 {
    int field_00;
    int field_04;
    int field_08;
    char request[0x80];       /* +0x0C */
    int state;                /* +0x8C: 0 idle, 1 loading, 2 loaded, 3 shown */
} ShopSlot36;
A36_SIZE(ShopSlot36, 0x90);

typedef struct ShopWork36 {
    void **heap_block;        /* +0x00: [0] is the heap */
    void *block;              /* +0x04: from D_089D5EC0's heap */
    void **layout;            /* +0x08 */
    void **handles;           /* +0x0C */
    char pad_010[0x4];
    void *texts[0x21];        /* +0x14 */
    unsigned short shown[0x21]; /* +0x98 */
    char pad_0DA[0x2E];
    Cursor36 cursor;          /* +0x108 */
    char arrows[0x31C];       /* +0x134 */
    char number[0x40];        /* +0x450 */
    ShopSlot36 slots[16];     /* +0x490 */
    unsigned short marks[0x20]; /* +0xD90: model shown per cell, 0xFFFF: none */
    unsigned short list[0x200]; /* +0xDD0 */
    char pad_11D0[0x80];
    int field_1250;           /* +0x1250 */
    int state;                /* +0x1254 */
    int list_count;           /* +0x1258 */
    int item;                 /* +0x125C */
    char pad_1260[0x8];
    int slot_count;           /* +0x1268 */
} ShopWork36;
A36_AT(ShopWork36, slots, 0x490);
A36_AT(ShopWork36, slot_count, 0x1268);
A36_AT(ShopWork36, list_count, 0x1258);

/* Item of the list screen (func_088CC364). */
typedef struct ShopItem36 {
    char pad_00[0x2];
    unsigned short model;     /* +0x02 */
    unsigned short id;        /* +0x04 */
    unsigned short need;      /* +0x06: id that must be known */
} ShopItem36;
int func_088CC3B0(int id);           /* exists */
extern void *D_089D6998;
void *func_0887D868(void *fonts, int a, const char *text, int b, int c, int d, int e, int f);
ShopItem36 *func_088CC364(int id);

#ifdef __cplusplus
/* Bit test on the known set, base first (inlined). */
struct Known36 {
    static int Of(unsigned int id) {
        unsigned int *bits = D_089D726C;
        return (bits[id >> 5] & (1 << (id & 0x1F))) != 0;
    }
};
#endif
A36_AT(ShopWork36, handles, 0xC);
A36_AT(ShopWork36, cursor, 0x108);
A36_AT(ShopWork36, marks, 0xD90);
A36_AT(ShopWork36, list, 0xDD0);

typedef struct ShopScreen36 {
    char pad_0000[0x1274];
    ShopWork36 *work;         /* +0x1274 */
    int mode;                 /* +0x1278: 1: the item goes to func_08A4D76C */
} ShopScreen36;

/* ------------------------------------------------------------ name list screen (+0x2A14 work) */

typedef struct TextObj36 {
    char pad_00[0x10];
    unsigned int flags;       /* +0x10 */
} TextObj36;

/* An entry of the name list (0x10 bytes from +0x2638). */
typedef struct NameEntry36b {
    char pad_00[0x4];
    int id;                   /* +0x04 */
    char pad_08[0x8];
} NameEntry36b;

/* A record of the name list (0x10C bytes from +0x4B8). */
typedef struct NameRecord36 {
    int chara;                /* +0x000: -1: all characters */
    int field_04;
    int field_08;             /* +0x008 */
    int values[0x40];         /* +0x00C: per kind */
} NameRecord36;
A36_SIZE(NameRecord36, 0x10C);

typedef struct NameWork36 {
    void **layout;            /* +0x0000 */
    Model36 **handles;        /* +0x0004 */
    void *messages;           /* +0x0008 */
    TextObj36 *texts[0x20];   /* +0x000C: text and sprite per row */
    unsigned short shown[0x28]; /* +0x008C: two per row */
    int top;                  /* +0x00DC */
    char pad_00E0[0x34];
    char field_114[0x298];    /* +0x0114: handed to func_08A41454 */
    NameRecord36 all;         /* +0x03AC */
    NameRecord36 records[0x20]; /* +0x04B8 */
    NameEntry36b entries[0x20]; /* +0x2638 */
    char block_2838[0x138];   /* +0x2838 */
    unsigned short ids[0x40]; /* +0x2970 */
    int count;                /* +0x29F0 */
    int record_count;         /* +0x29F4 */
    int cells;                /* +0x29F8 */
    int field_29FC;           /* +0x29FC */
    void *field_2A00;         /* +0x2A00 */
    int field_2A04;           /* +0x2A04 */
    int field_2A08;           /* +0x2A08 */
    unsigned char field_2A0C; /* +0x2A0C */
    unsigned char field_2A0D; /* +0x2A0D */
    char pad_2A0E[0x2];
} NameWork36;
A36_AT(NameWork36, top, 0xDC);
A36_AT(NameWork36, entries, 0x2638);
A36_AT(NameWork36, ids, 0x2970);
A36_AT(NameWork36, cells, 0x29F8);
A36_AT(NameWork36, all, 0x3AC);
A36_AT(NameWork36, block_2838, 0x2838);
A36_AT(NameWork36, field_2A00, 0x2A00);
A36_AT(NameWork36, field_114, 0x114);
A36_SIZE(NameWork36, 0x2A10);

typedef struct NameScreen36 {
    char pad_0000[0x2A14];
    NameWork36 *work;         /* +0x2A14 */
} NameScreen36;

extern void *D_089D7830;
void func_088D0984(void *loader, void *entry, int kind, unsigned short id);
void func_088D9D24(Model36 *model);

/* List state of the menu (D_09B6DA68). */
typedef struct ListState36 {
    int mode;                 /* +0x00 */
    int index;                /* +0x04 */
    int count;                /* +0x08 */
    int field_0C;
    int field_10;
    int field_14;
    int field_18;
    char pad_1C[0x1];
    unsigned char saved;      /* +0x1D */
    unsigned char field_1E;
    unsigned char field_1F;
    unsigned char field_20;
    char pad_21[0x3];
    int field_24;
    int field_28;
    int field_2C;
} ListState36;

extern ListState36 D_09B6DA68;

/* ------------------------------------------------------------ event list screen (+0x13C list) */

typedef struct EventList36 {
    unsigned short ids[0x200];  /* +0x000 */
    void *buffer;               /* +0x400 */
    int field_404;
    int field_408;              /* +0x408 */
} EventList36;

typedef struct EventScreen36 {
    char pad_000[0x100];
    char task[0x34];            /* +0x100 */
    void *heap;                 /* +0x134 */
    char pad_138[0x4];
    EventList36 *list;          /* +0x13C */
} EventScreen36;

/* Game options state (D_09B6DCE8). */
typedef struct OptionState36 {
    int field_00;
    int field_04;
    int field_08;
    char pad_0C[0x4];
    unsigned char field_10;
    unsigned char field_11;
    unsigned char field_12;
    unsigned char field_13;
    unsigned char field_14;
    unsigned char field_15;
} OptionState36;
extern OptionState36 D_09B6DCE8;

/* script players and model/text switches (main module) */
void func_088D3F00(void *layout, void *handles, const short *script);
void func_088D44EC(void *handles, void *texts, const unsigned short *script);
void func_088D3EA0(void *layout, void *handles, const short *script);
void func_0887BA5C(void *text);    /* free */
void func_088D9CD8(void *model);   /* show */
void func_088D9F5C(void *model, int motion, char arg);
void func_088DA0A8(void *model, int part, float *matrix, int *color);
void func_088D9B84(void *model, float *matrix);
void func_088D9AF4(void *model, int *color);
void func_088D9CFC(void *model);   /* hide */
void func_0887B924(void *text);    /* show */
void func_0887B944(void *text);    /* hide */

/* menu root (0x08A1E168..) */
void func_08A1E168(MenuRoot36 *self);
int func_08A1E1A8(MenuRoot36 *self);
int func_08A20378(MenuRoot36 *self);
extern MenuRoot36 *D_08A23FF0;
void func_08A1E5D4(MenuRoot36 *self, int mode, void *layout, void *handles, void *texts, void *block);
void func_08A1E6B4(MenuRoot36 *self, int mode, void *layout, void *handles, void *texts, void *block);
void func_08A1EA3C(MenuRoot36 *self, void *handles);
int func_08A1D8E4(MenuRoot36 *self);
void func_08A1F108(MenuRoot36 *self);
int func_08A1F550(void);
void func_08A1F1DC(MenuRoot36 *self);
void func_08A2005C(MenuRoot36 *self, void **handles);
void func_08A1CD14(MenuRoot36 *self);
void func_08A1C0B8(MenuRoot36 *self);
void func_08A1C2F8(MenuRoot36 *self);

#ifdef __cplusplus
}
#endif

#endif
