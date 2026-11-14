#ifndef BATTLE_AGENT28_TYPES_H
#define BATTLE_AGENT28_TYPES_H

/* Types identified by agent28 (battle 0x089DD6B4-0x08A22D38). */

/* Doubly linked node at the start of each pooled record (0x54 bytes). */
typedef struct PoolLink {
    struct PoolLink *next;
    struct PoolLink *prev;
} PoolLink;

/* Free-ring slot: a record pointer and the next slot. */
typedef struct PoolSlot {
    PoolLink *data;
    struct PoolSlot *next;
} PoolSlot;

/* Record pool (same object as BattleRecordPool in agent1_types.h), seen
 * through its active list and free ring. */
typedef struct RecordPool {
    unsigned char open;
    unsigned char pad_01[3];
    void *owner;
    void *context;
    unsigned char models[0x18];
    unsigned char fixed[3][0x54];
    PoolSlot slots[14];
    unsigned char records[14][0x54];
    PoolSlot *free_read;
    PoolSlot *free_write;
    PoolLink *head;
    PoolLink *tail;
    int count;
    void *vtable;
} RecordPool;

/* Fighter display set at +0x1DC of the battle root (close 0x089E7AFC). */
typedef struct FighterDisplay {
    unsigned char data[0xDC];
} FighterDisplay;

typedef struct DisplayGauge {
    unsigned char data[0x28];
} DisplayGauge;

typedef struct DisplaySet {
    unsigned char open;
    unsigned char pad_001[3];
    unsigned char pool[0x648];
    void *texture;
    FighterDisplay displays[2];
    DisplayGauge gauges[2];
    unsigned char effect[4];
} DisplaySet;

/* Phase timer (tick 0x089E8698, cue start 0x089E8BE4). */
typedef struct PhaseTimer {
    unsigned char pad_00[0x10];
    float time;
    unsigned char pad_14[0x10];
    unsigned char cue[0x18];
    int kind;
} PhaseTimer;

/* Card item (0x84C bytes, two per card pair at +0x1C; reset 0x089EA680):
 * sixteen 0x78-byte records in a free ring and an active list. */
/* Effect attached to a model: kept in the model's effect list. */
typedef struct ModelEffect {
    unsigned char pad_00[0x10];
    unsigned int flags;
    unsigned char pad_14[0x38];
    struct ModelEffect *next;
    unsigned char pad_50[4];
    struct ModelEffect *prev;
} ModelEffect;

typedef struct EffectModel {
    unsigned char pad_00[0x30];
    ModelEffect *head;
    ModelEffect *tail;
    int count;
} EffectModel;

/* Card definition copied into a record by func_089EBDDC. */
typedef struct CardDef {
    unsigned int side;
    int slot;
    unsigned char kind;
    unsigned char style;
    unsigned char pad_0a[2];
    int value;
    int params[3];
    unsigned char mark;
    unsigned char pad_1d[3];
    int timer;
    unsigned char sound;
    unsigned char voice;
    unsigned char pad_26[2];
    int extra;
} CardDef;

typedef struct CardRecord {
    struct CardRecord *next;
    struct CardRecord *prev;
    unsigned char open;
    unsigned char pad_09[3];
    int state;
    EffectModel *model;
    ModelEffect *effects[4];
    unsigned char pad_24[4];
    float time;
    unsigned char held;
    unsigned char landed;
    unsigned char skip;
    unsigned char pad_2f;
    int target_slot;
    float position;
    float blend;
    void *owner;
    void *source;
    unsigned int side;
    int slot;
    unsigned char kind;
    unsigned char style;
    unsigned char pad_4e[2];
    int value;
    int params[3];
    unsigned char mark;
    unsigned char pad_61[3];
    int timer;
    unsigned char sound;
    unsigned char voice;
    unsigned char pad_6a[2];
    int extra;
    unsigned char pad_70[4];
    void *vtable;
} CardRecord;

typedef struct CardSlot {
    CardRecord *record;
    struct CardSlot *next;
} CardSlot;

typedef struct CardItemState {
    unsigned char open;
    unsigned char pad_01[3];
    int index;
    CardSlot slots[16];
    CardRecord records[16];
    CardSlot *free_read;
    CardSlot *free_write;
    CardRecord *head;
    CardRecord *tail;
    int count;
    unsigned char indicator[0x10];
    unsigned char flags[2];
    unsigned char shown;
    unsigned char pad_82f;
    int field_830;
    float field_834;
    int field_838;
    int field_83c;
    int field_840;
    int field_844;
    void *vtable;
} CardItemState;

typedef struct FrameClock {
    unsigned char pad_00[0x18];
    float frame_scale;
} FrameClock;

typedef struct BattleClock {
    unsigned char pad_00[0x5C];
    unsigned char timer[4];
} BattleClock;

/* Banner timer (update 0x089ED0B8, force 0x089ED23C). */
typedef struct BannerTimer {
    unsigned char open;
    unsigned char pad_01[0xB];
    int start;
    int state;
    float time;
    unsigned char pad_18[3];
    unsigned char forced;
    int started_at;
} BannerTimer;

/* Model bank (close 0x089ED974, select 0x089EDCD4). */
typedef struct ModelBank {
    unsigned char open;
    unsigned char pad_01[0xF];
    void *models[9];
    void *frame;
    void *icon;
    int owner;
    void *source;
} ModelBank;

/* Battle root reached through D_089D6A10->root (0x7A4). */
typedef struct RootHolder {
    unsigned char pad_000[0x7A4];
    void *root;
} RootHolder;

/* Battle phase (dispatcher 0x089F0500): state at 8, timer at 0x10, cue icon
 * at 0x1C, round counters at 0x148/0x14C. */
typedef struct PhaseState {
    void *vtable;
    unsigned char pad_04[4];
    unsigned int state;
    unsigned char ready;
    unsigned char pad_0d[3];
    float time;
    unsigned char pad_14[8];
    unsigned char cue[0x12C];
    int wins;
    int losses;
    unsigned char pad_150[4];
    void *banner;
} PhaseState;

/* Triple-model widget (load 0x089F3B48, update 0x089F39E4). */
typedef struct WidgetModel {
    unsigned char pad_00[0xC];
    short motion;
    short phase;
} WidgetModel;

typedef struct TripleModel {
    unsigned char open;
    unsigned char pad_01[3];
    WidgetModel *base;
    WidgetModel *fill;
    WidgetModel *cap;
    int owner;
    void *source;
} TripleModel;

/* Duel phase (step dispatcher 0x089F4800). */
typedef struct DuelGauge {
    unsigned char data[0x38];
} DuelGauge;

typedef struct DuelPartSet {
    unsigned char data[0x140];
} DuelPartSet;

typedef struct DuelBadge {
    unsigned char data[0x34];
} DuelBadge;

typedef struct DuelPhaseData {
    void *vtable;
    unsigned char pad_04[4];
    int state;
    unsigned char ready;
    unsigned char shown;
    unsigned char pad_0e[2];
    float time;
    unsigned char done[2];
    unsigned char pad_16[2];
    int scores[2];
    unsigned char widget[0x1C];
    unsigned char timer[0x2C];
    DuelGauge gauges[2];
    unsigned char pad_d8[8];
    DuelPartSet parts[2];
    DuelBadge badges[2];
    int wins;
    int losses;
    unsigned char pad_3d0[4];
    void *banner;
} DuelPhaseData;

/* Pad rumble set: three per-controller rumble slots. */
typedef struct RumbleSlot {
    unsigned char data[0x48];
} RumbleSlot;

typedef struct RumbleSet {
    unsigned char open;
    unsigned char pad_01[3];
    RumbleSlot slots[3];
    int states[3];
    int strengths[3];
    int active;
    float delay;
    float time;
    unsigned char stopping;
} RumbleSet;

/* Score tag (attach 0x089FC7E0, load 0x089FC89C, fade 0x089FCA0C). */
typedef struct ScoreTag {
    unsigned char open;
    unsigned char pad_01[3];
    void *model;
    void *tag;
    float fade;
    int owner;
    void *source;
} ScoreTag;

/* Side cue (open 0x08A0189C, attach 0x08A01948). */
typedef struct SideCue {
    unsigned char open;
    unsigned char pad_01[3];
    void *model;
    void *mark;
    unsigned char closing;
    unsigned char pad_0d[3];
    unsigned int side;
    unsigned int kind;
    void *source;
    void *layout;
    unsigned char shared;
} SideCue;

/* Side flag (load 0x08A0641C, raise 0x08A06684). */
typedef struct SideFlag {
    unsigned char open;
    unsigned char pad_01[0xF];
    int state;
    void *model;
    float time;
    int owner;
    unsigned char pad_20[4];
    void *source;
} SideFlag;

/* Phase clock (load 0x08A16B78, rate 0x08A16CC4, update 0x08A169DC): a face
 * model and a hand model whose speed follows the round time. */
typedef struct PhaseClock {
    unsigned char open;
    unsigned char pad_01[7];
    void *face;
    void *hand;
    unsigned char pad_10[4];
    int kind;
    void *source;
    unsigned char pad_1c[0x14];
} PhaseClock;

/* Bonus prize (update 0x08A18F00, number 0x08A19080, clear 0x08A191E8): two
 * digit models that follow the owner's bones while its motion plays. */
typedef struct PrizeModel {
    unsigned char pad_00[0xC];
    short frame;
} PrizeModel;

typedef struct BonusPrize {
    unsigned char open;
    unsigned char shown;
    unsigned char pad_02[2];
    PrizeModel *models[2];
    int motion;
    int *bones;
    void *owner;
    void *source;
    int *frames;
    unsigned char pad_20[4];
} BonusPrize;

/* Brave panel (load 0x08A20328, number 0x08A2045C, attach 0x08A2052C): a
 * side's Brave number drawn on four parts that follow the panel's bones. */
typedef struct BravePanel {
    unsigned char open;
    unsigned char pad_01[3];
    unsigned char parts[0x20];
    void *model;
    unsigned int side;
    int cue;
    void *source;
} BravePanel;

#endif
