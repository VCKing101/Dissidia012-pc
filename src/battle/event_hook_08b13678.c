/* On event 5, kicks the object at D_089D8364 through func_089072AC and func_08906F6C. */
extern void *D_089D8364;
extern void func_089072AC(void *o, int v);
extern void func_08906F6C(void *o);
void func_08B13678(void *self, int event)
{
    void *keep[1];
    switch (event) {
    case 0:
        break;
    case 5:
        if (D_089D8364 != 0) {
            keep[0] = D_089D8364;
            func_089072AC(D_089D8364, 1);
            func_08906F6C(keep[0]);
        }
        break;
    }
}
