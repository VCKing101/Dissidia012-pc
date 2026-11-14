#ifndef MAIN_AGENT30_TYPES_H
#define MAIN_AGENT30_TYPES_H

/* Types shared by agent 30's sources (main 0x08804BA0-0x0884516C). */

typedef struct Vec4 { float x, y, z, w; } Vec4;

/* One effect slot of an actor (8 bytes). Actors hold 16 timed slots at
   +0x584 and 16 fixed slots at +0x604. */
struct ActorEffectSlot {
    unsigned char active;
    unsigned char held;  /* never counts down */
    unsigned char kind;
    unsigned char level;
    float time;          /* frames left */

    static void set(ActorEffectSlot *slot, int kind, int level, int frames)
    {
        slot->kind = kind;
        slot->level = level;
        slot->time = frames;
        if (frames <= 0) {
            slot->held = 1;
        }
        slot->active = 1;
    }
};

struct ActorEffectSlots {
    ActorEffectSlot timed[16];
    ActorEffectSlot fixed[16];
};

/* Camera blend state (the camera functions at 0x08806650-0x08806A34). */
struct CameraBlend {
    char pad_000[0x90];
    Vec4 pos;                   /* 0x090 */
    char pad_0A0[0xA90 - 0xA0];
    Vec4 from;                  /* 0xA90 */
    Vec4 to;                    /* 0xAA0 */
    float duration;             /* 0xAB0 */
    float elapsed;              /* 0xAB4 */
    float offset_x;             /* 0xAB8 */
    float offset_y;             /* 0xABC */
    float offset_z;             /* 0xAC0 */
    float a_time;               /* 0xAC4 */
    float a_value;
    float b_time;               /* 0xACC */
    float b_value;
    float c_time;               /* 0xAD4 */
    float c_value;
    float d_time;               /* 0xADC */
    float d_value;
    int mode;                   /* 0xAE4 */
    float distance;             /* 0xAE8 */
    float base_distance;        /* 0xAEC */
    unsigned char started;      /* 0xAF0 */
};

/* Queue of short motions played one after another (0x08806B94-0x08806CD4).
   Each queued byte is (slot << 5) | duration / 2. */
struct MotionQueue {
    void *motions[7];           /* 0x00 */
    unsigned char codes[64];    /* 0x1C, ring buffer */
    float timer;                /* 0x5C */
    int pushed;                 /* 0x60 */
    unsigned char slot;         /* 0x64 */
    unsigned char head;         /* 0x65 */
    unsigned char count;        /* 0x66 */
};

/* 256-bit set (8 words). */
struct BitSet256 {
    unsigned int words[8];

    static bool test(BitSet256 *set, unsigned int bit)
    {
        unsigned int *words = set->words;
        return (words[bit >> 5] & (1 << (bit & 0x1F))) != 0;
    }

    static void add(BitSet256 *set, unsigned int bit)
    {
        unsigned int *w = &set->words[bit >> 5];
        *w |= 1 << (bit & 0x1F);
    }

    static void remove(BitSet256 *set, unsigned int bit)
    {
        unsigned int *w = &set->words[bit >> 5];
        *w &= ~(1 << (bit & 0x1F));
    }
};

/* Attack hit area (0x0880A9E8-0x0880AE58): which actors it has hit, with a
   few timed hit flags; areas are kept on the list at D_089D4770. */
struct HitAreaTimer {
    unsigned int bit;
    float time;
    void *actor;
};

struct HitArea {
    void *owner;                /* 0x00 */
    BitSet256 hit;              /* 0x04 */
    BitSet256 seen;             /* 0x24 */
    float cooldown;             /* 0x44 */
    HitAreaTimer timers[4];     /* 0x48 */
    HitArea *next;              /* 0x78 */
};

struct HitAreaList {
    HitArea *head;
    HitArea *tail;

    static HitArea *&next_of(HitArea *a) { return a->next; }

    void append(HitArea *area)
    {
        if (this->tail == 0) {
            this->tail = area;
            this->head = area;
            next_of(area) = 0;
        } else {
            next_of(this->tail) = area;
            next_of(area) = 0;
            this->tail = area;
        }
    }

    void remove(HitArea *area)
    {
        HitArea *head = this->head;
        HitArea *tail = this->tail;
        if (head != 0 && tail != 0) {
            HitArea *next = area->next;
            HitArea *prev;
            if (area == this->head) {
                this->head = next;
                prev = 0;
            } else {
                prev = this->head;
                while (prev->next != area) {
                    prev = prev->next;
                }
                prev->next = next;
            }
            if (area == this->tail) {
                this->tail = prev;
            }
            area->next = 0;
        }
    }
};

/* Effect object of 0x0880FBF8-0x08810180: a resource with a sorted table
   of motion entries, an animation player and a motion state. */
struct EffectMotionEntry {
    short key;
    unsigned short motion;
    int data;
};

struct EffectMotionTable {
    int pad_0;
    int count;
    EffectMotionEntry entries[1];
};

extern "C" int func_0880FB60(const void *a, const void *b);
extern "C" EffectMotionEntry *func_08934944(int key, EffectMotionEntry *base, int count, int size, int (*cmp)(const void *, const void *));

struct EffectResource {
    char pad_00[0x1C];
    EffectMotionTable *table;

    static EffectMotionEntry *find(EffectResource *r, int key)
    {
        EffectMotionTable *table = r->table;
        EffectMotionTable *t = table != 0 ? table : 0;
        return func_08934944(key, t->entries, t->count, sizeof(EffectMotionEntry), func_0880FB60);
    }
};

struct EffectObject {
    char pad_00[0x94];
    EffectResource *resource;   /* 0x94 */
    void *player;               /* 0x98 */
    char anim[0x14];            /* 0x9C */
    int arg;                    /* 0xB0 */
    int state;                  /* 0xB4 */
    float length;               /* 0xB8 */
    char pad_BC[4];
    int id;                     /* 0xC0 */
};

#endif
