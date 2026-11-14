typedef struct ReleaseItem {
    char data[0x24];
} ReleaseItem;

typedef struct ReleaseSet {
    char pad_00[0x8];
    void *handle;
    char pad_0C[0x10];
    ReleaseItem items[2];
} ReleaseSet;

extern "C" {

extern void func_08B0532C(ReleaseItem *item);
extern void func_088DAB44(void *handle);
extern void func_08B04C8C(ReleaseSet *self);

void func_08B04CA0(ReleaseSet *self)
{
    for (int i = 0; i < 2; i++) {
        func_08B0532C(&self->items[i]);
    }
    if (self->handle != 0) {
        func_088DAB44(self->handle);
        self->handle = 0;
    }
    func_08B04C8C(self);
}

}
