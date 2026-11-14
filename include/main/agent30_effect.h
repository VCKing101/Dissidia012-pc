#ifndef MAIN_AGENT30_EFFECT_H
#define MAIN_AGENT30_EFFECT_H

/* Effect node of the effect manager (0x08818B28-0x08819480). */
struct EffectNodeDef {
    short id;
    char pad_02[8];
    short flags;                /* 0x0A: 1 cached, 2 restartable */
};

struct EffectNodeTrack {
    char pad_00[0x18];
    int looping;
};

struct EffectNode {
    EffectNode *next;           /* 0x00 */
    char pad_04[8];
    unsigned char active;       /* 0x0C */
    char pad_0D[3];
    int state;                  /* 0x10 */
    void *owner;                /* 0x14 */
    char lights[0xC];           /* 0x18 */
    unsigned char a_on;         /* 0x24 */
    char pad_25[3];
    float a_time;               /* 0x28 */
    int a_value;                /* 0x2C */
    unsigned char b_on;         /* 0x30 */
    char pad_31[3];
    float b_time;               /* 0x34 */
    int b_value;                /* 0x38 */
    float c_time;               /* 0x3C */
    int sound;                  /* 0x40 */
    unsigned char sound_on;     /* 0x44 */
    unsigned char sound_loop;   /* 0x45 */
    char pad_46[2];
    EffectNodeDef *def;         /* 0x48 */
    EffectNodeTrack *track;     /* 0x4C */
};

#endif
