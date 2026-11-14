/* 14-byte rows; the symbol is the row's second halfword. */
typedef struct PickRow {
    short field_00;
    unsigned short field_02;
    unsigned short field_04;
    char pad_06[0x8];
} PickRow;

extern PickRow D_08A0AF92[];
extern int func_089EF420(void *self, int key);

unsigned short func_089EF4A0(void *self, int key, unsigned char second)
{
    int row = func_089EF420(self, key);
    if (second) {
        return D_08A0AF92[row].field_04;
    } else {
        return D_08A0AF92[row].field_02;
    }
}
