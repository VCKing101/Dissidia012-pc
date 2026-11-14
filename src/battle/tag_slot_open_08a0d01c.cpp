/* Opens a tag slot from a slot definition: looks up the side's round entry
 * and opens the slot's cue and progress displays with it. */
class SlotBase {
public:
    virtual void slot_008();
    virtual void slot_010();
    virtual void reset();
};

struct TagEntry {
    unsigned char data[0xA8];
};

struct TagTable {
    unsigned char pad_000[0x14];
    TagEntry entries[2];
    unsigned char pad_164[4];
    void *source;
    unsigned char pad_16c[4];
    void *layout;
};

struct TagDef {
    unsigned int side;
    unsigned int kind;
    void *owner;
    unsigned char pad_0c[4];
    TagTable *tables;
    void *frame;
    int round;
};

class TagSlot : public SlotBase {
public:
    unsigned char open;
    unsigned char pad_05[0xF];
    unsigned char cue[0x28];
    unsigned char progress[0x30];
    unsigned int side;
    unsigned int kind;
    void *owner;
    void *frame;
};

extern "C" {

extern void *func_08A04C44(void *entries, int round);
extern int func_08A0189C(void *cue, unsigned int side, unsigned int kind, void *source, void *layout,
                         unsigned char shared);
extern void func_08A16874(void *progress, unsigned int side, unsigned int kind, void *source, int flags);

int func_08A0D01C(TagSlot *slot, TagDef *def)
{
    slot->reset();
    if (def == 0 || def->side >= 2 || def->kind >= 2 || def->owner == 0 || def->tables == 0 || def->frame == 0 ||
        def->round < 0) {
        return -1;
    }
    unsigned int side = def->side;
    unsigned int kind = def->kind;
    void *owner = def->owner;
    void *frame = def->frame;
    TagTable *tables = def->tables;
    int round = def->round;
    void *source = tables->source;
    void *layout = tables->layout;
    if (source == 0 || layout == 0) {
        return -2;
    }
    void *entry = func_08A04C44(&tables->entries[side], round);
    if (entry == 0) {
        return -3;
    }
    func_08A0189C(slot->cue, side, kind, source, entry, 0);
    func_08A16874(slot->progress, side, kind, source, 0);
    slot->side = side;
    slot->kind = kind;
    slot->owner = owner;
    slot->frame = frame;
    slot->open = 1;
    return 0;
}

}
