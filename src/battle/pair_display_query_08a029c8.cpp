/* Pair display queries: true when either child reports the state. */
class DisplayChild {
public:
    virtual void slot_008();
    virtual void slot_010();
    virtual void slot_018();
    virtual void slot_020();
    virtual void slot_028();
    virtual void slot_030();
    virtual void slot_038();
    virtual void slot_040();
    virtual void slot_048();
    virtual void slot_050();
    virtual void slot_058();
    virtual void slot_060();
    virtual void slot_068();
    virtual void slot_070();
    virtual int is_shown();
    virtual int is_moving();
    virtual int is_done();
};

class DisplayBase {
public:
    virtual void slot_008();
    virtual void slot_010();
    virtual void slot_018();
    virtual void slot_020();
    virtual void slot_028();
    virtual void slot_030();
    virtual void slot_038();
    virtual int is_active();
};

class PairDisplay : public DisplayBase {
public:
    unsigned char open;
    unsigned char pad_05[3];
    DisplayChild *children[2];
};

extern "C" {

int func_08A029C8(PairDisplay *display)
{
    if (!display->is_active()) {
        return 0;
    }
    int result = 0;
    DisplayChild **children = display->children;
    for (int i = 0; i < 2; i++) {
        if (children[i]->is_shown() == 1) {
            result = 1;
            break;
        }
    }
    return result;
}

int func_08A02A78(PairDisplay *display)
{
    if (!display->is_active()) {
        return 0;
    }
    int result = 0;
    DisplayChild **children = display->children;
    for (int i = 0; i < 2; i++) {
        if (children[i]->is_moving() == 1) {
            result = 1;
            break;
        }
    }
    return result;
}

int func_08A02B28(PairDisplay *display)
{
    if (!display->is_active()) {
        return 0;
    }
    int result = 0;
    DisplayChild **children = display->children;
    for (int i = 0; i < 2; i++) {
        if (children[i]->is_done() == 1) {
            result = 1;
            break;
        }
    }
    return result;
}

}
