typedef struct LazyOwner {
    char pad_00[0x8];
    void *created;
} LazyOwner;

extern unsigned char func_08AC9284(LazyOwner *self);

/* 1 when the owner is ready but its object is not created yet. */
int func_08AC92AC(LazyOwner *self)
{
    if (func_08AC9284(self) == 0) {
        return 0;
    }
    if (self->created != 0) {
        return 0;
    }
    return 1;
}
