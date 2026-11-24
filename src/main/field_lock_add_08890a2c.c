/* Adds a hold on one field lock counter and refreshes the lock state. */
extern int D_09B49208[];

extern void func_08890A80(int lock);

void func_08890A2C(int lock)
{
    D_09B49208[lock]++;
    func_08890A80(lock);
}
