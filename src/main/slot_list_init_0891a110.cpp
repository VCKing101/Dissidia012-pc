extern "C" {
extern void func_0891A0F4(void *slot);

typedef struct SlotEntry {
    char data[0x2E8];
} SlotEntry;

typedef struct SlotList {
    int field_00;
    short field_04;
    char pad_06[0x20 - 6];
    SlotEntry entries[5];       /* 0x20 */
} SlotList;

void func_0891A110(SlotList *list)
{
    list->field_00 = 0;
    list->field_04 = 0;
    for (int i = 0; i < 5; i++) {
        func_0891A0F4(&list->entries[i]);
    }
}
}
