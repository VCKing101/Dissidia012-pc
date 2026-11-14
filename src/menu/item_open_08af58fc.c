/* Opens an item: kind 0 registers its frames, kind 2 runs its callback; then shows it. */
typedef struct OpenItem {
    short field_00;
    char pad_02[0x6];
    void *field_08;
    void *field_0C;
    char pad_10[0x24 - 0x10];
    void *field_24;
    char pad_28[0x4];
    void (*field_2C)(struct OpenItem *item);
} OpenItem;

extern void func_088D3CF0(void *a, void *b, void *table);
extern void func_08AF5C28(void *self, OpenItem *item);

void func_08AF58FC(void *self, OpenItem *item)
{
    if (item->field_00 == 0) {
        func_088D3CF0(item->field_08, item->field_0C, item->field_24);
    } else if (item->field_00 == 2 && item->field_2C != 0) {
        item->field_2C(item);
    }
    func_08AF5C28(self, item);
}
