class MenuAction {
public:
    virtual void slot_008();
    virtual void slot_010();
    virtual void slot_018();
    virtual void slot_020(int value);
    virtual void slot_028();
    virtual void slot_030(int value);
};

struct MenuActionOwner {
    char pad_000[0x270];
    MenuAction *field_270;
};

extern "C" {
void func_08A292BC(MenuActionOwner *self) {
    self->field_270->slot_008();
}

void func_08A292EC(MenuActionOwner *self) {
    self->field_270->slot_010();
}

void func_08A2931C(MenuActionOwner *self) {
    self->field_270->slot_018();
}

void func_08A2934C(MenuActionOwner *self, int value) {
    self->field_270->slot_020(value);
}

void func_08A2937C(MenuActionOwner *self) {
    self->field_270->slot_028();
}

void func_08A293AC(MenuActionOwner *self, int value) {
    self->field_270->slot_030(value);
}
}
