/* Lays out the two rectangles of list row `index`; returns the rectangle count. */
typedef struct RowRect {
    short field_00;
    short field_02;
    short field_04;
    short field_06;
} RowRect;

typedef struct RowRectPage {
    char pad_0000[0xD18];
    RowRect field_D18[(0x1510 - 0xD18) / 0x28][5];
    int field_1510;
} RowRectPage;

typedef struct RowRectOwner {
    char pad_000[0x138];
    RowRectPage *field_138;
} RowRectOwner;

int func_08A2A6D8(RowRectOwner *self, int index)
{
    int count = 0;
    int top = index * self->field_138->field_1510;
    int column;
    RowRect *rect = &self->field_138->field_D18[index][count++];
    rect->field_00 = top + 10;
    rect->field_02 = 9;
    column = index + 0x31;
    rect->field_04 = column;
    rect->field_06 = 3;
    rect = &self->field_138->field_D18[index][count++];
    rect->field_00 = top + 12;
    rect->field_02 = 12;
    rect->field_04 = column;
    rect->field_06 = 0;
    return count;
}
