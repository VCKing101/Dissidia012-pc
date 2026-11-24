/* If the word at 0 of the fourth argument is set, subtracts its stat cost from the world's counter and clears it. */
#include "main/agent51_types.h"
extern "C" void func_08963DD8(int a0, A51World *w, int a2, int *a3) {
    int *q = a3;
    A51World *ww = w;
    if (q[0] != 0) {
        int d = func_08971010(ww->w18, q[0]);
        ww->cnt[ww->row->info->slot] -= d;
        q[0] = 0;
    }
}
