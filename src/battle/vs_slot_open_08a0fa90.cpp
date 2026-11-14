/* Opens a versus slot from a slot definition and opens its board. */
class SlotBase {
public:
    virtual void slot_008();
    virtual void slot_010();
    virtual void reset();
};

struct VsDef {
    unsigned int side;
    unsigned int kind;
    void *owner;
    unsigned char pad_0c[4];
    void *source;
    void *layout;
    int round;
};

class VsSlot : public SlotBase {
public:
    unsigned char open;
    unsigned char pad_05[0xB];
    unsigned char board[0x12C];
    unsigned int side;
    unsigned int kind;
    void *owner;
    void *layout;
};

extern "C" {

extern int func_08A0EE2C(void *board, int turn, unsigned int side, void *source, int round);

int func_08A0FA90(VsSlot *slot, VsDef *def)
{
    slot->reset();
    if (def == 0 || def->side >= 2 || def->kind >= 2 || def->owner == 0 || def->source == 0 || def->layout == 0 ||
        def->round < 0) {
        return -1;
    }
    unsigned int side = def->side;
    unsigned int type = def->kind;
    void *owner = def->owner;
    void *source = def->source;
    void *layout = def->layout;
    int round = def->round;
    int turn = (int)(side + type) % 2;
    func_08A0EE2C(slot->board, turn, side, source, round);
    slot->side = side;
    slot->kind = type;
    slot->owner = owner;
    slot->layout = layout;
    slot->open = 1;
    return 0;
}

}
