/* Sets bit 0x40 of value 5, sets value 12 (the menu info's field 0x210) and
   redraws with mask 0x840, unless func_08AB6DAC reports the menu busy. */
struct Info08AB56EC {
    char pad_000[0x210];
    int field_210;
};

struct Menu08AB56EC;

extern int func_08AB6DAC(struct Menu08AB56EC *menu);
extern struct Info08AB56EC *func_08AB6D58(struct Menu08AB56EC *menu);
extern unsigned int func_08AB73A0(struct Menu08AB56EC *menu, int key);
extern void func_08AB58E0(struct Menu08AB56EC *menu, int key, int value);
extern void func_08AB5A20(struct Menu08AB56EC *menu, int mask);
extern void func_08AB5D58(struct Menu08AB56EC *menu);
extern void func_08AB5A30(struct Menu08AB56EC *menu, int mask);

void func_08AB56EC(struct Menu08AB56EC *self)
{
    int mask;
    int info;
    unsigned int value;
    if (func_08AB6DAC(self) == 1) {
        return;
    }
    mask = 0x840;
    info = func_08AB6D58(self)->field_210;
    value = func_08AB73A0(self, 5);
    func_08AB58E0(self, 5, value | 0x40);
    func_08AB58E0(self, 0xC, info);
    func_08AB5A20(self, mask);
    func_08AB5D58(self);
    func_08AB5A30(self, mask);
}
