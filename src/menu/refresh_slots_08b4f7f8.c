/* Unless the main widget is in frame 1, resets it, refreshes the four slot values and updates each slot entry from the constant table. */
extern void func_088D9F5C(void *w, int id, int a);
extern void func_08B4FA0C(void *self, int a, int *out, int b, int c);
extern void func_08B4ED6C(char *slot, int a, void *w, int v, unsigned short b, unsigned short c, unsigned short d, unsigned short e);

typedef struct Row4F7F8 { unsigned short a; unsigned short b; unsigned short c; unsigned short d; unsigned short pad[4]; } Row4F7F8;
extern Row4F7F8 D_08B4FDE0[];

typedef struct W4F7F8 { char pad[0xC]; short frame; } W4F7F8;
typedef struct S4F7F8 { char pad[0x2C]; int f2C; W4F7F8 *w; int pad34; char pad38[4]; char slots[0x40 * 4]; char pad_f[0x1BC - 0x3C - 0x100]; int f1BC; } S4F7F8;

void func_08B4F7F8(S4F7F8 *self)
{
    int cell[4];
    int i;
    char *slot;
    int *p;
    Row4F7F8 *row;
    if (self->w->frame != 1) {
        func_088D9F5C(self->w, 1, 0);
        func_08B4FA0C(self, self->f1BC, cell, 9999, 1);
        i = 0;
        slot = self->slots;
        p = cell;
        row = D_08B4FDE0;
        do {
            func_08B4ED6C(slot, self->f2C, self->w, *p, row->a, row->b, row->c, row->d);
            i++;
            slot += 0x40;
            p++;
            row++;
        } while (i < 4);
    }
}
