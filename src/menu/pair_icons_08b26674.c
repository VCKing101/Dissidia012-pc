/* Shows the two icons of pair `index`. */
struct Pair08B26674 {
    short field_0;
    short field_2;
};

struct State08B26674 {
    char pad_00[0xA4];
    void *field_A4;
    void *field_A8;
};

struct Owner08B26674 {
    struct State08B26674 *field_0;
};

extern struct Pair08B26674 D_08B27F24[];
extern void func_088D9F5C(void *part, int id, int show);

void func_08B26674(struct Owner08B26674 *self, int index)
{
    func_088D9F5C(self->field_0->field_A4, D_08B27F24[index].field_0, 1);
    func_088D9F5C(self->field_0->field_A8, D_08B27F24[index].field_2, 1);
}
