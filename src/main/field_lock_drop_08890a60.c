/* Drops a hold on one field lock counter. */
extern int D_09B49208[];

void func_08890A60(int lock)
{
    D_09B49208[lock]--;
}
