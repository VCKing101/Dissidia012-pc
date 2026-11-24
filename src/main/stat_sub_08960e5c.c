/* Subtracts func_08971010(world->w18, n) from the per-slot counter at 0xB4 + slot*4 of the global world object D_09B6F478. */
typedef struct { char pad[0x18]; unsigned short slot; } Info08960E5C;
typedef struct { char pad[0x1C8]; Info08960E5C *info; } Row08960E5C;
typedef struct World08960E5C { char pad[0x18]; int w18; char pad2[0x88 - 0x1C]; Row08960E5C *row; char pad3[0xB4 - 0x8C]; int cnt[1]; } World08960E5C;
extern World08960E5C *D_09B6F478;
extern int func_08971010(int, int);
void func_08960E5C(int n) {
    if (n == 0) { return; }
    World08960E5C *w = D_09B6F478;
    int d = func_08971010(w->w18, n);
    w->cnt[w->row->info->slot] -= d;
}
