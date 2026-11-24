/* Subtracts func_08971010(world->w18, n) from the per-slot counter at 0xB4 + slot*4 of the global world object D_09B6F478. */
typedef struct { char pad[0x18]; unsigned short slot; } Info0898E938;
typedef struct { char pad[0x1C8]; Info0898E938 *info; } Row0898E938;
typedef struct World0898E938 { char pad[0x18]; int w18; char pad2[0x88 - 0x1C]; Row0898E938 *row; char pad3[0xB4 - 0x8C]; int cnt[1]; } World0898E938;
extern World0898E938 *D_09B6F478;
extern int func_08971010(int, int);
void func_0898E938(int n) {
    if (n == 0) { return; }
    World0898E938 *w = D_09B6F478;
    int d = func_08971010(w->w18, n);
    w->cnt[w->row->info->slot] -= d;
}
