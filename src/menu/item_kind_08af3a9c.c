/* Kind of the item with this id, or 0 when it is not in the list. */
typedef struct KindItem {
    short field_00;
    short field_02;
} KindItem;

typedef struct KindList {
    char pad_000[0xC80];
    char field_C80[0x10];
} KindList;

typedef struct KindListOwner {
    char pad_000[0x140];
    KindList *field_140;
} KindListOwner;

extern KindItem *func_088B8414(void *list, int id, int one);

int func_08AF3A9C(KindListOwner *self, int id)
{
    KindItem *item = func_088B8414(self->field_140->field_C80, id, 1);
    if (item != 0) {
        return item->field_02;
    }
    return 0;
}
