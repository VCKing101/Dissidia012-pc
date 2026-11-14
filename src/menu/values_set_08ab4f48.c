/* Sets values 11, 4 and 12 (the menu info's field 0x210), then redraws
   with mask 0x70, unless func_08AB6DAC reports the menu busy. */
struct Info08AB4F48 {
    char pad_000[0x210];
    int field_210;
};

struct Menu08AB4F48;

extern int func_08AB6DAC(struct Menu08AB4F48 *menu);
extern struct Info08AB4F48 *func_08AB6D58(struct Menu08AB4F48 *menu);
extern void func_08AB58E0(struct Menu08AB4F48 *menu, int key, int value);
extern void func_08AB5A20(struct Menu08AB4F48 *menu, int mask);
extern void func_08AB5D58(struct Menu08AB4F48 *menu);
extern void func_08AB5A30(struct Menu08AB4F48 *menu, int mask);

void func_08AB4F48(struct Menu08AB4F48 *self, int first, int second)
{
    int mask;
    int info;
    if (func_08AB6DAC(self) == 1) {
        return;
    }
    mask = 0x70;
    info = func_08AB6D58(self)->field_210;
    func_08AB58E0(self, 0xB, first);
    func_08AB58E0(self, 4, second);
    func_08AB58E0(self, 0xC, info);
    func_08AB5A20(self, mask);
    func_08AB5D58(self);
    func_08AB5A30(self, mask);
}
