/* Draws the gauge digits for `value` (hidden when -1). */
struct Owner08B265A8;

extern "C" {
extern unsigned short D_08B27F0C[];
void func_08B26194(Owner08B265A8 *self, int value, int digits, int a, int b,
                   unsigned short *table, int shown);
}

struct Gauge08B265A8 {
    static void draw(int value, Owner08B265A8 *self, int shown)
    {
        func_08B26194(self, value, 3, 1, 0x1B, D_08B27F0C, shown);
    }
};

extern "C" {
void func_08B265A8(Owner08B265A8 *self, int value)
{
    int shown = 1;
    if (value == -1) {
        shown = 0;
    }
    Gauge08B265A8::draw(value, self, shown);
}
}
