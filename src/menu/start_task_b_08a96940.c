/* Sets up the global object, starts a task with a callback and notifies it. */
extern int D_089D8508;
extern int D_089D6A10;
extern void func_0891ACF0(void *buf, int p);
extern void func_0891B8C0(int obj, void *buf);
extern void func_0891B96C(int obj, int a, int b);
extern void func_08889ED4(int a, void *cb, int b);
extern void func_0888964C(int task, int a, int b, int c, int d);
extern void func_08889808(int task);
extern void func_08A96A08(void);
void func_08A96940(int a0, int p) {
    char buf[8];
    int obj = D_089D8508;
    int task;
    func_0891ACF0(buf, p);
    task = D_089D6A10;
    func_0891B8C0(obj, buf);
    func_0891B96C(obj, 0, 0);
    func_08889ED4(2, (void *)func_08A96A08, 1);
    func_0888964C(task, 3, 0, 0, 0);
    func_08889808(task);
}
