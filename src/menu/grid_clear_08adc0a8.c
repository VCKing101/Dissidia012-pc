/* Clears the current grid row to 0x7FFF and redraws the grid. */
typedef struct ClearGridRow {
    unsigned short field_00[10];
} ClearGridRow;

typedef struct ClearGrid {
    char pad_0000[0x1CA4];
    int field_1CA4;
    int field_1CA8;
    char pad_1CAC[0x3064 - 0x1CAC];
    ClearGridRow field_3064[16];
} ClearGrid;

typedef struct ClearGridOwner {
    char pad_0000[0x46AC];
    ClearGrid *field_46AC;
} ClearGridOwner;

extern void func_08ADB9A0(ClearGridOwner *self);

void func_08ADC0A8(ClearGridOwner *self)
{
    int i;
    for (i = 0; i < self->field_46AC->field_1CA8; i++) {
        self->field_46AC->field_3064[self->field_46AC->field_1CA4].field_00[i] = 0x7FFF;
    }
    func_08ADB9A0(self);
}
