#include "battle/agent31_types.h"

/* Open request for the card part. */
struct CardRequest {
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

class HudCard : public HudPartBase {
public:
    unsigned char open;
    unsigned char pad_05[0xF];
    unsigned char card[0x28];
    unsigned char back[0x30];
    unsigned int side;
    unsigned int slot;
    void *name;
    void *icon;
};

extern "C" void *func_08A04C44(A31SideTable *table, int key);
extern "C" void func_08A0189C(void *card, unsigned int side, unsigned int slot, void *cards, void *entry, int flags);
extern "C" void func_08A16874(void *back, unsigned int side, unsigned int slot, void *cards, int flags);

extern "C" int func_08A29F04(HudCard *self, CardRequest *request)
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
    int key = request->key;
    void *cards = set->cards;
    if (cards == 0) {
        return -2;
    }
    void *entry = func_08A04C44(&set->sides[side], key);
    if (entry == 0) {
        return -3;
    }
    func_08A0189C(self->card, side, slot, cards, entry, 0);
    func_08A16874(self->back, side, slot, cards, 0);
    self->side = side;
    self->slot = slot;
    self->name = name;
    self->icon = icon;
    self->open = 1;
    return 0;
}
