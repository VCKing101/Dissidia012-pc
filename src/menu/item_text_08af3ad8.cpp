/* Text id for the item: its second text when flag 2 is set, 0 when unavailable. */
struct TextItem {
    char pad_0[0x4];
    unsigned short field_04;

    static bool hasFlag(unsigned int flags, unsigned int mask) { return (flags & mask) != 0; }
};

struct TextInfo {
    char pad_00[0xE];
    short field_0E;
    short field_10;
};

struct TextList {
    char pad_000[0xC80];
    char field_C80[0x10];
};

struct TextListOwner {
    char pad_000[0x140];
    TextList *field_140;
};

extern "C" TextItem *func_088B8414(void *list, int id, int one);
extern "C" TextInfo *func_088B1D9C(int id);

extern "C" {
int func_08AF3AD8(TextListOwner *self, int id)
{
    TextItem *item = func_088B8414(self->field_140->field_C80, id, 1);
    TextInfo *info = func_088B1D9C(id);
    if (info == 0) {
        return 0;
    }
    if (item == 0) {
        return 0;
    }
    if (!TextItem::hasFlag(item->field_04, 2)) {
        return info->field_0E;
    }
    return info->field_10;
}
}
