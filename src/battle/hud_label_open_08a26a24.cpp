#include "battle/agent31_types.h"

/* Open request for the name label part. */
struct LabelRequest {
    unsigned int side;
    unsigned int slot;
    void *name;
    int pad_0c;
    A31ResourceSet *set;
    void *icon;
    int key;
};

class HudPartBase {
public:
    virtual void slot_008();
    virtual void slot_010();
    virtual void close();
};

class HudLabel : public HudPartBase {
public:
    unsigned char open;
    unsigned char pad_05[0x17];
    unsigned char label[0x34];
    unsigned char frame[0x38];
    unsigned int side;
    unsigned int slot;
    void *name;
    void *icon;
};

extern "C" void func_08A08254(void *label, int kind, int mode, void *sheet);
extern "C" void func_089F82F8(void *frame, unsigned int side, int kind, void *textures, void *motions);

extern "C" int func_08A26A24(HudLabel *self, LabelRequest *request)
{
    self->close();
    if (request == 0 || request->side >= 2 || request->slot >= 2 || request->name == 0 ||
        request->set == 0 || request->icon == 0 || request->key < 0) {
        return -1;
    }
    unsigned int side = request->side;
    unsigned int slot = request->slot;
    void *name = request->name;
    A31ResourceSet *set = request->set;
    void *icon = request->icon;
    void *sheet = set->sheet;
    if (sheet == 0) {
        return -2;
    }
    void *textures = set->textures;
    void *motions = set->motions;
    if (textures == 0 || motions == 0) {
        return -3;
    }
    int kind = (int)(side + slot) % 2;
    func_08A08254(self->label, kind, 2, sheet);
    func_089F82F8(self->frame, side, kind, textures, motions);
    self->side = side;
    self->slot = slot;
    self->name = name;
    self->icon = icon;
    self->open = 1;
    return 0;
}
