// Adds an amount looked up for entry n to the current world's per-slot counter.
struct AInfo { char pad[0x18]; unsigned short slot; };
struct ARow { char pad[0x1C8]; AInfo *info; };
struct AWorld { char pad[0x18]; int w18; char pad2[0x88 - 0x1C]; ARow *row; char pad3[0xB4 - 0x8C]; int cnt[1]; };
extern "C" AWorld *D_09B6F478;
extern "C" int func_08971134(int, int, int *, int);
struct AStat { static void add(int n) {
    struct { int amount; AWorld *w; } l;
    AWorld *w = D_09B6F478;
    int h = w->w18;
    l.w = w;
    int ok = func_08971134(h, n, &l.amount, 1);
    w = l.w;
    if (ok) { w->cnt[w->row->info->slot] += l.amount; }
} };
extern "C" void func_08960DFC(int n) { AStat::add(n); }
