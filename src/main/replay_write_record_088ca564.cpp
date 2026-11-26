// Replay recorder: appends one 3-byte (big-endian) record for a fighter's finished run, or marks the replay full.
#include "main/replay_types.h"
extern "C" int func_088CA658(int mask, int run);
extern "C" void func_088CA564(D012Replay *r, unsigned int fighter)
{
    int code;
    if (fighter >= 2) return;
    if (r->record_count[fighter] >= D012_REPLAY_SLOTS) { r->flags |= D012_REPLAY_FULL; return; }
    if (r->rec_run[fighter] == 0) return;
    code = func_088CA658(r->rec_last[fighter], r->rec_run[fighter]);
    int n = r->record_count[fighter];
    int k = n * 3;
    unsigned char *code_bytes = (unsigned char *)&code;
    for (int i = 2; i >= 0; i--) {
        r->records[fighter][k] = code_bytes[i];
        k++;
    }
    r->record_count[fighter] = n + 1;
}
