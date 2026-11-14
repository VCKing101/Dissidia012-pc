/* Shows the pair (icon `id` and its frame) when `on` is 1, else hides both. */
struct State08B26524 {
    char pad_00[0x3C];
    void *field_3C;
    void *field_40;
};

struct Owner08B26524 {
    struct State08B26524 *field_0;
};

extern short D_08B27F40[];
extern void func_088D9F5C(void *part, int id, int show);
extern void func_088D9CD8(void *part);
extern void func_088D9CFC(void *part);

void func_08B26524(struct Owner08B26524 *self, unsigned char on, int id)
{
    if (on == 1) {
        func_088D9F5C(self->field_0->field_40, D_08B27F40[id], 1);
        func_088D9CD8(self->field_0->field_3C);
    } else {
        func_088D9CFC(self->field_0->field_40);
        func_088D9CFC(self->field_0->field_3C);
    }
}
