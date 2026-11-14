#ifndef BATTLE_AGENT11_TYPES_H
#define BATTLE_AGENT11_TYPES_H

/* Result of the motion lookup (func_08A22E2C), kept on the caller's stack. */
typedef struct MotionInfo {
    char pad_00[0x8];
    float duration;
    char pad_0C[0x18];
    unsigned char kind;
    char pad_25[0x7];
    char name[0x30];
    short index;
    char pad_5E[0xE];
} MotionInfo;

#ifdef __cplusplus

/*
 * Battle task objects driven by a small state machine. The vptr sits at
 * offset 0 (SNC places it after a class's own data, so the virtuals live in
 * an empty base). The virtual at vtable offset 0x40 reports whether the task
 * is active; the state number is at 0x08 and per-task flag bytes follow it.
 */
class BattleTaskBase {
public:
    virtual void slot_008();
    virtual void slot_010();
    virtual void slot_018();
    virtual void slot_020();
    virtual void slot_028();
    virtual void slot_030();
    virtual void slot_038();
    virtual int isActive();
};

class BattleTask : public BattleTaskBase {
public:
    int field_04;
    int state;
    unsigned char flags[8];
};

/*
 * Fighter object with its vptr after 0x544 bytes of data (SNC places the
 * vptr after the class's own members). Virtual N sits at vtable offset
 * 8 * (N + 1); only the slots used here have real signatures.
 */
class BattleFighter {
public:
    char data_000[0x544];
    virtual void slot_008();
    virtual void slot_010();
    virtual void slot_018();
    virtual void slot_020();
    virtual void slot_028();
    virtual void slot_030();
    virtual void slot_038();
    virtual void slot_040();
    virtual void slot_048();
    virtual void slot_050();
    virtual void slot_058();
    virtual void slot_060();
    virtual void slot_068();
    virtual void slot_070();
    virtual void slot_078();
    virtual void slot_080();
    virtual void slot_088();
    virtual void slot_090();
    virtual void slot_098();
    virtual void slot_0A0();
    virtual void slot_0A8();
    virtual void slot_0B0();
    virtual void slot_0B8();
    virtual void slot_0C0();
    virtual void slot_0C8();
    virtual void slot_0D0();
    virtual void slot_0D8();
    virtual void slot_0E0();
    virtual void slot_0E8();
    virtual void slot_0F0();
    virtual void slot_0F8();
    virtual void slot_100();
    virtual void slot_108();
    virtual void slot_110();
    virtual void slot_118();
    virtual void slot_120();
    virtual void slot_128();
    virtual void slot_130();
    virtual void slot_138();
    virtual void slot_140();
    virtual void slot_148();
    virtual void slot_150();
    virtual void slot_158();
    virtual void slot_160();
    virtual void slot_168();
    virtual void slot_170();
    virtual void slot_178();
    virtual void slot_180();
    virtual void slot_188();
    virtual void slot_190();
    virtual int slot_198();
};

#endif

#endif
