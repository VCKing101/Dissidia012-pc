/* Shows icon `which` of slot `slot`'s pair on the slot's part. */
struct State08B26BC8 {
    char pad_00[0x84];
    void *field_84[1];
};

struct Owner08B26BC8 {
    struct State08B26BC8 *field_0;
};

extern short D_08B27F30[];
extern void func_088D9F5C(void *part, int id, int show);

void func_08B26BC8(struct Owner08B26BC8 *self, int slot, int which)
{
    func_088D9F5C(self->field_0->field_84[slot], D_08B27F30[slot * 2 + which], 1);
}
