typedef struct HandleBinding {
    unsigned char active;
    void *handle;
} HandleBinding;

extern void func_08ABA0B4(HandleBinding *self);

/* Reset, then bind handle; -1 without one. */
int func_08ABA0D0(HandleBinding *self, void *handle)
{
    func_08ABA0B4(self);
    if (handle == 0) {
        return -1;
    }
    self->handle = handle;
    self->active = 1;
    return 0;
}
