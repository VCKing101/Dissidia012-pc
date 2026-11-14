struct MenuWidget {
    char pad_00[0xC];
    short field_0C;
    char pad_0E[0x12];
    unsigned short field_20;

    static bool Has(MenuWidget *widget, unsigned int mask) {
        return (widget->field_20 & mask) != 0;
    }
};

struct MenuPageState {
    char pad_000[0x684];
    MenuWidget *field_684;
};

struct MenuPageOwner {
    char pad_000[0x14C];
    MenuPageState *field_14C;
};

extern "C" {
void func_089F5990(MenuPageOwner *self, int flags);

int func_089F75DC(MenuPageOwner *self) {
    MenuWidget *widget = self->field_14C->field_684;
    int result = 0;
    if (widget->field_0C == 0x83) {
        if (MenuWidget::Has(widget, 0x20)) {
            func_089F5990(self, 0x80);
            result = 2;
        } else {
            result = 1;
        }
    }
    return result;
}
}
