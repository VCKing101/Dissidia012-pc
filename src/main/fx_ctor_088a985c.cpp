/* Constructor and deleting destructor of the effect base (vtable at 0x2C). */
extern "C" {
extern char D_089CB248[];
void func_08992F04(void *p);
}

struct FxBase088A985C {
    int field_00;
    unsigned char field_04;
    char pad_05[0x1F];
    unsigned char enabled;
    char pad_25[7];
    void *vtable;

    static FxBase088A985C *construct(FxBase088A985C *fx)
    {
        fx->vtable = D_089CB248;
        fx->field_00 = 0;
        fx->enabled = 1;
        fx->field_04 = 0;
        return fx;
    }

    static void destroy(FxBase088A985C *fx, unsigned int flags)
    {
        if (fx != 0) {
            fx->vtable = D_089CB248;
            if (flags & 1) {
                func_08992F04(fx);
            }
        }
    }
};

extern "C" {

FxBase088A985C *func_088A985C(FxBase088A985C *fx)
{
    return FxBase088A985C::construct(fx);
}

void func_088A9880(FxBase088A985C *fx, unsigned int flags)
{
    FxBase088A985C::destroy(fx, flags);
}

}
