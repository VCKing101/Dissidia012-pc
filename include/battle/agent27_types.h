#ifndef BATTLE_AGENT27_TYPES_H
#define BATTLE_AGENT27_TYPES_H

/* A memory stream over a byte buffer: open flag, size, base and position. */
typedef struct MemStream {
    unsigned char open;     /* 0x00 */
    char pad01[3];
    int size;               /* 0x04 */
    char *base;             /* 0x08 */
    int pos;                /* 0x0C */
} MemStream;

/* The object reset by func_08B19628 and opened by func_08B197E0. */
typedef struct LoadJob {
    unsigned char active;   /* 0x00 */
    char pad01[7];
    int field_08;           /* 0x08 */
    int field_0C;
    int field_10;
    int field_14;
    char pad18[4];
    float field_1C;
    unsigned char field_20;
    char pad21[3];
    void *field_24;
    void *field_28;
    void *field_2C;
    int kind;               /* 0x30 */
    int field_34;
    int field_38;
    void *field_3C;
    char pad40[4];
} LoadJob;

/* A resource with a kind at 0x0C and flags at 0x20. */
typedef struct Resource {
    char pad00[0xC];
    short kind;             /* 0x0C */
    char pad0E[0x12];
    unsigned short flags;   /* 0x20 */
} Resource;

/* A holder: active flag, a value and an owned resource (func_08B1A0D8 resets it). */
typedef struct ResHolder {
    unsigned char active;   /* 0x00 */
    char pad01[3];
    int field_04;
    Resource *res;          /* 0x08 */
    char pad0C[4];
} ResHolder;

/* One 0x18-byte entry of a LoadSet's record list. */
typedef struct LoadRecord {
    int value;              /* 0x00 */
    char pad04[0x14];
} LoadRecord;

/* Nine load jobs sharing one resource holder. */
typedef struct LoadSet {
    unsigned char active;   /* 0x000 */
    char pad01[3];
    int state;              /* 0x004 */
    int field_08;           /* 0x008 */
    ResHolder holder;       /* 0x00C */
    LoadJob jobs[9];        /* 0x01C */
    int count;              /* 0x280 */
    LoadRecord records[16]; /* 0x284 */
} LoadSet;

/* A node linked into a NodeList through next (0x4C) and prev (0x54). */
typedef struct ListNode {
    char pad00[0x10];
    unsigned int flags;     /* 0x10 */
    char pad14[0x38];
    struct ListNode *next;  /* 0x4C */
    char pad50[4];
    struct ListNode *prev;  /* 0x54 */
} ListNode;

typedef struct NodeList {
    char pad00[0x30];
    ListNode *head;         /* 0x30 */
    ListNode *tail;         /* 0x34 */
    int count;              /* 0x38 */
} NodeList;

/* A stream slot reset by func_08B1AAF8: a resource, its list and node. */
typedef struct StreamSlot {
    unsigned char active;   /* 0x00 */
    char pad01[3];
    int state;              /* 0x04 */
    char pad08[4];
    unsigned int field_0C;
    int field_10;
    int field_14;
    int field_18;
    char pad1C[4];
    Resource *res;          /* 0x20 */
    NodeList *list;         /* 0x24 */
    ListNode *node;         /* 0x28 */
    int field_2C;
    int field_30;
    int field_34;
    int field_38;
    char pad3C[4];
} StreamSlot;

/* 128 stream slots sharing one resource holder. */
typedef struct StreamSet {
    unsigned char active;   /* 0x0000 */
    char pad01[3];
    int state;              /* 0x0004 */
    int phase;              /* 0x0008 */
    float field_0C;
    int field_10;
    ResHolder holder;       /* 0x0014 */
    StreamSlot slots[0x80]; /* 0x0024 */
    int count;              /* 0x2024 */
    LoadRecord records[16]; /* 0x2028 */
} StreamSet;

/* The sound manager: a LoadSet at 0x64 and a StreamSet at 0x68. */
typedef struct SoundManager {
    unsigned char active;   /* 0x00 */
    char pad01[3];
    int state;              /* 0x04 */
    int field_08;           /* 0x08 */
    char pad0C[8];
    unsigned char field_14[0x30];   /* 0x14 */
    unsigned char ready;    /* 0x44 */
    char pad45[0x1F];
    LoadSet *loads;         /* 0x64 */
    StreamSet *streams;     /* 0x68 */
} SoundManager;

/* A model slot: a list owner at 0x04 and a node linked into it at 0x08. */
typedef struct ModelSlot {
    unsigned char active;   /* 0x00 */
    char pad01[3];
    NodeList *list;         /* 0x04 */
    ListNode *node;         /* 0x08 */
    unsigned char field_0C;
    char pad0D[3];
    float field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
} ModelSlot;

/* An effect slot with a 9 x 2 grid of handles. */
typedef struct EffectSlot {
    unsigned char active;   /* 0x00 */
    char pad01[3];
    Resource *res;          /* 0x04 */
    int grid[9][2];         /* 0x08 */
    unsigned char field_50;
    char pad51[3];
    float field_54;
    int field_58;
    int field_5C;
    int field_60;
} EffectSlot;

/* Three resource tables: 4 + 50 + 50 handles. */
typedef struct ResTables {
    unsigned char active;   /* 0x000 */
    char pad001[3];
    void *small[4];         /* 0x004 */
    void *first[50];        /* 0x014 */
    void *second[50];       /* 0x0DC */
    unsigned char field_1A4;
    char pad1A5[3];
    int firstCount;         /* 0x1A8 */
    int secondCount;        /* 0x1AC */
    struct BattleCtx *ctx;  /* 0x1B0 */
    int field_1B4;
} ResTables;

/* One 0x24-byte entry of the battle context table at 0xA180. */
typedef struct CtxEntry {
    char pad00[0x18];
    int field_18;
    char pad1C[4];
    unsigned char kind;     /* 0x20 */
    char pad21[3];
} CtxEntry;

typedef struct BattleCtx {
    char pad0000[0xA180];
    CtxEntry entries[200];      /* 0xA180 */
    unsigned int entryCount;    /* 0xBDA0 */
    char padBDA4[8];
    int limit;                  /* 0xBDAC */
} BattleCtx;

/* A scene: lights at 0x0C, models at 0x34, effects at 0x58. */
typedef struct SceneParts {
    unsigned char active;   /* 0x000 */
    char pad001[3];
    int field_004;
    char pad008[4];
    char lights[0x28];      /* 0x00C */
    char models[0x24];      /* 0x034 */
    char effects[0x1BC];    /* 0x058 */
    int field_214;
    int field_218;
    int field_21C;
} SceneParts;

/* A slot with two handles at 0x08 (reset by func_08B2C2F8). */
typedef struct PairSlot {
    unsigned char active;   /* 0x00 */
    char pad01[3];
    int field_04;
    int handles[2];         /* 0x08 */
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
} PairSlot;

/* A battle panel with two links at 0x18C. */
typedef struct PanelLink {
    char pad00[0x14];
    unsigned char flags;    /* 0x14 */
} PanelLink;

typedef struct Panel {
    unsigned char active;   /* 0x000 */
    char pad001[3];
    int field_004;
    int field_008;
    int field_00C;
    int field_010;
    char pad014[0x150];
    int field_164;
    int field_168;
    int field_16C;
    int field_170;
    int field_174;
    int field_178;
    int field_17C;
    int field_180[2];
    int field_188;
    PanelLink *links[2];    /* 0x18C */
    int field_194;
    int field_198;
    int field_19C;
    int field_1A0;
    int field_1A4[2];
    int field_1AC[2];
    int field_1B4;
    int field_1B8;
    int field_1BC;
    int field_1C0;
    int field_1C4;
    int field_1C8;
    int field_1CC;
    int field_1D0;
} Panel;

#ifdef __cplusplus
/* The first 48 virtuals of a battle actor, named by vtable offset. */
#define AGENT27_ACTOR_VIRTUALS \
    virtual int slot_008(); \
    virtual int slot_010(); \
    virtual int slot_018(); \
    virtual int slot_020(); \
    virtual int slot_028(); \
    virtual int slot_030(); \
    virtual int slot_038(); \
    virtual int slot_040(); \
    virtual int slot_048(); \
    virtual int slot_050(); \
    virtual int slot_058(); \
    virtual int slot_060(); \
    virtual int slot_068(); \
    virtual int slot_070(); \
    virtual int slot_078(); \
    virtual int slot_080(); \
    virtual int slot_088(); \
    virtual int slot_090(); \
    virtual int slot_098(); \
    virtual int slot_0A0(); \
    virtual int slot_0A8(); \
    virtual int slot_0B0(); \
    virtual int slot_0B8(); \
    virtual int slot_0C0(); \
    virtual int slot_0C8(); \
    virtual int slot_0D0(); \
    virtual int slot_0D8(); \
    virtual int slot_0E0(); \
    virtual int slot_0E8(); \
    virtual int slot_0F0(); \
    virtual int slot_0F8(); \
    virtual int slot_100(); \
    virtual int slot_108(); \
    virtual int slot_110(); \
    virtual int slot_118(); \
    virtual int slot_120(); \
    virtual int slot_128(); \
    virtual int slot_130(); \
    virtual int slot_138(); \
    virtual int slot_140(); \
    virtual int slot_148(); \
    virtual int slot_150(); \
    virtual int slot_158(); \
    virtual int slot_160(); \
    virtual int slot_168(); \
    virtual int slot_170(); \
    virtual int slot_178(); \
    virtual int slot_180();

/* An object with 0x544 bytes of data before its vptr; virtual N sits at
 * vtable offset 8 * (N + 1), named by that offset. */
class Actor544 {
public:
    char data[0x544];
    virtual int slot_008();
    virtual int slot_010();
    virtual int slot_018();
    virtual int slot_020();
    virtual int slot_028();
    virtual int slot_030();
    virtual int slot_038();
    virtual int slot_040();
    virtual int slot_048();
    virtual int slot_050();
    virtual int slot_058();
    virtual int slot_060();
    virtual int slot_068();
    virtual int slot_070();
    virtual int slot_078();
    virtual int slot_080();
    virtual int slot_088();
    virtual int slot_090();
    virtual int slot_098();
    virtual int slot_0A0();
    virtual int slot_0A8();
    virtual int slot_0B0();
    virtual int slot_0B8();
    virtual int slot_0C0();
    virtual int slot_0C8();
    virtual int slot_0D0();
    virtual int slot_0D8();
    virtual int slot_0E0();
    virtual int slot_0E8();
    virtual int slot_0F0();
    virtual int slot_0F8();
    virtual int slot_100();
    virtual int slot_108();
    virtual int slot_110();
    virtual int slot_118();
    virtual int slot_120();
    virtual int slot_128();
    virtual int slot_130();
    virtual int slot_138();
    virtual int slot_140();
    virtual int slot_148();
    virtual int slot_150();
    virtual int slot_158();
    virtual int slot_160();
    virtual int slot_168();
    virtual int slot_170();
    virtual int slot_178();
    virtual int slot_180();
};

/* The same layout with the position vector at 0x80 named. */
class ActorPos544 {
public:
    char pad000[0x80];
    float pos[4];           /* 0x080 */
    char pad090[0x330];
    float field_3C0;        /* 0x3C0 */
    char pad3C4[0x180];
    virtual int slot_008();
    virtual int slot_010();
    virtual int slot_018();
    virtual int slot_020();
    virtual int slot_028();
    virtual int slot_030();
    virtual int slot_038();
    virtual int slot_040();
    virtual int slot_048();
    virtual int slot_050();
    virtual int slot_058();
    virtual int slot_060();
    virtual int slot_068();
    virtual int slot_070();
    virtual int slot_078();
    virtual int slot_080();
    virtual int slot_088();
    virtual int slot_090();
    virtual int slot_098();
    virtual int slot_0A0();
    virtual int slot_0A8();
    virtual int slot_0B0();
    virtual int slot_0B8();
    virtual int slot_0C0();
    virtual int slot_0C8();
    virtual int slot_0D0();
    virtual int slot_0D8();
    virtual int slot_0E0();
    virtual int slot_0E8();
    virtual int slot_0F0();
    virtual int slot_0F8();
    virtual int slot_100();
    virtual int slot_108();
    virtual int slot_110();
    virtual int slot_118();
    virtual int slot_120();
    virtual int slot_128();
    virtual int slot_130();
    virtual int slot_138();
    virtual int slot_140();
    virtual int slot_148();
    virtual int slot_150();
    virtual int slot_158();
    virtual int slot_160();
    virtual int slot_168();
    virtual int slot_170();
    virtual int slot_178();
    virtual int slot_180();
};
#endif

#endif
