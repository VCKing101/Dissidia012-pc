/* Subtracts func_08971010(world->w18, n) from the per-slot counter at 0xB4 + slot*4 of the global world object D_09B6F478. */
typedef struct { char pad[0x18]; unsigned short slot; } Info08968C64;
typedef struct { char pad[0x1C8]; Info08968C64 *info; } Row08968C64;
typedef struct World08968C64 { char pad[0x18]; int w18; char pad2[0x88 - 0x1C]; Row08968C64 *row; char pad3[0xB4 - 0x8C]; int cnt[1]; } World08968C64;
extern World08968C64 *D_09B6F478;
extern int func_08971010(int, int);
void func_08968C64(int n) {
    if (n == 0) { return; }
    World08968C64 *w = D_09B6F478;
    int d = func_08971010(w->w18, n);
    w->cnt[w->row->info->slot] -= d;
}
