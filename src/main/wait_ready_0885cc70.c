extern unsigned char D_089D4E30;
extern int (*D_089D4E20)(void *arg);
extern void *D_089D4E24;
extern void (*D_089D4E28)(void *arg);
extern void *D_089D4E2C;

void func_0885CC70(void)
{
    int done = 1;
    unsigned char ready = D_089D4E30;

    while (ready == 0 || done == 0) {
        if (D_089D4E20 != 0) {
            done = D_089D4E20(D_089D4E24);
            ready = D_089D4E30;
        }
    }
    if (D_089D4E28 != 0) {
        D_089D4E28(D_089D4E2C);
    }
}
