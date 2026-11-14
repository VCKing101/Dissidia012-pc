/* Sets or clears bit 0x20 of value 5, sets value 12 (the menu info's field
   0x210) and redraws with mask 0x440, unless func_08AB6DAC reports the menu busy. */
struct Info08AB5358 {
    char pad_000[0x210];
    int field_210;
};

struct Menu08AB5358;

extern int func_08AB6DAC(struct Menu08AB5358 *menu);
extern struct Info08AB5358 *func_08AB6D58(struct Menu08AB5358 *menu);
extern unsigned int func_08AB73A0(struct Menu08AB5358 *menu, int key);
extern void func_08AB58E0(struct Menu08AB5358 *menu, int key, int value);
extern void func_08AB5A20(struct Menu08AB5358 *menu, int mask);
extern void func_08AB5D58(struct Menu08AB5358 *menu);
extern void func_08AB5A30(struct Menu08AB5358 *menu, int mask);

void func_08AB5358(struct Menu08AB5358 *self, int on)
{
    int info;
    unsigned int value;
    int mask;
    if (func_08AB6DAC(self) == 1) {
        return;
    }
    info = func_08AB6D58(self)->field_210;
    value = func_08AB73A0(self, 5);
    if (on == 0) {
        value = value & ~0x20;
    } else {
        value = value | 0x20;
    }
    func_08AB58E0(self, 5, value);
    func_08AB58E0(self, 0xC, info);
    mask = 0x440;
    func_08AB5A20(self, mask);
    func_08AB5D58(self);
    func_08AB5A30(self, mask);
}
