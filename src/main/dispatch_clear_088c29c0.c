// Clears the first two words of a record, then calls handler table D_089D74E0[kind].
extern void (*D_089D74E0[])(int *, int, int);
void func_088C29C0(int *rec, int a, int kind, int b)
{
    rec[0] = 0;
    rec[1] = 0;
    D_089D74E0[kind](rec, a, b);
}
