/* Runs the current step of a 8-step member-function table copied from .data. */
class ShopState {
public:
    char pad_0000[0xE44];
    int field_E44;
};

class ShopScreen {
public:
    char pad_000[0x14C];
    ShopState *field_14C;
    virtual void v0();
};

typedef void (ShopScreen::*ShopScreenStep)();
struct ShopScreenTable {
    ShopScreenStep steps[8];
};

extern "C" void func_08935524(void *dst, const void *src, int size);
extern "C" const ShopScreenTable D_08A457F4;

extern "C" {
void func_08A40F04(ShopScreen *self)
{
    ShopScreenTable table;
    func_08935524(&table, &D_08A457F4, sizeof(table));
    (self->*table.steps[self->field_14C->field_E44])();
}
}
