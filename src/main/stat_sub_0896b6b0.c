/* Subtracts func_08971010(world->w18, n) from the per-slot counter at 0xB4 + slot*4 of the global world object D_09B6F478. */
typedef struct { char pad[0x18]; unsigned short slot; } Info0896B6B0;
typedef struct { char pad[0x1C8]; Info0896B6B0 *info; } Row0896B6B0;
typedef struct World0896B6B0 { char pad[0x18]; int w18; char pad2[0x88 - 0x1C]; Row0896B6B0 *row; char pad3[0xB4 - 0x8C]; int cnt[1]; } World0896B6B0;
extern World0896B6B0 *D_09B6F478;
extern int func_08971010(int, int);
void func_0896B6B0(int n) {
    if (n == 0) { return; }
    World0896B6B0 *w = D_09B6F478;
    int d = func_08971010(w->w18, n);
    w->cnt[w->row->info->slot] -= d;
}
