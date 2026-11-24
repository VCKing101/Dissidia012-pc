/* Subtracts func_08971010(world->w18, n) from the per-slot counter at 0xB4 + slot*4 of the global world object D_09B6F478. */
typedef struct { char pad[0x18]; unsigned short slot; } Info089699A4;
typedef struct { char pad[0x1C8]; Info089699A4 *info; } Row089699A4;
typedef struct World089699A4 { char pad[0x18]; int w18; char pad2[0x88 - 0x1C]; Row089699A4 *row; char pad3[0xB4 - 0x8C]; int cnt[1]; } World089699A4;
extern World089699A4 *D_09B6F478;
extern int func_08971010(int, int);
void func_089699A4(int n) {
    if (n == 0) { return; }
    World089699A4 *w = D_09B6F478;
    int d = func_08971010(w->w18, n);
    w->cnt[w->row->info->slot] -= d;
}
