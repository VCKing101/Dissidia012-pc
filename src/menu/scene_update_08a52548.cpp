class MenuScene {
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
    virtual void slot_078();
};

extern "C" {
void func_08A52C08(MenuScene *self);
void func_08A529F8(MenuScene *self);

void func_08A52548(MenuScene *self) {
    func_08A52C08(self);
    func_08A529F8(self);
    self->slot_078();
}
}
