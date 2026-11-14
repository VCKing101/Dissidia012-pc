/* Shows the grid with cell `index` temporarily set to `value`, then restores it. */
typedef struct PreviewRow {
    unsigned short field_00[10];
} PreviewRow;

typedef struct PreviewGrid {
    char pad_0000[0x1CA4];
    int field_1CA4;
    char pad_1CA8[0x3064 - 0x1CA8];
    PreviewRow field_3064[(0x35B4 - 0x3064) / 0x14];
    char field_35B4[0x10];
} PreviewGrid;

typedef struct PreviewOwner {
    char pad_0000[0x46AC];
    PreviewGrid *field_46AC;
} PreviewOwner;

extern void *D_08B01CF8;
extern void func_08AF4B2C(void *game);
extern int func_08AF96B0(void);
extern void func_08ADB9A0(PreviewOwner *self);
extern void func_08AF4B6C(void *game, void *grid, int zero);

void func_08ADBFB8(PreviewOwner *self, int index, int value)
{
    unsigned short saved = self->field_46AC->field_3064[self->field_46AC->field_1CA4].field_00[index];
    func_08AF4B2C(D_08B01CF8);
    if (func_08AF96B0() == 0) {
        self->field_46AC->field_3064[self->field_46AC->field_1CA4].field_00[index] = value;
    }
    func_08ADB9A0(self);
    func_08AF4B6C(D_08B01CF8, self->field_46AC->field_35B4, 0);
    if (func_08AF96B0() == 0) {
        self->field_46AC->field_3064[self->field_46AC->field_1CA4].field_00[index] = saved;
    }
}
