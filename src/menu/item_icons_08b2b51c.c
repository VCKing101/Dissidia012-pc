/* Shows the item's two icons (0x23 and 5) unless the state has no selection. */
struct Item08B2B51C {
    char pad_00[0xF4];
    void *field_F4;
    void *field_F8;
};

struct State08B2B51C {
    char pad_000[0x864];
    int field_864;
};

struct Owner08B2B51C {
    char pad_000[0xA18];
    struct State08B2B51C *field_A18;
};

extern void func_088D9F5C(void *part, int id, int show);

void func_08B2B51C(struct Owner08B2B51C *self, int unused, struct Item08B2B51C *item)
{
    if (self->field_A18->field_864 == -1) {
        return;
    }
    func_088D9F5C(item->field_F4, 0x23, 1);
    func_088D9F5C(item->field_F8, 5, 1);
}
