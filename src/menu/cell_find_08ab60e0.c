/* Returns the column of row `row` (the menu's default row when -1) whose cell
   holds the menu info's field 0x210, -1 if none, or 0 while busy. */
struct Info08AB60E0 {
    char pad_000[0x210];
    int field_210;
};

struct CellRow08AB60E0 {
    short field_0[6];
};

struct CellTable08AB60E0 {
    char pad_000[0x118];
    struct CellRow08AB60E0 field_118[1];
};

struct Menu08AB60E0;

extern struct CellTable08AB60E0 D_09B6DF08;
extern int func_08AB6DAC(struct Menu08AB60E0 *menu);
extern struct Info08AB60E0 *func_08AB6D58(struct Menu08AB60E0 *menu);
extern int func_08A6FE48(struct Info08AB60E0 *info);

int func_08AB60E0(struct Menu08AB60E0 *self, int row)
{
    struct Info08AB60E0 *info;
    int value;
    int pick;
    struct CellRow08AB60E0 *rows;
    int i;
    if (func_08AB6DAC(self) == 1) {
        return 0;
    }
    info = func_08AB6D58(self);
    value = info->field_210;
    pick = func_08A6FE48(info);
    rows = D_09B6DF08.field_118;
    if (row != -1) {
        pick = row;
    }
    for (i = 0; i < 5; i++) {
        if (rows[pick].field_0[i] == value) {
            return i;
        }
    }
    return -1;
}
