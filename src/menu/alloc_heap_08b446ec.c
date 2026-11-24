/* Allocates size bytes from the heap held at the first word of D_08B44F08. */
extern int *D_08B44F08;
extern int func_0885C6CC(int heap, int size, int a, int b, int c, int d);
int func_08B446EC(int size)
{
    int one = 1;
    return func_0885C6CC(D_08B44F08[0], size, 0, 0, one, 0);
}
