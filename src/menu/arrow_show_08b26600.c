/* Shows the arrow and its label for the current mode when `on` is 1, else
   hides it. */
struct State08B26600 {
    char pad_000[0xAC];
    void *field_AC;
    char pad_0B0[0x1BC - 0xB0];
    int field_1BC;
};

struct Owner08B26600 {
    struct State08B26600 *field_0;
};

extern short D_08B27F20[];
extern void func_088D9CD8(void *part);
extern void func_088D9CFC(void *part);
extern void func_08B257AC(struct Owner08B26600 *self, int slot, int text);

void func_08B26600(struct Owner08B26600 *self, unsigned char on)
{
    if (on == 1) {
        func_088D9CD8(self->field_0->field_AC);
        func_08B257AC(self, 0x2A, D_08B27F20[self->field_0->field_1BC]);
    } else {
        func_088D9CFC(self->field_0->field_AC);
    }
}
