/* Constructs the shared default effect and registers its destructor. */
typedef struct Fx088A94A0 {
    char pad_00[0x2C];
    void *vtable;
    void *listener_vtable;
} Fx088A94A0;

extern Fx088A94A0 D_09B4D440;
extern char D_089CB1B0[];
extern char D_089CB1F8[];
extern char D_089D6DB8[];

void func_088A985C(Fx088A94A0 *fx);
void func_08992F6C(void *entry);

void func_088A94A0(void)
{
    Fx088A94A0 *fx = &D_09B4D440;
    func_088A985C(fx);
    fx->vtable = D_089CB1B0;
    fx->listener_vtable = D_089CB1F8;
    func_08992F6C(D_089D6DB8);
}
