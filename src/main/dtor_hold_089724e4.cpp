// Destructor: release the two held objects and free the object itself when asked.
extern "C" void func_089C2CE4(void);
extern "C" void func_089C2D2C(void);
extern "C" void func_08993754(int obj, int a, int b, void (*f)(void), void (*g)(void), int c);
extern "C" void func_089748D0(int obj);
struct Obj { int pad; int first; int second; };
struct D {
    static void tail(Obj *self, unsigned flags)
    {
        if (self->second != 0) {
            func_089748D0(self->second);
            self->second = 0;
        }
        if (flags & 1) {
            func_089748D0((int)self);
        }
    }
};
extern "C" void func_089724E4(Obj *self, unsigned flags)
{
    if (self != 0) {
        if (self->first != 0) {
            func_08993754(self->first, -1, 4, func_089C2CE4, func_089C2D2C, 0);
            self->first = 0;
        }
        D::tail(self, flags);
    }
}
