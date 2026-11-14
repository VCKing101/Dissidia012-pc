typedef struct HandleList {
    unsigned char active;
    unsigned char busy;
    char pad_02[0x2];
    void *handles[4];
} HandleList;

extern "C" {

extern void func_088DA060(void *handle);

int func_089F7E40(HandleList *self)
{
    if (!self->active) {
        return -1;
    }
    for (int i = 0; i < 4; i++) {
        if (self->handles[i] != 0) {
            func_088DA060(self->handles[i]);
            self->handles[i] = 0;
        }
    }
    self->busy = 0;
    return 0;
}

}
