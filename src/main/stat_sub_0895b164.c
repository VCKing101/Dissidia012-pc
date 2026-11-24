/* Subtracts func_08971010(world->w18, n) from the per-slot counter at 0xB4 + slot*4 of the global world object D_09B6F478. */
typedef struct { char pad[0x18]; unsigned short slot; } Info0895B164;
typedef struct { char pad[0x1C8]; Info0895B164 *info; } Row0895B164;
typedef struct World0895B164 { char pad[0x18]; int w18; char pad2[0x88 - 0x1C]; Row0895B164 *row; char pad3[0xB4 - 0x8C]; int cnt[1]; } World0895B164;
extern World0895B164 *D_09B6F478;
extern int func_08971010(int, int);
void func_0895B164(int n) {
    if (n == 0) { return; }
    World0895B164 *w = D_09B6F478;
    int d = func_08971010(w->w18, n);
    w->cnt[w->row->info->slot] -= d;
}
