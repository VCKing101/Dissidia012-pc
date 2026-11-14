#include "battle/agent31_types.h"

/* Open request for the side tag part. */
struct TagRequest {
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

class HudTag : public HudPartBase {
public:
    unsigned char open;
    unsigned char pad_05[0xF];
    unsigned char label[0x34];
    unsigned int side;
    void *name;
    void *icon;
};

extern "C" void func_08A08254(void *label, int kind, int mode, void *sheet);

extern "C" int func_08A28724(HudTag *self, TagRequest *request)
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
    void *sheet = set->sheet;
    if (sheet == 0) {
        return -2;
    }
    int kind = (int)(side + 1) % 2;
    func_08A08254(self->label, kind, 2, sheet);
    self->side = side;
    self->name = name;
    self->icon = icon;
    self->open = 1;
    return 0;
}
