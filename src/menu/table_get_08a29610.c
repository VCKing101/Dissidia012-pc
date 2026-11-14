typedef struct TableHeader {
    char pad_00[0xC];
    unsigned short field_0C;
} TableHeader;

extern TableHeader *D_08A29BF0;
extern int D_08A29BF4;

int func_08A29610(int index)
{
    if (D_08A29BF0 != 0) {
        if (index < 0 || (int)(D_08A29BF0->field_0C - 1) < index) {
            index = 0;
        }
        return D_08A29BF4 + index * 0x40;
    }
    return 0;
}
