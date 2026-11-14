#include "battle/agent31_types.h"

/* Open request for the name part. */
struct NameRequest {
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

class HudName : public HudPartBase {
public:
    unsigned char open;
    unsigned char pad_05[0xF];
    unsigned char label[0x34];
    unsigned int side;
    unsigned int slot;
    void *name;
    void *icon;
};

extern "C" void func_08A08254(void *label, int kind, int mode, void *sheet);

extern "C" int func_08A273E0(HudName *self, NameRequest *request)
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
    int kind = (int)(side + slot) % 2;
    func_08A08254(self->label, kind, 2, sheet);
    self->side = side;
    self->slot = slot;
    self->name = name;
    self->icon = icon;
    self->open = 1;
    return 0;
}
