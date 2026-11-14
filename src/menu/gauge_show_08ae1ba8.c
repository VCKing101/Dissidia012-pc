/* Shows the gauge for `value` (up to 100; -1 hides both parts, 100 also
   shows the second part) and redraws its digits. */
struct State08AE1BA8 {
    char pad_000[0x4AC];
    void *field_4AC;
    char pad_4B0[0x4BC - 0x4B0];
    void *field_4BC;
};

struct Screen08AE1BA8 {
    char pad_000[0x850];
    struct State08AE1BA8 *field_850;
};

extern unsigned short D_08AE45DA[];
extern void func_088D9CFC(void *part);
extern void func_088D9CD8(void *part);
extern void func_08AE1C58(struct Screen08AE1BA8 *self, int value, int digits, int a, int b,
                          unsigned short *table, int shown);

void func_08AE1BA8(struct Screen08AE1BA8 *self, int value)
{
    int shown;
    if (value > 100) {
        return;
    }
    if (value == -1) {
        func_088D9CFC(self->field_850->field_4AC);
        func_088D9CFC(self->field_850->field_4BC);
        shown = 0;
    } else {
        func_088D9CD8(self->field_850->field_4AC);
        if (value == 100) {
            func_088D9CD8(self->field_850->field_4BC);
        }
        shown = 1;
    }
    func_08AE1C58(self, value, 3, 0, 0x2C, D_08AE45DA, shown);
}
