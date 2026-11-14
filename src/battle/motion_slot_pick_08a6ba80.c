/* Picks the motion slot for this request: the plain one when it exists,
 * otherwise a random variant of the fighter's motion set (-1 when none). */
typedef struct PickSet {
    unsigned char pad_00[2];
    signed char variants;
} PickSet;

typedef struct PickSets {
    PickSet *current;
} PickSets;

typedef struct PickFighter {
    unsigned char pad_000[0xD5C];
    PickSets sets;
} PickFighter;

typedef struct PickInfo {
    unsigned char pad_00[4];
    PickFighter *fighter;
} PickInfo;

typedef struct PickEvent {
    unsigned char pad_00[0x64];
    void *owner;
} PickEvent;

extern void *func_08AB3284(void *owner);
extern PickInfo *func_08AB3264(void *owner);
extern unsigned int func_08A65110(void *bank, int motion, unsigned char kind, int variant);
extern int func_08ACF234(void);

unsigned int func_08A6BA80(PickEvent *self, int motion, unsigned char kind)
{
    void *bank = func_08AB3284(self->owner);
    int none = -1;
    unsigned int slot;
    PickSet *set;
    int variants;
    slot = func_08A65110(bank, motion, kind, -1);
    if (slot >= 12) {
        {
            PickSets *sets = &func_08AB3264(self->owner)->fighter->sets;
            set = sets->current;
        }
        if (set == 0) {
            return none;
        }
        variants = set->variants;
        if (variants <= 0) {
            return none;
        }
        slot = func_08A65110(bank, motion, kind, (func_08ACF234() + 1) % variants);
        if (slot >= 12) {
            return none;
        }
    }
    return slot;
}
