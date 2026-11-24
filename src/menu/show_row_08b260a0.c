/* Shows or hides the widgets of row `row` for the given mode and applies the row's frame. */
extern void func_088D9CFC(void *w);
extern void func_088D9CD8(void *w);
extern void func_08B26194(void *self, int mode, int a, int b, int frame, const char *tag, int shown);
extern const char D_08B27EF8[];

typedef struct S260A0 { void **blk; } S260A0;

void func_08B260A0(S260A0 *self, int mode, int row)
{
    int shown;
    int base = row * 5;
    if (mode >= 0x65) {
        return;
    }
    if (mode == -1) {
        func_088D9CFC(self->blk[base + 0x12]);
        func_088D9CFC(self->blk[base + 0x16]);
        shown = 0;
    } else {
        func_088D9CD8(self->blk[base + 0x12]);
        if (mode == 100 && row == 0) {
            func_088D9CD8(self->blk[base + 0x16]);
        }
        shown = 1;
    }
    func_08B26194(self, mode, 3, 0, (short)(base + 0x12), D_08B27EF8, shown);
}
