/* Calls func_088E9C14(D_089D7E88), then registers the "MainTask" (prio 1000) and "DrawTask" (prio 110000) tasks on D_089D7E88 + 4. */
extern char *D_089D7E88;
extern char D_089CC834[];
extern char D_089CC840[];
extern void func_088E9A2C(void);
extern void func_088E9A4C(void);
extern void func_088E9C14(char *w);
extern void func_08865088(void *work, int a, int prio, void (*fn)(void), char *name);
void func_088E9968(void)
{
    func_088E9C14(D_089D7E88);
    func_08865088(D_089D7E88 + 4, -1, 1000, func_088E9A2C, D_089CC834);
    func_08865088(D_089D7E88 + 4, -1, 110000, func_088E9A4C, D_089CC840);
}
