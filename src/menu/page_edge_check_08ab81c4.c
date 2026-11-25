/* Checks whether the cursor sits on the first (key 0x40) or last entry of the page list. */
extern int func_08AB8128(void *self, int a);
extern int *func_08AB8150(void *self, int a);
extern int func_08AB9B84(void *self, int a);
extern int func_088F45DC(int a);

typedef struct PBk { char pad[0x8B0]; int sel; char pad2[0xAB4 - 0x8B4]; unsigned char tab[1]; } PBk;
typedef struct PB { char pad[0x138]; PBk *blk; } PB;

int func_08AB81C4(PB *self, int key)
{
    int sel = self->blk->sel;
    int *list;
    unsigned int idx;
    if (func_08AB8128(self, sel) == 0) {
        return -1;
    }
    list = func_08AB8150(self, sel);
    idx = self->blk->tab[func_088F45DC(func_08AB9B84(self, sel))];
    if (key == 0x40) {
        if (idx == 0) {
            return 1;
        }
    } else {
        if (idx == *list - 1) {
            return 1;
        }
    }
    return 0;
}
