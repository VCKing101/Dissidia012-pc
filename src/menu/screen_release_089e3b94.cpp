struct MenuPartEntry {
    unsigned short field_00;
    unsigned short field_02;
    char pad_04[0x14];
};

struct MenuPartOwner {
    char pad_000[0x408];
    int field_408[1];
};

struct MenuScreen {
    char pad_000[0x140];
    MenuPartOwner *field_140;
};

extern "C" {
extern MenuPartEntry D_089E7518[];
void func_088D9CFC(int handle);

void func_089E3B94(MenuScreen *self) {
    for (int i = 4; i < 7; i++) {
        func_088D9CFC(self->field_140->field_408[D_089E7518[i].field_02]);
    }
}
}
