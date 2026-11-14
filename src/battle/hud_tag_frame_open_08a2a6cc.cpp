#include "battle/agent31_types.h"

/* Open request for the framed tag part. */
struct FrameTagRequest {
    unsigned int side;
    void *name;
    int pad_08;
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

class HudFrameTag : public HudPartBase {
public:
    unsigned char open;
    unsigned char pad_05[0x17];
    unsigned char face[0x2C];
    unsigned char frame[0x38];
    unsigned char plate[0x48];
    unsigned int side;
    void *name;
    void *icon;
};

extern "C" void *func_08A04C44(A31SideTable *table, int key);
extern "C" void func_08A10BE0(void *face, void *entry, int a, int b, int c, int d, int e);
extern "C" void func_089F82F8(void *frame, unsigned int side, int kind, void *textures, void *motions);
extern "C" void func_089F5F74(void *plate, unsigned int side, int kind, void *textures, void *motions);

extern "C" int func_08A2A6CC(HudFrameTag *self, FrameTagRequest *request)
{
    self->close();
    if (request == 0 || request->side >= 2 || request->name == 0 || request->set == 0 ||
        request->icon == 0 || request->key < 0) {
        return -1;
    }
    unsigned int side = request->side;
    void *name = request->name;
    A31ResourceSet *set = request->set;
    void *icon = request->icon;
    int key = request->key;
    void *textures = set->textures;
    void *motions = set->motions;
    if (textures == 0 || motions == 0) {
        return -2;
    }
    void *entry = func_08A04C44(&set->sides[side], key);
    if (entry == 0) {
        return -3;
    }
    func_08A10BE0(self->face, entry, 0, 0, -1, -1, 0);
    func_089F82F8(self->frame, side, 2, textures, motions);
    func_089F5F74(self->plate, side, 2, textures, motions);
    self->side = side;
    self->name = name;
    self->icon = icon;
    self->open = 1;
    return 0;
}
