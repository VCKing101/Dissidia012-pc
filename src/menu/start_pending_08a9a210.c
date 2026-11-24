/* Runs two setup routines; when the second succeeds, optionally runs a follow-up, queries the global and, if it succeeds, starts it and sets the owner's state to 0x34. */
extern int D_089D69E8;
extern void func_08A9C538(void);
extern int func_08A9C608(int self);
extern void func_08AB3BA0(void);
extern int func_08885608(int g, int a);
extern void func_08885220(int g);
extern void func_088D3CB0(int p, int n);
void func_08A9A210(int self) {
    int r;
    int zero;
    func_08A9C538();
    r = func_08A9C608(self);
    if (r == 0) {
        return;
    }
    if (*(int *)(*(int *)(self + 0x154) + 0x8AE0) != 0) {
        func_08AB3BA0();
    }
    zero = 0;
    r = func_08885608(D_089D69E8, zero);
    if (r == 0) {
        return;
    }
    func_08885220(D_089D69E8);
    func_088D3CB0(*(int *)(self + 0x154) + 0x404, 0x120);
    *(int *)(*(int *)(self + 0x154) + 0x910) = 0x34;
}
