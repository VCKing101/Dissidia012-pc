#ifndef BATTLE_AGENT14_TYPES_H
#define BATTLE_AGENT14_TYPES_H

/* Table of 34 twelve-byte slots (0x08A82xxx). */
typedef struct SlotEntry {
    unsigned char used;
    unsigned char kind;
    char pad_02[0x2];
    int value;
    float time;
} SlotEntry;

typedef struct SlotTable {
    unsigned char active;
    char pad_01[0x3];
    void *field_04;
    void *field_08;
    void *field_0C;
    SlotEntry entries[34];
    int field_1A8;
    unsigned char field_1AC;
} SlotTable;

typedef struct Vec4 {
    float x, y, z, w;
} Vec4;

typedef struct Vec3 {
    float x, y, z;
} Vec3;

/* Marker objects around 0x08A85C90: an id, a position and id pairs. */
typedef struct MarkerPair {
    int first;
    int second;
} MarkerPair;

typedef struct Marker {
    unsigned char active;
    char pad_01[0x3];
    void *owner;
    int field_08;
    unsigned char field_0C;
    char pad_0D[0x3];
    Vec4 position;
    int id;
    Vec3 field_24;
    Vec3 field_30;
    MarkerPair pairs[2];
    int field_4C;
    Vec3 field_50;
} Marker;

/* Set of three 0x30-byte link slots (0x08A86A20-0x08A870D8). */
typedef struct LinkSlot {
    unsigned char active;
    char pad_01[0x3];
    void *owner;
    char pad_08[0x28];
} LinkSlot;

typedef struct LinkSet {
    unsigned char active;
    char pad_01[0x3];
    void *owner;
    void *data;
    char pad_0C[0x14];
    LinkSlot slots[3];
    int field_B0;
} LinkSet;

#ifdef __cplusplus

extern "C" void func_08992F04(void *object); /* operator delete */

/*
 * Small polymorphic objects whose deleting destructor only stores the
 * class's own vtable and frees the object when bit 0 of the flags is set.
 * The vtable pointer sits after 8 or 0x10 bytes of base data. The whole body
 * is an in-class helper: SNC then fills the null check's delay slot with the
 * vtable `lui`, as retail does.
 */
struct TableAt08Object {
    char data_00[0x8];
    const void *vtable;

    static void destroy(TableAt08Object *object, int flags, const void *vtable)
    {
        if (object) {
            object->vtable = vtable;
            if (flags & 1) func_08992F04(object);
        }
    }
};

struct TableAt10Object {
    char data_00[0x10];
    const void *vtable;

    static void destroy(TableAt10Object *object, int flags, const void *vtable)
    {
        if (object) {
            object->vtable = vtable;
            if (flags & 1) func_08992F04(object);
        }
    }
};

/*
 * Handle-holding battle tasks (0x08A88xxx-0x08A92xxx): vptr at 0, a "bound"
 * byte at 4 and the handle at 8. Virtual 1 releases, virtual 2 resets and
 * virtual 5 reports readiness. The vptr sits in an empty base because SNC
 * places it after a class's own data members.
 */
class HandleTaskBase {
public:
    virtual void slot_008();
    virtual void release();
    virtual void reset();
    virtual void slot_020();
    virtual void slot_028();
    virtual int isReady();
};

class HandleTask : public HandleTaskBase {
public:
    unsigned char bound;
    int handle;
};

/* The same objects seen by their destructors, with the vtable as a field. */
struct HandleTaskFrame {
    const void *vtable;

    /* Inlined base-class destructor: only restores the base vtable. */
    static void destructBase(HandleTaskFrame *base, const void *baseTable)
    {
        if (base != 0) {
            base->vtable = baseTable;
        }
    }
};

#endif

#endif
