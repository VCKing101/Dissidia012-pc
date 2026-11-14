typedef struct TableHeader {
    char pad_00[2];
    unsigned short field_02;
} TableHeader;

extern TableHeader *D_08A3CCF0;
extern int D_08A3CD00;

int func_08A3C0C4(int index)
{
    if (D_08A3CD00 != 0) {
        if (index < 0 || (int)(D_08A3CCF0->field_02 - 1) < index) {
            index = 0;
        }
        return D_08A3CD00 + index * 0x40;
    }
    return 0;
}
