/* Subtracts func_08971010(world->w18, n) from the per-slot counter at 0xB4 + slot*4 of the global world object D_09B6F478. */
typedef struct { char pad[0x18]; unsigned short slot; } Info0895B92C;
typedef struct { char pad[0x1C8]; Info0895B92C *info; } Row0895B92C;
typedef struct World0895B92C { char pad[0x18]; int w18; char pad2[0x88 - 0x1C]; Row0895B92C *row; char pad3[0xB4 - 0x8C]; int cnt[1]; } World0895B92C;
extern World0895B92C *D_09B6F478;
extern int func_08971010(int, int);
void func_0895B92C(int n) {
    if (n == 0) { return; }
    World0895B92C *w = D_09B6F478;
    int d = func_08971010(w->w18, n);
    w->cnt[w->row->info->slot] -= d;
}
