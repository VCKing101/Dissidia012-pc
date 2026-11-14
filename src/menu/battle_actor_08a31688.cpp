class MenuBattleActor {
public:
    char data[0x544];
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
    virtual void slot_080();
    virtual void slot_088();
    virtual void slot_090();
    virtual void slot_098();
    virtual void slot_0A0();
    virtual void slot_0A8();
    virtual void slot_0B0();
    virtual void slot_0B8();
    virtual void slot_0C0();
    virtual void slot_0C8();
    virtual void slot_0D0();
    virtual void slot_0D8();
    virtual void slot_0E0();
    virtual void slot_0E8();
    virtual void slot_0F0();
    virtual void slot_0F8();
};

struct MenuBattleScene {
    char pad_00[0x3C];
    MenuBattleActor *field_3C;
};

struct MenuBattleView {
    char pad_00[0x48];
    MenuBattleScene *field_48;
};

extern "C" {
void func_08A31688(MenuBattleView *self) {
    MenuBattleActor *actor = self->field_48->field_3C;
    if (actor != 0) {
        actor->slot_0F8();
        self->field_48->field_3C = 0;
    }
}
}
