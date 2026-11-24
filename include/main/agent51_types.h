#ifndef AGENT51_TYPES_H
#define AGENT51_TYPES_H
/* Shared types for agent51 matches. */

/* Per-slot resource counters of the global world object D_09B6F478. */
struct A51Info { char pad[0x18]; unsigned short slot; };
struct A51Row { char pad[0x1C8]; A51Info *info; };
struct A51World {
    char pad[0x18]; int w18; char pad2[0x88 - 0x1C]; A51Row *row; char pad3[0xB4 - 0x8C]; int cnt[1];
};
extern "C" A51World *D_09B6F478;
extern "C" int func_08971010(int, int);

struct A51Stat {
    static void sub(int n) {
        A51World *w = D_09B6F478;
        int d = func_08971010(w->w18, n);
        w->cnt[w->row->info->slot] -= d;
    }
};
#endif
