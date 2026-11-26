/* Battle ending screen dispatch: calls the page update for the battle mode, then func_088AC8C8. */
struct G10 { char p0[0x4C]; int mode; char p1[4]; int idx; };
struct Ent { char b0; char b1; char p[11]; };
struct G85 { char p0[0x2A04]; Ent ent[1]; };
extern "C" {
extern G10 *D_089D6A10;
extern G85 *D_089D8508;
extern void *D_089D712C;
extern char D_09AFB8A0[];
extern int func_088F7050(int a, int b);
extern void func_088AC8C8(void *a, unsigned char b);
extern void func_08B08C14(void *a);
extern void func_08B09014(void *a);
extern void func_08B091A4(void *a);
extern void func_08B0C324(void *a);
extern void func_08B07974(void *self) {
    char *sub;
    int offs, base;
    switch (D_089D6A10->mode) {
    case 10:
        if ((unsigned char)((*(unsigned short *)(D_09AFB8A0 + 0x2784) & 0x40) != 0) == 1) {
            func_08B0C324(self);
        } else {
            func_08B08C14(self);
        }
        break;
    case 9:
    case 11:
    case 12:
        break;
    case 1:
    case 2:
    case 7:
        break;
    case 5:
        offs = D_089D6A10->idx * 13;
        base = (int)D_089D8508->ent;
        sub = D_09AFB8A0 + 0x25A8;
        if (func_088F7050(D_089D6A10->idx, ((Ent *)(offs + base))->b1) == 1) {
            if (sub[8] == 6) {
                func_08B091A4(self);
                func_08B0C324(self);
            } else {
                func_08B08C14(self);
            }
        } else if (sub[8] != 6) {
            func_08B09014(self);
        }
        break;
    default:
        func_08B08C14(self);
        break;
    }
    unsigned char b = 1;
    self = D_089D712C;
    func_088AC8C8(self, b);
}
}
