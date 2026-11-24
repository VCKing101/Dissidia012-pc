/* Frees the effect handle at +0xC8, creates a new one from the object's id at +0x18 and refreshes the panel. */
extern int D_089D6998;
extern char D_08A61D78[];
extern void func_0887BA5C(void);
extern int func_0887D868(int mgr, int a, int b, int c, int d, int e, int f, int g);
extern void func_088D428C(int a, int b, void *c, int d);
void func_08A60AB0(int arg) {
    int self = arg;
    int h;
    if (*(int *)(self + 0xC8) != 0) {
        func_0887BA5C();
        *(int *)(self + 0xC8) = 0;
    }
    h = func_0887D868(D_089D6998, 0, *(int *)(*(int *)(self + 0x128) + 0x18), -1, 0, 0, 0, 0);
    *(int *)(self + 0xC8) = h;
    func_088D428C(self + 0x40, self + 0xA4, D_08A61D78, 1);
}
