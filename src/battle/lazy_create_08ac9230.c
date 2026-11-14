typedef struct LazyOwner {
    char pad_00[0x8];
    void *created;
} LazyOwner;

extern int func_08AC9284(LazyOwner *self);
extern void func_08AC9D7C(LazyOwner *self);

/* Create the object on first use once the owner is ready. */
int func_08AC9230(LazyOwner *self)
{
    if (func_08AC9284(self) == 0) {
        return -1;
    }
    if (self->created == 0) {
        func_08AC9D7C(self);
    }
    return 0;
}
