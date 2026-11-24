/* For the selected row, resolves the item index, refreshes the row and shows the left or right arrow for the given direction. */
extern void func_08B0F818(void *self, int row, int item);
extern void func_08B0F838(void *self, int item, int a);
extern void func_088D9F5C(void *w, int id, int a);

typedef struct Row0FA20 { char pad[0xA]; short base; short step2; short step; } Row0FA20;
typedef struct Arrow0FA20 { char pad[0xE]; short frame; } Arrow0FA20;
typedef struct Blk0FA20 {
    char pad0[0x834];
    Arrow0FA20 *left;
    Arrow0FA20 *right;
    char pad83C[0x24988 - 0x83C];
    int sel;
    int pad2498C;
    int pad24990;
    int pos;
} Blk0FA20;
typedef struct S0FA20 { char pad[0x138]; Blk0FA20 *blk; } S0FA20;
extern Row0FA20 *D_089D76EC;

void func_08B0FA20(S0FA20 *self, int dir)
{
    int sel = self->blk->sel;
    Row0FA20 *row;
    int item;
    if (sel == -1) {
        return;
    }
    row = &D_089D76EC[sel];
    item = self->blk->pos * row->step + row->base;
    func_08B0F818(self, sel, item);
    func_08B0F838(self, item, 1);
    if (dir < 0) {
        func_088D9F5C(self->blk->left, 0x44, 1);
        self->blk->left->frame = 0x43;
    } else if (dir > 0) {
        func_088D9F5C(self->blk->right, 0x47, 1);
        self->blk->right->frame = 0x46;
    }
}
