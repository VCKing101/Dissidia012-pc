extern int func_08B019E4(void *self);
extern void func_08B02308(void *self);
extern void func_08B026C0(void *self);

int func_08B01870(void *self)
{
    if (func_08B019E4(self) == 0) {
        return -1;
    }
    func_08B02308(self);
    func_08B026C0(self);
    return 0;
}
