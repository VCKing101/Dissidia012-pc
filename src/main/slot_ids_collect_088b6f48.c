extern int func_088B6F00(int);
extern int func_088B6E14(int, int);

void func_088B6F48(short *out, int a, int b, int c)
{
    int tbl = func_088B6F00(a);
    short *start;
    short *end;
    short *cur;
    short *q;
    int i;
    out[0] = 0;
    out[0x51] = 0;
    if (tbl != 0) {
        int s = func_088B6E14(tbl, b);
        end = (short *)func_088B6E14(tbl, c);
        if (s == 0) {
            cur = *(short **)(tbl + 8);
        } else {
            cur = (short *)(s + 8);
        }
        for (; cur <= end; cur = cur + 4) {
            i = 0;
            q = cur + 1;
            do {
                short v;
                v = *q;
                if (v != -1) {
                    short n = out[0];
                    if (n < 0x50) {
                        out[n + 1] = v;
                        out[0] = n + 1;
                    } else {
                        return;
                    }
                }
                q = q + 1;
                i = i + 1;
            } while (i < 3);
            out[0x51] = out[0x51] + (short)*(char *)((int)cur + 1);
        }
    }
}
