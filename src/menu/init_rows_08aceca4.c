typedef struct Blk {
    char pad[0x120C]; int w120C; char pad2[0x1430 - 0x1210]; int cnt[9];
    char pad3[0xA03A - 0x1454]; unsigned short rows[9][150];
} Blk;
typedef struct Wk { char pad[0xDB30]; Blk *b; } Wk;
extern void func_08ACEA5C(Wk *, int, int);
extern void func_08AD7330(Wk *, int, int);
void func_08ACECA4(Wk *self, int sel)
{
    Blk *b = self->b;
    int i, j;
    if (sel != -1) {
        if (b->w120C != sel) {
            return;
        }
    }
    {
        for (i = 0; i < 9; i++) {
            for (j = 0; j < self->b->cnt[i]; j++) {
                self->b->rows[i][j] = j;
            }
        }
        for (i = 0; i < 9; i++) {
            for (j = 0; j < self->b->cnt[i]; j++) {
                func_08ACEA5C(self, i, j);
            }
        }
        if (self->b->w120C != 0) {
            for (i = 4; i < 9; i++) {
                for (j = 0; j < self->b->cnt[i]; j++) {
                    func_08AD7330(self, i, j);
                }
            }
        }
    }
}
