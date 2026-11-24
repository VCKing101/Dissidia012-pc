// Subtracts the cost of two entries from the current world's per-slot counter.
struct SInfo { char pad[0x18]; unsigned short slot; };
struct SRow { char pad[0x1C8]; SInfo *info; };
struct SWorld {
    char pad[0x18]; int w18; char pad2[0x88 - 0x1C]; SRow *row; char pad3[0xB4 - 0x8C]; int cnt[1];
};
extern "C" SWorld *D_09B6F478;
extern "C" int func_08971010(int, int);
struct SStat {
    static void sub(int n) {
        SWorld *w = D_09B6F478;
        int d = func_08971010(w->w18, n);
        w->cnt[w->row->info->slot] -= d;
    }
};
struct SPair {
    static void run(int **a3) {
        int *p = *a3;
        if (p != 0) {
            if (*p != 0) {
                SStat::sub(*p);
            }
            if (p != 0) {
                SStat::sub((int)p);
            }
        }
    }
};
extern "C" void func_08977514(int a0, int a1, int a2, int **a3)
{
    SPair::run(a3);
}
