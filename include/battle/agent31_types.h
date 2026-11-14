#ifndef BATTLE_AGENT31_TYPES_H
#define BATTLE_AGENT31_TYPES_H

/* Types identified by agent31 (battle 0x08A22DEC-0x08A7EE04). */

/* Battle-wide timing (D_089D69E0). */
typedef struct A31FrameClock {
    unsigned char pad_00[0x18];
    float frame_scale;
} A31FrameClock;

/* Battle state (D_089D69F8): the two fighters at +0x18. */
typedef struct A31Fighter {
    unsigned char pad_000[0x2A8];
    unsigned char model[4];
} A31Fighter;

typedef struct A31Battle {
    unsigned char pad_00[0x18];
    A31Fighter *fighters[2];
} A31Battle;

/* Resource package (0x28 bytes each) and the records read from it with
 * 0x088916A4 (header) and 0x08891894 (entry). */
typedef struct A31Package {
    unsigned char data[0x28];
} A31Package;

typedef struct A31PackageInfo {
    unsigned char pad_00[0xC];
    unsigned short first;
    unsigned short count;
    unsigned char pad_10[0x48];
} A31PackageInfo;

typedef struct A31PackageEntry {
    unsigned char pad_00[0x2C];
    short kind;
    unsigned char pad_2e[0x12];
} A31PackageEntry;

/* Ready screen (start 0x08A23358). */
typedef struct ReadyScreen {
    unsigned char active;
    unsigned char pad_01[3];
    float fade;
    unsigned char kinds[2];
    unsigned char prev_kinds[2];
    unsigned char pad_0c[0xE4];
    A31Package *packages;
    void *owner;
} ReadyScreen;

/* Four-digit number panel (set 0x08A253CC). */
typedef struct A31DigitPanel {
    unsigned char open;
    unsigned char pad_01[0x27];
    void *digits[4];
    unsigned char pad_38[0x44];
    void *sheet;
} A31DigitPanel;

/* Fighter resource set passed to the HUD part openers (0x08A256C4,
 * 0x08A26304): per-side motion tables at +0x14 and three banks. */
typedef struct A31SideTable {
    unsigned char data[0xA8];
} A31SideTable;

typedef struct A31ResourceSet {
    unsigned char pad_000[0xC];
    void *sheet;
    unsigned char pad_010[4];
    A31SideTable sides[2];
    unsigned char pad_164[4];
    void *cards;
    unsigned char pad_16c[4];
    void *models;
    void *textures;
    void *motions;
} A31ResourceSet;

/* Paired HUD part (open 0x08A26304, start 0x08A264D8). */
typedef struct A31HudPair {
    unsigned char open;
    unsigned char pad_01[3];
    int state;
    float time;
    int value;
    unsigned char look;
    unsigned char pad_11[3];
    int arg_14;
    int arg_18;
    unsigned char frame[0x38];
    unsigned char body[0x88];
    unsigned char label[0x34];
    unsigned int side;
    unsigned int slot;
} A31HudPair;

/* Fighter as the state callbacks see it (a1 of the handlers at
 * 0x08A2AE70-0x08A2C828). */
typedef struct A31Vec4 {
    float x, y, z, w;
} A31Vec4;

typedef struct A31Move {
    float x, y, z;
    unsigned char pad_0c[0x18];
    unsigned char landed;
} A31Move;

typedef struct A31Timer {
    int id;
    int active;
} A31Timer;

typedef struct A31Actor {
    unsigned char pad_000[0x130];
    unsigned char motion[0x250];
    unsigned int state_flags;
    unsigned int flags;
    unsigned long long mask;
    unsigned char pad_390[0x18];
    A31Move *move;
    unsigned char pad_3ac[0x224];
    unsigned int flags_5d0;
    unsigned char pad_5d4[0x76C];
    A31Timer timer;
} A31Actor;

#ifdef __cplusplus
/* The fighter class seen from the effect code: data up to 0x544, then the
 * vptr (virtual 37: the held partner, 39: the guard state), then more data. */
struct A31ModelPart {
    unsigned char pad_00[0x28];
    void *bone;
};

struct A31Model {
    unsigned char pad_00[0x34];
    A31ModelPart part;
};

struct A31ActorBits {
    unsigned int word;
    unsigned char b4;
    unsigned char b5;
    unsigned char pad_6[2];
    unsigned char b8;
};

class A31VActorBase {
public:
    unsigned char pad_000[0x130];
    unsigned char motion[0x50];
    short motion_id;
    unsigned char pad_182[0x26];
    float time;
    unsigned char pad_1ac[0x1D4];
    unsigned int state_flags;
    unsigned int flags;
    unsigned long long mask;
    A31Model *model;
    void *voice;
    unsigned char pad_398[0x10];
    void *move;
    unsigned char pad_3ac[0x11C];
    unsigned char hit_point[4];
    unsigned char pad_4cc[0x78];

    virtual void slot_000();
    virtual void slot_001();
    virtual void slot_002();
    virtual void slot_003();
    virtual void slot_004();
    virtual void slot_005();
    virtual void slot_006();
    virtual void slot_007();
    virtual void slot_008();
    virtual void slot_009();
    virtual void slot_010();
    virtual void slot_011();
    virtual void slot_012();
    virtual void slot_013();
    virtual void slot_014();
    virtual void slot_015();
    virtual void slot_016();
    virtual void slot_017();
    virtual void slot_018();
    virtual void slot_019();
    virtual void slot_020();
    virtual void slot_021();
    virtual void slot_022();
    virtual void slot_023();
    virtual void slot_024();
    virtual void slot_025();
    virtual void slot_026();
    virtual void slot_027();
    virtual void slot_028();
    virtual void slot_029();
    virtual void slot_030();
    virtual void slot_031();
    virtual void slot_032();
    virtual void slot_033();
    virtual void slot_034();
    virtual void slot_035();
    virtual void slot_036();
    virtual class A31VActor *partner();
    virtual void slot_038();
    virtual int guard();
};

class A31VActor : public A31VActorBase {
public:
    unsigned char pad_548[0x64];
    void *guard_display;
    A31ActorBits bits;
    unsigned char pad_5bc[0x14];
    unsigned int flags_5d0;
    unsigned char pad_5d4[0x78C];
    signed char form;
    unsigned char pad_d61[3];
    unsigned char effects[4];
};

/* A spawned bone effect (0x088A9DE8). */
struct A31BoneEffect {
    unsigned char pad_000[0xEC];
    float scale;
};

extern "C" A31BoneEffect *func_088A9DE8(void *bone, int id, void *point, int a, int b);

struct A31Spawn {
    static A31BoneEffect *at_hit(A31VActor *actor, int id)
    {
        A31ModelPart *part = &actor->model->part;
        return func_088A9DE8(part->bone, id, actor->hit_point, 0, 0);
    }
};
#endif

#endif
