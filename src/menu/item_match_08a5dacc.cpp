struct MatchRow {
    int field_00;
    char pad_04[0xC];
};

struct MatchItem {
    signed char field_00;
    char pad_01[3];
    short field_04[5];
};

extern "C" MatchRow D_08A5DC38[16];
extern "C" void func_08A5D754(void *self, int row);

extern "C" void func_08A5DACC(void *self, MatchItem *item)
{
    if (item->field_00 != 5) {
        return;
    }
    for (int i = 0; i < 5; i++) {
        if (item->field_04[i] < 0) {
            return;
        }
        for (int j = 0; j < 16; j++) {
            if (item->field_04[i] == D_08A5DC38[j].field_00) {
                func_08A5D754(self, j);
                break;
            }
        }
    }
}
