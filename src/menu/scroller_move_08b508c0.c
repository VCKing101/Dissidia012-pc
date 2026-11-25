/* Moves a scroller to a target row: jumps (mode 0) or steps one row at a time (mode 1) toward it and sets the state. */
typedef struct Sc { char p0[0x28]; int s28; int pad2C; int h30; char p1[0xEC - 0x34]; int cur; int pad; int tgt; } Sc;
extern void func_08B512B4(Sc *s, int from, int to);
extern void func_08B51360(Sc *s, int dir);
void func_08B508C0(Sc *s, int row, unsigned char step)
{
    int c;
    if (s->h30 == 0) {
        return;
    } else if ((c = s->cur) == row) {
        return;
    } else if (step != 0) {
        s->tgt = row;
        if (c <= row) {
            func_08B512B4(s, c, c + 1);
            func_08B51360(s, 1);
            s->cur = s->cur + 1;
        } else {
            func_08B512B4(s, c, c - 1);
            func_08B51360(s, -1);
            s->cur = s->cur - 1;
        }
        s->s28 = 3;
    } else {
        func_08B512B4(s, c, row);
        if (s->cur <= row) {
            func_08B51360(s, 1);
        } else {
            func_08B51360(s, -1);
        }
        s->cur = row;
        s->s28 = 1;
    }
}
