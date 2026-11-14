struct Transfer {
    int state;
    int handle;
    int user;
    void *buffer;
    int size;
    int heap;
    virtual void complete(void *buffer, int size, int user);
};

extern "C" {
void func_0885C538(int heap, void *buffer, int flags);
void func_0886300C(int handle);

void func_08861E2C(Transfer *self)
{
    self->complete(self->buffer, self->size, self->user);
    func_0885C538(self->heap, self->buffer, 0);
    self->buffer = 0;
    self->size = 0;
    func_0886300C(self->handle);
    self->handle = -1;
    self->state = 3;
}
}
