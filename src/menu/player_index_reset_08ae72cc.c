/* Resets the per-player index tables to 0..n-1 and then runs two per-entry update passes over every player's entries. */
typedef struct B8E72CC { char p0[0x26A0]; int cnt[4]; char p1[0x7EC0 - 0x26B0]; unsigned short idx[4][0x2BF]; } B8E72CC;
typedef struct S8E72CC { char pad[0xA690]; B8E72CC *b; } S8E72CC;
extern void func_08AE70EC(S8E72CC *self, int p, int k);
extern void func_08AE6EC8(S8E72CC *self, int p, int k);
void func_08AE72CC(S8E72CC *self)
{
    int p;
    int k;
    for (p = 0; p < 4; p++) {
        for (k = 0; k < self->b->cnt[p]; k++) {
            self->b->idx[p][k] = k;
        }
    }
    for (p = 0; p < 4; p++) {
        for (k = 0; k < self->b->cnt[p]; k++) {
            func_08AE70EC(self, p, k);
        }
    }
    for (p = 0; p < 4; p++) {
        for (k = 0; k < self->b->cnt[p]; k++) {
            func_08AE6EC8(self, p, k);
        }
    }
}
