#ifndef BATTLE_AGENT23_TYPES_H
#define BATTLE_AGENT23_TYPES_H

/* Shared types for agent23's battle functions. */

#ifdef __cplusplus
extern "C" {
#endif
#include "battle/agent15_types.h"
#ifdef __cplusplus
}
#endif

/* A unit parameter record (UnitParamEntry) seen as the chance rates rolled against. */
typedef struct UnitRateEntry {
    char pad_00[0xC];
    float rate_0C;
    float rate_10;
    char pad_14[0xC];
    float rate_20;
    char pad_24[0x4];
    float rate_28;
    char pad_2C[0x1C];
    float rate_48;
    float rate_4C;
    char pad_50[0xC];
    float rate_5C;
    float rate_60;
    char pad_64[0x44];
    float rate_A8;
    char pad_AC[0x70];
    float rate_11C;
    float rate_120;
    char pad_124[0x4];
    float rate_128;
    float rate_12C;
    char pad_130[0x4];
    float rate_134;
    float rate_138;
    char pad_13C[0x8];
    float rate_144;
    char pad_148[0x4];
    float rate_14C;
    char pad_150[0x8];
    float rate_158;
    char pad_15C[0xC];
    float rate_168;
} UnitRateEntry;

/* The leading 0x2C bytes of the motion lookup result (func_08A22E2C), as the callers that only
   need the duration and kind keep it on the stack. */
typedef struct MotionBrief {
    char pad_00[0x8];
    float duration;
    char pad_0C[0x18];
    unsigned char kind;
    char pad_25[0x7];
} MotionBrief;

/* An object that names another battle object by handle at 0x30 (resolved with func_08B11718). */
typedef struct HandleOwner {
    char pad_00[0x30];
    int handle;
} HandleOwner;

#ifdef __cplusplus
#include "battle/agent11_types.h"

/* A child of a pair task; virtual N sits at vtable offset 8 * (N + 1). */
class TaskChild {
public:
    virtual void slot_008();
    virtual void slot_010();
    virtual void slot_018();
    virtual void slot_020();
    virtual void slot_028();
    virtual void update(float x, float y);
    virtual void slot_038();
    virtual void slot_040();
    virtual void reset();
    virtual void stop();
    virtual void slot_058();
    virtual void slot_060();
    virtual void slot_068();
    virtual int isReady();
};

/* A battle task that drives one child per side. */
class PairTask : public BattleTaskBase {
public:
    int field_04;
    TaskChild *children[2];
    char pad_10[0x4];
    int side;
};
#endif

#ifdef __cplusplus
extern "C" float func_08AA578C(int id);

struct UnitRoll {
    /* 1 when a random value in [0, 1) falls below the rate. */
    static unsigned char below(const float &rate)
    {
        int result = 0;
        if (func_08AA578C(D_08AA5828) < rate) {
            result = 1;
        }
        return result;
    }
};
#endif

#endif
