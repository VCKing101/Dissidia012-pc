typedef struct HandleOne {
    void *handle;
} HandleOne;

extern void func_08890970(void *handle);

void func_08B05C94(HandleOne *self)
{
    if (self->handle != 0) {
        func_08890970(self->handle);
        self->handle = 0;
    }
}
