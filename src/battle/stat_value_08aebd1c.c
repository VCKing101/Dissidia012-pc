// Returns a statistic selected by kind (0..13) from the records around p2 (and, for kind 7 and 10, from owner a0 or its timer); 0 for other kinds.
extern int func_0885ECF4(void *);
typedef struct { char pad[0x40]; unsigned v; } Sub08AEBD1C;
typedef struct { char pad[0x8C]; Sub08AEBD1C sub; } Stat08AEBD1C;
unsigned func_08AEBD1C(void *a0, int *p2, unsigned kind, unsigned char flag)
{
    unsigned char fl = flag;
    unsigned *t0 = (unsigned *)((char *)p2 + 8);
    unsigned *a3 = (unsigned *)((char *)p2[1] + 8);
    unsigned v = 0;
    switch (kind) {
    case 0:
        break;
    case 1:
        v = t0[0x1C / 4];
        break;
    case 2:
        v = t0[0x14 / 4] + t0[0x1C / 4];
        break;
    case 3:
        v = a3[0x50 / 4];
        break;
    case 4:
        v = a3[0x4C / 4];
        break;
    case 5:
        v = t0[0x10 / 4];
        break;
    case 6:
        v = t0[0x3C / 4];
        break;
    case 7:
        {
            char *st = *(char **)(*(char **)((char *)a0 + 8) + 0x30 + *(short *)(*p2 + 0x10CC) * 4);
            unsigned *q = (unsigned *)(st + 0x8C);
            v = q[0x40 / 4];
        }
        break;
    case 8:
        v = t0[8 / 4];
        break;
    case 9:
        if (fl == 1) {
            v = 1;
        }
        break;
    case 10:
        if (fl == 1) {
            int t = func_0885ECF4(*(char **)((char *)a0 + 8) + 0x5C);
            v = t / 1000 + (t % 1000 != 0);
        }
        break;
    case 11:
        v = t0[0xB0 / 4];
        break;
    case 12:
        v = a3[0x44 / 4];
        break;
    case 13:
        v = a3[0xB0 / 4];
    }
    return v;
}
