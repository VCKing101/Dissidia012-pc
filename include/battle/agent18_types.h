#ifndef BATTLE_AGENT18_TYPES_H
#define BATTLE_AGENT18_TYPES_H

#ifdef __cplusplus

#include "battle/agent11_types.h"

/*
 * A polymorphic battle object reached only through its vtable. Virtual N
 * sits at vtable offset 8 * (N + 1); the names give that offset.
 */
class BattleObject {
public:
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
};

/* A battle task (isActive at vtable 0x40) holding two child objects. */
class ChildPairTask : public BattleTaskBase {
public:
    int field_04;
    BattleObject *children[2];
    int field_10;
};

/* The same objects seen through virtuals that take one argument. */
class BattleArgObject {
public:
    virtual int slot_008(int arg);
    virtual int slot_010(int arg);
    virtual int slot_018(int arg);
    virtual int slot_020(int arg);
    virtual int slot_028(int arg);
    virtual int slot_030(int arg);
    virtual int slot_038(int arg);
    virtual int slot_040(int arg);
    virtual int slot_048(int arg);
    virtual int slot_050(int arg);
    virtual int slot_058(int arg);
    virtual int slot_060(int arg);
    virtual int slot_068(int arg);
    virtual int slot_070(int arg);
    virtual int slot_078(int arg);
    virtual int slot_080(int arg);
    virtual int slot_088(int arg);
    virtual int slot_090(int arg);
    virtual int slot_098(int arg);
    virtual int slot_0A0(int arg);
    virtual int slot_0A8(int arg);
    virtual int slot_0B0(int arg);
    virtual int slot_0B8(int arg);
    virtual int slot_0C0(int arg);
    virtual int slot_0C8(int arg);
    virtual int slot_0D0(int arg);
    virtual int slot_0D8(int arg);
    virtual int slot_0E0(int arg);
    virtual int slot_0E8(int arg);
    virtual int slot_0F0(int arg);
    virtual int slot_0F8(int arg);
    virtual int slot_100(int arg);
    virtual int slot_108(int arg);
    virtual int slot_110(int arg);
    virtual int slot_118(int arg);
    virtual int slot_120(int arg);
    virtual int slot_128(int arg);
    virtual int slot_130(int arg);
    virtual int slot_138(int arg);
    virtual int slot_140(int arg);
};

/* A battle task holding two child objects called with an argument. */
class ChildPairArgTask : public BattleTaskBase {
public:
    int field_04;
    BattleArgObject *children[2];
    int field_10;
};

/*
 * Fighter object seen through its vptr after 0x544 bytes of data, with
 * int-returning virtuals (slot names give the vtable offset).
 */
class FighterObject {
public:
    char data_000[0x544];
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
    virtual int slot_188();
    virtual int slot_190();
    virtual int slot_198();
    virtual int slot_1A0();
};

#endif

#endif
