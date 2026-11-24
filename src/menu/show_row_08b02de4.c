/* Shows or hides the widgets of row `row` for the given mode and applies the row's frame. */
extern void func_088D9CFC(void *w);
extern void func_088D9CD8(void *w);
extern void func_08B02ED0(void *self, int mode, int a, int b, int frame, const char *tag, int shown);
extern const char D_08B04514[];

typedef struct B02DE4 { char pad[0x438]; void *w[1]; } B02DE4;
typedef struct S02DE4 { char pad[0x67C]; void **blk; } S02DE4;

void func_08B02DE4(S02DE4 *self, int mode, int row)
{
    int shown;
    if (mode >= 0x65) {
        return;
    }
    if (mode == -1) {
        func_088D9CFC(self->blk[row + 0x10E]);
        func_088D9CFC(self->blk[row + 0x112]);
        shown = 0;
    } else {
        func_088D9CD8(self->blk[row + 0x10E]);
        func_088D9CFC(self->blk[row + 0x112]);
        if (mode == 100) {
            func_088D9CD8(self->blk[row + 0x112]);
        }
        shown = 1;
    }
    func_08B02ED0(self, mode, 3, 0, (short)(row + 0xF), D_08B04514, shown);
}
