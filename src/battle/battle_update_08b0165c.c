extern int func_08B019E4(void *self);
extern void func_08B01EE4(void *self);
extern void func_08B0433C(void *self);
extern void func_08B02BF8(void *self);
extern void func_08B031AC(void *self);
extern void func_08B01C14(void *self);
extern void func_08B01CEC(void *self);
extern void func_08B0203C(void *self);
extern void func_08B0452C(void *self);
extern void func_08B02164(void *self);
extern void func_08B04870(void *self);
extern void func_08B02228(void *self);

int func_08B0165C(void *self)
{
    if (func_08B019E4(self) == 0) {
        return -1;
    }
    func_08B01EE4(self);
    func_08B0433C(self);
    func_08B02BF8(self);
    func_08B031AC(self);
    func_08B01C14(self);
    func_08B01CEC(self);
    func_08B0203C(self);
    func_08B0452C(self);
    func_08B02164(self);
    func_08B04870(self);
    func_08B02228(self);
    return 0;
}
