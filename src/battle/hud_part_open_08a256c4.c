#include "battle/agent31_types.h"

typedef struct HudPart256C4 {
    unsigned char open;
    unsigned char pad_01[0x13];
    unsigned char frame[0x38];
    unsigned char body[0x74];
    unsigned int side;
} HudPart256C4;

extern void func_08A2568C(HudPart256C4 *self);
extern void *func_08A04C44(A31SideTable *table, int key);
extern void func_089F82F8(void *frame, unsigned int side, int kind, void *textures, void *motions);
extern void func_08A25C8C(void *body, void *entry, void *models, void *textures, void *motions);

int func_08A256C4(HudPart256C4 *self, unsigned int side, A31ResourceSet *set, int key)
{
    void *models;
    void *textures;
    void *motions;
    void *entry;

    func_08A2568C(self);
    if (side >= 2 || set == 0 || key < 0) {
        return -1;
    }
    models = set->models;
    textures = set->textures;
    motions = set->motions;
    if (models == 0 || textures == 0 || motions == 0) {
        return -2;
    }
    entry = func_08A04C44(&set->sides[side], key);
    if (entry == 0) {
        return -3;
    }
    func_089F82F8(self->frame, side, 2, textures, motions);
    func_08A25C8C(self->body, entry, models, textures, motions);
    self->side = side;
    self->open = 1;
    return 0;
}
