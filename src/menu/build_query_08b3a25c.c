/* Builds a five-word query on the stack (first word the argument, rest -1) and passes it to func_08B3A298. */
extern void func_08B3A298(void *self, int *q, int one);
void func_08B3A25C(void *self, int a)
{
    int q[5];
    q[0] = 0;
    q[1] = -1;
    q[2] = -1;
    q[3] = -1;
    *(int *)((char *)q + 0) = a;
    q[4] = -1;
    func_08B3A298(self, q, 1);
}
