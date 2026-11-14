/* Closes the menu at 0x90 unless it is busy, then moves to phase 6. */
typedef struct PhaseOwner {
    char pad_0000[0x90];
    char menu[0x1860 - 0x90];
    int phase;
} PhaseOwner;

extern int func_088E03B4(void *menu);
extern void func_088E0390(void *menu);
extern void func_08841EF8(void);

void func_088E9334(PhaseOwner *owner)
{
    void *menu = owner->menu;

    if (func_088E03B4(menu) != 0) {
        return;
    }
    func_088E0390(menu);
    func_08841EF8();
    owner->phase = 6;
}
