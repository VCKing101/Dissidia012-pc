class MenuLayer {
public:
    virtual void slot_008();
    virtual void slot_010();
    virtual void slot_018();
};

struct MenuLayerSet {
    char pad_00[0x84];
    MenuLayer *field_84[8];
};

extern "C" {
void func_08A52764(MenuLayerSet *self, int index) {
    self->field_84[index]->slot_010();
}

void func_08A5279C(MenuLayerSet *self, int index) {
    self->field_84[index]->slot_018();
}
}
