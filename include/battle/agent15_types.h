#ifndef BATTLE_AGENT15_TYPES_H
#define BATTLE_AGENT15_TYPES_H

/* Shared types for agent15's battle functions (0x08AAFEB8 onward). */

/* One 0x188-byte unit parameter record, found by id in the parameter table. */
typedef struct UnitParamEntry {
    int id;
    char pad_04[0x4];
    float field_08;
    char pad_0C[0x8];
    float field_14;
    float field_18;
    float field_1C;
    char pad_20[0xC];
    float field_2C;
    int field_30;
    float field_34;
    float field_38;
    float field_3C;
    float field_40;
    float field_44;
    char pad_48[0x8];
    float field_50;
    float field_54;
    float field_58;
    char pad_5C[0x8];
    float field_64;
    char pad_68[0xAC];
    float field_114;
    int field_118;
    char pad_11C[0x14];
    int field_130;
    char pad_134[0x14];
    float field_148;
    char pad_14C[0x4];
    float field_150;
    float field_154;
    char pad_158[0x8];
    float field_160;
    char pad_164[0x24];
} UnitParamEntry;

/* The table returned by func_08A80200(D_08A80218). */
typedef struct UnitParamTable UnitParamTable;

/* Any object whose first byte says whether it is active and that can name its unit id. */
typedef struct UnitParamOwner {
    unsigned char active;
} UnitParamOwner;

extern int D_08A80218;
extern UnitParamTable *func_08A80200(int key);
extern int func_08AAE41C(UnitParamOwner *self);
extern UnitParamEntry *func_08AACA24(UnitParamTable *table, int id);

typedef struct FloatRange {
    float min;
    float max;
} FloatRange;

/* Battle-wide tuning values, reached through func_08AACB10(table). */
typedef struct BattleCommonParams {
    char pad_00[0x18];
    FloatRange range_18;
    FloatRange range_20;
    char pad_28[0x14];
    FloatRange range_3C;
    FloatRange range_44;
    FloatRange range_4C;
    float field_54;
    float field_58;
    char pad_5C[0x8];
    FloatRange range_64;
    FloatRange range_6C;
} BattleCommonParams;

extern int D_08AA5828;
extern BattleCommonParams *func_08AACB10(UnitParamTable *table);
extern float func_08AA57D0(int seed);

/* Id/value pair from the table's list at 0x194, found by func_08AACB30. */
typedef struct IdValue {
    int id;
    float value;
} IdValue;

extern IdValue *func_08AACB30(UnitParamTable *table, int id);

/* 31-bit flag set with a pending word and the previous frame's flags (0x08AB3848-0x08AB3A2C). */
typedef struct FlagFrames {
    unsigned char active;
    unsigned int current;
    unsigned int pending;
    unsigned int previous;
} FlagFrames;

/*
 * Object whose four ready bytes (0x00, 0x60, 0xB4, 0xD8) must all be 1
 * before its methods act (func_08ABB474, 0x08ABB2BC-0x08ABB6xx).
 */
typedef struct ReadyOwner {
    unsigned char ready_00;
    char pad_001[0x5F];
    unsigned char ready_60;
    char pad_061[0x53];
    unsigned char ready_B4;
    char pad_0B5[0x23];
    unsigned char ready_D8;
    unsigned char flag_D9;
    unsigned char flag_DA;
    unsigned char flag_DB;
    char member_DC[0x80];
    char member_15C[0x10];
    char member_16C[0x38];
    char member_1A4[0x14];
    char list_1B8[0x10];
    char tasks_1C8[0x18];
    float values[4];
} ReadyOwner;

extern unsigned char func_08ABB474(ReadyOwner *self);

#ifdef __cplusplus
extern "C" void func_08AFCB44(void *object);

/* Object with its vtable at 4, freed through func_08AFCB44 (0x08AECDD4). */
struct TableAt04NodeObject {
    char data_00[0x4];
    const void *vtable;

    static void destroy(TableAt04NodeObject *object, int flags, const void *vtable)
    {
        if (object) {
            object->vtable = vtable;
            if (flags & 1) func_08AFCB44(object);
        }
    }
};
/*
 * Fighter-side action holder (0x08AB69CC-0x08AB7044): 0x544 bytes of data,
 * the vptr (SNC puts it after the class's own members), then the derived
 * class's state. Virtual N sits at vtable offset 8 * (N + 1).
 */
struct ActionLinkPart {
    char pad_00[0x28];
    void *target;

    static void *targetOf(ActionLinkPart *part) { return part->target; }
};

struct ActionLink {
    char pad_00[0x34];
    ActionLinkPart part;
};

class ActionHolderBase {
public:
    char pad_000[0x390];
    ActionLink *link_390;
    char pad_394[0x134];
    char target_4C8[0x7C];
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
};

class ActionHolder : public ActionHolderBase {
public:
    char pad_548[0x8];
    unsigned char active;
    int state;
    int value;
    char pad_55C[0x8];
    unsigned char flag_564;
    unsigned char flag_565;
};
/*
 * Scene node with 0xF8 bytes of data before its vptr (0x08AD046C,
 * 0x08AD1228); nodes are chained through next at 0xF4.
 */
struct NodeFlags {
    char pad_00[0x38];
    unsigned int flags;
};

struct NodeInfo {
    char pad_00[0x24];
    NodeFlags *flags;
};

class SceneNode {
public:
    char pad_000[0x64];
    NodeInfo *info;
    char pad_068[0x8C];
    SceneNode *next;
    virtual void slot_008();
    virtual void slot_010();
    virtual void slot_018(int arg);
    virtual void slot_020();
    virtual void slot_028();
    virtual void slot_030();
    virtual int slot_038();
    virtual void slot_040();
};

struct SceneNodeList {
    char pad_00[0x3C];
    SceneNode *head;

    static SceneNode *nextOf(SceneNodeList *list, SceneNode *node) { return node == 0 ? list->head : node->next; }
};
#endif

#endif
