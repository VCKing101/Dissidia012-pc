/* Calls func_0891813C(D_089D83D8), then registers the "MainTask" (prio 1000) and "DrawTask" (prio 110000) tasks, group 3, on D_089D83D8 + 4. */
extern char *D_089D83D8;
extern char D_089CE400[];
extern char D_089CE40C[];
extern void func_08917210(void);
extern void func_08917230(void);
extern void func_0891813C(char *w);
extern void func_08865088(void *work, int a, int prio, void (*fn)(void), char *name);
void func_0891714C(void)
{
    func_0891813C(D_089D83D8);
    func_08865088(D_089D83D8 + 4, 3, 1000, func_08917210, D_089CE400);
    func_08865088(D_089D83D8 + 4, 3, 110000, func_08917230, D_089CE40C);
}
