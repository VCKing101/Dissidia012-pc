/* Tear down the lobby helpers: release the handle if valid, close the table and unregister. */
extern int D_089D8370;
extern int D_089D6C80;
extern int *D_089D8378;
extern int D_089D83D8;
extern void func_08912D94(void);
extern void func_08913310(void);
extern void func_088A0DFC(int a, int b);
extern void func_0890F264(int *p);
extern void func_088CDC60(int p);
void func_0890EC88(void)
{
    func_08912D94();
    func_08913310();
    if (D_089D8370 >= 0) {
        func_088A0DFC(D_089D6C80, D_089D8370);
        D_089D8370 = -1;
    }
    func_0890F264(D_089D8378);
    func_088CDC60(D_089D83D8 + 0x3C0);
}
