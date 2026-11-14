/* Detaches the bound actor's controller and clears the binding flags. */
typedef struct Actor08823394 {
    char pad_000[0x380];
    unsigned int flags;
    char pad_384[0x10];
    void *controller;
} Actor08823394;

typedef struct Binding08823394 {
    char pad_00[0x20];
    unsigned int flags;
    Actor08823394 *actor;
} Binding08823394;

void func_0886CA58(void *controller);
void func_0886B240(void *controller);

void func_08823394(Binding08823394 *self)
{
    void *controller;
    if (!(self->flags & 8)) {
        return;
    }
    controller = self->actor->controller;
    func_0886CA58(controller);
    func_0886B240(controller);
    self->actor->flags &= ~1;
    self->flags &= ~8;
}
