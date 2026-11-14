/* Re-attaches the unit's controller to its body unless blocked. */
struct Unit08831744 {
    char pad_000[0x330];
    char body[0x58];
    unsigned long long flags;
    char pad_390[4];
    void *controller;

    static bool any(Unit08831744 *u, unsigned long long mask) { return (u->flags & mask) != 0; }
};

extern "C" {

extern unsigned long long D_089C7F00;

int func_08837990(Unit08831744 *u);
void func_0886ADBC(void *controller, void *body);

void func_08831744(Unit08831744 *u)
{
    if (func_08837990(u) != 0) {
        return;
    }
    if (u->controller == 0) {
        return;
    }
    if (Unit08831744::any(u, D_089C7F00)) {
        return;
    }
    func_0886ADBC(u->controller, u->body);
}

}
