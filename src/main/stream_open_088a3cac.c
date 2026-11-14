/* Opens a stream under the next id: the id on success, 0 on failure. */
extern int D_089D6D38;

int func_088A3F90(int a, int b, int id);

int func_088A3CAC(int unused, int a, int b)
{
    int id = D_089D6D38;
    if (func_088A3F90(a, b, id) == 1) {
        D_089D6D38 = (D_089D6D38 + 1) | 0x80000000;
        return id;
    }
    return 0;
}
