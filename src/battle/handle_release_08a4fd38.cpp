struct HandleSetFD38 {
    char pad_00[0x38];
    void *handles[8];
};

extern "C" {

extern void func_08A50024(HandleSetFD38 *self);
extern void func_088DA060(void *handle);

void func_08A4FD38(HandleSetFD38 *self)
{
    func_08A50024(self);
    for (int i = 0; i < 8; i++) {
        if (self->handles[i] != 0) {
            func_088DA060(self->handles[i]);
            self->handles[i] = 0;
        }
    }
}

}
