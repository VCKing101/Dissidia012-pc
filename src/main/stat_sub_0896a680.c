/* Subtracts func_08971010(world->w18, n) from the per-slot counter at 0xB4 + slot*4 of the global world object D_09B6F478. */
typedef struct { char pad[0x18]; unsigned short slot; } Info0896A680;
typedef struct { char pad[0x1C8]; Info0896A680 *info; } Row0896A680;
typedef struct World0896A680 { char pad[0x18]; int w18; char pad2[0x88 - 0x1C]; Row0896A680 *row; char pad3[0xB4 - 0x8C]; int cnt[1]; } World0896A680;
extern World0896A680 *D_09B6F478;
extern int func_08971010(int, int);
void func_0896A680(int n) {
    if (n == 0) { return; }
    World0896A680 *w = D_09B6F478;
    int d = func_08971010(w->w18, n);
    w->cnt[w->row->info->slot] -= d;
}
