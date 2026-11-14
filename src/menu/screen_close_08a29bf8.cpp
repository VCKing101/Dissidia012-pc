struct Block29BF8 {
    char pad_000[0x884];
    int handle_884;
    char pad_888[0x1030 - 0x888];
    void (*callback)(int);
};

struct Screen29BF8 {
    char pad_00[0x134];
    int pool;
    Block29BF8 *block;
};

template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

extern "C" {
extern void *D_089D7830;
extern void func_08A2958C(Screen29BF8 *self);
extern void func_0885C538(int pool, void *handle, int flag);
extern void func_088D0370(void *loader, int a, int b);
extern void func_08A2BC30(Screen29BF8 *self);
}

/* Inlined call on the loader in D_089D7830. */
struct Loader29BF8 {
    static void Call(int a, int b) {
        func_088D0370(D_089D7830, a, b);
    }
};

extern "C" void func_08A29BF8(Screen29BF8 *self)
{
    func_08A2958C(self);
    func_0885C538(self->pool, (void *)self->block->handle_884, 0);
    self->block->handle_884 = 0;
    int kind = 10;
    func_088D0370(D_089D7830, kind, 1);
    Kept<void (*)(int)> saved(self->block->callback);
    func_0885C538(self->pool, self->block, 0);
    self->block = 0;
    func_08A2BC30(self);
    void (*callback)(int) = saved.value;
    if (callback != 0) {
        callback(0);
    }
}
