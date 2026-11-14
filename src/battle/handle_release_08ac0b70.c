typedef struct HandleOwner {
    char pad_00[0x8];
    void *handle;
} HandleOwner;

extern void func_08ABB85C(void *handle);
extern void func_08AC0B60(HandleOwner *self);

/* Release the held handle, then clear the owner. */
void func_08AC0B70(HandleOwner *self)
{
    if (self->handle != 0) {
        func_08ABB85C(self->handle);
    }
    func_08AC0B60(self);
}
