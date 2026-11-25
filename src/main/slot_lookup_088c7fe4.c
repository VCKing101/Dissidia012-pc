extern int D_089D712C;
extern int func_088C7F78(void);
extern int func_088C7F9C(unsigned int p);
extern int func_088C75B4(int base, unsigned int u);
extern int func_088FB324(int a, unsigned int u);
extern int func_088F51AC(int p);
extern int func_088F51DC(int p);
extern int func_088F4EB4(int p, unsigned int u);
int func_088C7FE4(unsigned int p)
{
    int r = 0;
    if (func_088C7F78() == 1) {
        unsigned int u = p & 0x1F;
        switch (func_088C7F9C(p)) {
        case 0:
            r = func_088FB324(0, u);
            break;
        case 1:
            r = func_088FB324(1, u);
            break;
        case 2: {
            int v = func_088C75B4(D_089D712C + 0x39F38, u);
            if (v != 0) r = v;
            break;
        }
        case 3: {
            int *q = (int *)(D_089D712C + 0x1FDD8);
            int v, w;
            q = (int *)((char *)q + 0xF4);
            q = (int *)((char *)q + ((int)(p & 0x1F00) >> 8) * 0x10A8);
            v = func_088F51AC((int)q);
            if (v != 0) {
                r = func_088F4EB4(v, u);
            } else {
                w = func_088F51DC((int)q);
                if (w != 0) r = func_088F4EB4(w + ((int)(p & 0xE0) >> 5) * 0x2D8, u);
            }
            break;
        }
        }
    }
    return r;
}
