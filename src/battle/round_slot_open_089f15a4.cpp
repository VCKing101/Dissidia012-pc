/* Opens a round slot from a round definition: looks up the side's entry and
 * opens the slot's marker with it. */
class SlotBase {
public:
    virtual void slot_008();
    virtual void slot_010();
    virtual void reset();
};

struct RoundTable {
    unsigned char pad_00[0x14];
    unsigned char entries[0x94];
};

struct RoundDef {
    unsigned int side;
    void *owner;
    unsigned char pad_08[4];
    RoundTable *tables;
    void *source;
    int round;
};

class RoundSlot : public SlotBase {
public:
    unsigned char open;
    unsigned char pad_05[0xF];
    unsigned char marker[0x14];
    unsigned int side;
    void *owner;
    void *source;
};

extern "C" {

extern void *func_08A04C44(void *entries, int round);
extern void func_089F1CC0(void *marker, unsigned int side, void *entry);

int func_089F15A4(RoundSlot *slot, RoundDef *def)
{
    slot->reset();
    if (def == 0 || def->side >= 2 || def->owner == 0 || def->tables == 0 || def->source == 0 || def->round < 0) {
        return -1;
    }
    unsigned int side = def->side;
    void *owner = def->owner;
    void *source = def->source;
    int round = def->round;
    void *entry = func_08A04C44(def->tables[side].entries, round);
    if (entry == 0) {
        return -2;
    }
    func_089F1CC0(slot->marker, side, entry);
    slot->side = side;
    slot->owner = owner;
    slot->source = source;
    slot->open = 1;
    return 0;
}

}
