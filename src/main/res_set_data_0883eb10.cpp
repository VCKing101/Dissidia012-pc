/* Replaces a resource's data with a named copy of the given bytes. */
struct Res0883EB10 {
    char name[0x20];
    void *resource;
    int size;
};

extern "C" {

extern char D_089C8148[];

void func_0883ECC4(Res0883EB10 *self);
void *func_088A51C0(int size, char *tag, const char *name);
void *func_088A5208(void *resource);
void func_088A522C(void *resource);
void func_08935524(void *dst, const void *src, int size);
void func_089359A4(char *dst, const char *src);

void func_0883EB10(Res0883EB10 *self, const char *name, const void *data, int size)
{
    func_0883ECC4(self);
    if (data != 0) {
        self->resource = func_088A51C0(size, D_089C8148, name);
        if (self->resource != 0) {
            func_08935524(func_088A5208(self->resource), data, size);
            func_088A522C(self->resource);
            func_089359A4(self->name, name);
            self->size = size;
        }
    }
}

}
