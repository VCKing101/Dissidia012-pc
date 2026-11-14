/* Asks the system dialog to open; on success starts the menu and moves to state 2. */
struct SystemDialog {
    char pad_00[0x34];
    virtual int open(unsigned long long request);
};

struct MenuOpen {
    int field_00;
    char list[0x2C - 4];
    int state;
};

extern "C" {
extern unsigned long long D_089CCB40;
extern SystemDialog *func_088729A0(int index);
extern void func_088ECD7C(void);
extern void func_088EC6BC(void *list, void (*done)(void), MenuOpen *menu);

void func_088ECC10(MenuOpen *menu)
{
    if (func_088729A0(0)->open(D_089CCB40) == 1) {
        func_088EC6BC(menu->list, func_088ECD7C, menu);
        menu->state = 2;
    }
}
}
