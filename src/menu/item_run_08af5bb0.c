/* Item kind 0 draws its frame and label; kind 2 runs its callback if any. */
struct Item08AF5BB0 {
    short field_00;
    char pad_02[6];
    void *field_08;
    char *field_0C;
    char *field_10;
    char pad_14[0x24 - 0x14];
    unsigned short *field_24;
    char pad_28[0x38 - 0x28];
    void (*field_38)(struct Item08AF5BB0 *item);
};

extern unsigned short D_08B00CC2[];
extern void func_088D3EA0(void *owner, char *layout, unsigned short *table);
extern void func_088D4664(char *layout, char *cursor, unsigned short *table);

void func_08AF5BB0(void *self, struct Item08AF5BB0 *item)
{
    if (item->field_00 == 0) {
        func_088D3EA0(item->field_08, item->field_0C, item->field_24);
        func_088D4664(item->field_0C, item->field_10, D_08B00CC2);
    } else if (item->field_00 == 2 && item->field_38 != 0) {
        item->field_38(item);
    }
}
