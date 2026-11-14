typedef struct HandleOwner {
    char pad_00[0x8];
    void *handle;
} HandleOwner;

extern void func_08842138(void *handle);
extern void func_08AB7600(HandleOwner *self);

/* Release the held handle, then clear the owner. */
void func_08AB7610(HandleOwner *self)
{
    if (self->handle != 0) {
        func_08842138(self->handle);
    }
    func_08AB7600(self);
}
