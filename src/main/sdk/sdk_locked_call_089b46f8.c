extern void func_089A25C0(void);
extern void *func_089A0144(int *info);
extern int func_089B4480(void *dst, void **handle, int *info, int a, int b, int c, int d);
extern void func_089A0180(void *handle);
extern void func_089A296C(int mode);
extern void func_089A2568(void);

int func_089B46F8(void *dst, int a, int b, int c, int d)
{
    int info;
    void *handle;
    int ret;

    func_089A25C0();
    handle = func_089A0144(&info);
    ret = func_089B4480(dst, &handle, &info, a, b, c, d);
    func_089A0180(handle);
    func_089A296C(2);
    func_089A2568();
    return ret;
}
