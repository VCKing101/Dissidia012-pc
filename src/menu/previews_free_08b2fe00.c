/* Releases every preview, reloads the layout and enters mode 5. */
struct State08B2FE00 {
    char pad_000[4];
    int field_004;
    char pad_008[0xC - 8];
    void *field_00C;
    char field_010[0x490 - 0x10];
    void *field_490[0x6D];
};

struct Owner08B2FE00 {
    char pad_000[0x9D8];
    struct State08B2FE00 *field_9D8;
};

extern unsigned short D_08B310F8[];
extern void func_0887BA5C(void *preview);
extern void func_088D3C24(void *owner, char *layout, unsigned short *table, int size);

int func_08B2FE00(struct Owner08B2FE00 *self)
{
    int i;
    for (i = 0; i < 0x6D; i++) {
        if (self->field_9D8->field_490[i] != 0) {
            func_0887BA5C(self->field_9D8->field_490[i]);
            self->field_9D8->field_490[i] = 0;
        }
    }
    func_088D3C24(self->field_9D8->field_00C, self->field_9D8->field_010, D_08B310F8, 0x120);
    self->field_9D8->field_004 = 5;
    return 0;
}
