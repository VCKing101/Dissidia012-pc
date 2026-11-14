/* Opens a pair display: creates and opens its two children from the
 * display definition. */
struct ChildDef {
    unsigned int side;
    int index;
    void *source;
    void *layout;
    void *frame;
    void *digits;
    int round;
};

class DisplayChild {
public:
    virtual void slot_008();
    virtual void slot_010();
    virtual void slot_018();
    virtual int open(ChildDef *def);
};

struct DisplayDef {
    unsigned int side;
    void *source;
    void *layout;
    void *frame;
    void *digits;
    int round;
};

class DisplayBase {
public:
    virtual void slot_008();
    virtual void slot_010();
    virtual void reset();
};

class PairDisplay : public DisplayBase {
public:
    unsigned char open;
    unsigned char pad_05[3];
    DisplayChild *children[2];
    unsigned int side;
    void *source;
};

extern "C" {

extern DisplayChild *func_08A02E88(PairDisplay *display, int round, void *source);

int func_08A02300(PairDisplay *display, DisplayDef *def)
{
    display->reset();
    int result = -1;
    if (def == 0 || def->side >= 2 || def->source == 0 || def->layout == 0 || def->frame == 0 ||
        def->digits == 0 || def->round < 0) {
        return result;
    }
    unsigned int side = def->side;
    void *source = def->source;
    void *layout = def->layout;
    void *frame = def->frame;
    void *digits = def->digits;
    int round = def->round;
    for (int i = 0; i < 2; i++) {
        display->children[i] = func_08A02E88(display, round, source);
        if (display->children[i] == 0) {
            return -2;
        }
        DisplayChild *child = display->children[i];
        ChildDef child_def;
        child_def.side = side;
        child_def.index = i;
        child_def.source = source;
        child_def.layout = layout;
        child_def.frame = frame;
        child_def.digits = digits;
        child_def.round = round;
        if (child->open(&child_def) != 0) {
            return -3;
        }
    }
    display->side = side;
    display->source = source;
    display->open = 1;
    return 0;
}

}
