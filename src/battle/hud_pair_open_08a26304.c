#include "battle/agent31_types.h"

extern void func_08A262C4(A31HudPair *self);
extern void *func_08A04C44(A31SideTable *table, int key);
extern void func_089F82F8(void *frame, unsigned int side, int kind, void *textures, void *motions);
extern void func_08A24E18(void *body, int kind, void *entry, void *models, void *textures, void *motions);
extern void func_08A08254(void *label, int kind, int mode, void *sheet);

int func_08A26304(A31HudPair *self, unsigned int side, unsigned int slot, A31ResourceSet *set, int key)
{
    void *models;
    void *textures;
    void *motions;
    void *sheet;
    void *entry;
    int kind;

    func_08A262C4(self);
    if (side >= 2 || slot >= 2 || set == 0 || key < 0) {
        return -1;
    }
    models = set->models;
    textures = set->textures;
    motions = set->motions;
    sheet = set->sheet;
    if (models == 0 || textures == 0 || motions == 0 || sheet == 0) {
        return -2;
    }
    entry = func_08A04C44(&set->sides[side], key);
    if (entry == 0) {
        return -3;
    }
    kind = (int)(side + slot) % 2;
    func_089F82F8(self->frame, side, kind, textures, motions);
    func_08A24E18(self->body, kind, entry, models, textures, motions);
    func_08A08254(self->label, kind, 2, sheet);
    self->side = side;
    self->slot = slot;
    self->open = 1;
    return 0;
}
