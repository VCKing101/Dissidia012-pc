// Run the pre-step, store the two arguments, register the callback and enter state 5.
extern "C" void func_089160F0(void);
extern "C" void func_08B49C54(int h);
extern "C" void func_08916F78(int a, void (*cb)(void));
extern "C" void func_08917830(void);
extern "C" void func_08914B64(int self, int s);
extern "C" int func_08838074(int h);
struct Obj {
    char pad0[0x570];
    int h570;
    char pad1[0x6D0 - 0x574];
    int h6D0;
    char pad2[0x768 - 0x6D4];
    int a768;
    int a76C;
    char pad3[0x7DB - 0x770];
    char f7DB;
};
struct Kept {
    Obj *v;
    Kept(Obj *x) { v = x; }
};
extern "C" void func_08914D18(Obj *self, int a, int b)
{
    Kept k(self);
    func_089160F0();
    func_08B49C54(k.v->h570);
    Obj *o = k.v;
    o->a768 = a;
    o->a76C = b;
    func_08916F78(0, func_08917830);
    func_08914B64((int)k.v, 5);
    int r = func_08838074(k.v->h6D0);
    Obj *o2 = k.v;
    if (r == 0) {
        o2->f7DB = 1;
    }
}
