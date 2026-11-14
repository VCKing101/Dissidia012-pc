/* Shows icon `id` on part `index` when `on` is 1, else hides it. */
struct State08B26038 {
    char pad_00[0xB8];
    void *field_B8[1];
};

struct Owner08B26038 {
    struct State08B26038 *field_0;
};

extern short D_08B27F2C[];
extern void func_088D9F5C(void *part, int id, int show);
extern void func_088D9CFC(void *part);

void func_08B26038(struct Owner08B26038 *self, unsigned char on, int index, int id)
{
    if (on == 1) {
        func_088D9F5C(self->field_0->field_B8[index], D_08B27F2C[id], 1);
    } else {
        func_088D9CFC(self->field_0->field_B8[index]);
    }
}
