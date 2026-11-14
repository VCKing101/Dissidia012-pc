struct MenuScreenStatus {
    int field_00;
    unsigned int field_04;
    static bool Has(MenuScreenStatus *status, unsigned int mask) { return (status->field_04 & mask) != 0; }
};
struct MenuScreen { char pad_000[0x140]; MenuScreenStatus *field_140; };
extern "C" {
void func_089E1CA4(MenuScreen *self);
void func_089E1DD8(MenuScreen *self);
void func_089E21C8(MenuScreen *self);
void func_089E1598(MenuScreen *self) {
    func_089E1CA4(self);
    func_089E1DD8(self);
    func_089E21C8(self);
    if (!MenuScreenStatus::Has(self->field_140, 4)) { return; } self->field_140->field_00 = 7;
}
}
