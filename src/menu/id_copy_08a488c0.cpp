/* Copies the value of every id in a list from one bank to another. */
struct IdListHead {
    char pad_00[0xE];
    short field_0E;
};

struct IdList {
    IdListHead *field_00;
    char pad_04[0xC];
    short *field_10;
};

struct IdBank {
    char pad_0000[0xE7D8];
    char field_E7D8[1];
};

extern "C" int func_088B90F8(char *bank, int id);
extern "C" void func_088B8F08(char *bank, int id, int value);

extern "C" {
void func_08A488C0(void *self, IdBank *to, IdBank *from, IdList *list)
{
    char *dst = to->field_E7D8;
    char *src = from->field_E7D8;
    int count = list->field_00->field_0E;
    for (int i = 0; i < count; i++) {
        short *id = &list->field_10[i];
        int value = func_088B90F8(src, *id);
        func_088B8F08(dst, *id, value);
    }
}
}
