extern unsigned short *func_0887B500(int, int);
extern int func_0887E7F4(int);

int func_08A975E0(int a0, int text, int a2, int idx, int mode)
{
    unsigned short *p;
    int found = 0;
    int id = 0;
    p = func_0887B500(text, 4);
    if (p == 0) {
        return 0;
    }
    while (*p != 0) {
        if (*p == 0x1F) {
            id = p[1];
            found = 0;
            if (id > 100 && id < 0xC9) {
                if (id == idx + 1) {
                    found = 1;
                }
            }
        }
        if (*p == 0x10) if (found == 1 && id == idx + 1) {
            unsigned char *q = (unsigned char *)(p + 1);
            if (mode == 0) {
                q[3] = 0x81;
                q[2] = 0x81;
                q[1] = 0x81;
                q[0] = 0x81;
            } else if (mode != 1) {
                q[0] = 0x41;
                q[1] = 0x41;
                q[2] = 0x41;
                q[3] = 0x81;
            }
            return 1;
        }
        if (*p > 0x10 && *p < 0x20) {
            p += func_0887E7F4(*p);
        }
        p++;
    }
    return 0;
}
