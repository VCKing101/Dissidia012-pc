/* Subtracts func_08971010(world->w18, n) from the per-slot counter at 0xB4 + slot*4 of the global world object D_09B6F478. */
typedef struct { char pad[0x18]; unsigned short slot; } Info0895C31C;
typedef struct { char pad[0x1C8]; Info0895C31C *info; } Row0895C31C;
typedef struct World0895C31C { char pad[0x18]; int w18; char pad2[0x88 - 0x1C]; Row0895C31C *row; char pad3[0xB4 - 0x8C]; int cnt[1]; } World0895C31C;
extern World0895C31C *D_09B6F478;
extern int func_08971010(int, int);
void func_0895C31C(int n) {
    if (n == 0) { return; }
    World0895C31C *w = D_09B6F478;
    int d = func_08971010(w->w18, n);
    w->cnt[w->row->info->slot] -= d;
}
