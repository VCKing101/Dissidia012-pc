typedef struct HandlePair {
    void *owned;
    char pad_04[0xC];
    void *handle;
} HandlePair;

extern void func_088DA060(void *handle);
extern void func_08AB564C(HandlePair *self);

/* Release the handle at 0x10, then the owned object. */
void func_08AB54E4(HandlePair *self)
{
    if (self->handle != 0) {
        func_088DA060(self->handle);
        self->handle = 0;
    }
    if (self->owned != 0) {
        func_08AB564C(self);
        self->owned = 0;
    }
}
