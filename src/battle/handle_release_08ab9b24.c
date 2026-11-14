typedef struct HandleOwner {
    char pad_00[0x4];
    void *handle;
} HandleOwner;

extern void func_08ABB85C(void *handle);
extern void func_08AB9B18(HandleOwner *self);

/* Release the held handle, then clear the owner. */
void func_08AB9B24(HandleOwner *self)
{
    if (self->handle != 0) {
        func_08ABB85C(self->handle);
    }
    func_08AB9B18(self);
}
