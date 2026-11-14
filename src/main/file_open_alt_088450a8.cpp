/* Opens an entry's file, falling back to its alternate entry when the file
   is missing; returns the open result and switches the entry on success. */
struct Info088450A8 {
    char pad_00[0x24];
    char name[0x10];
    char name2[1];
};

extern "C" {

char *func_08844FA4(char *name, char *format);
int func_088FE708(char *path, int a, int b, unsigned char c);
Info088450A8 *func_08844FE4(Info088450A8 *info);

int func_088450A8(Info088450A8 **self, char *format, int a, int b, unsigned char c)
{
    int r = func_088FE708(func_08844FA4((*self)->name, format), a, b, c);
    if (r <= 0) {
        Info088450A8 *alt = func_08844FE4(*self);
        if (alt != 0) {
            r = func_088FE708(func_08844FA4(alt->name, format), a, b, 0);
            *self = alt;
        }
    }
    return r;
}

int func_0884516C(Info088450A8 **self, char *format, int a, int b, unsigned char c)
{
    int r = func_088FE708(func_08844FA4((*self)->name2, format), a, b, c);
    if (r <= 0) {
        Info088450A8 *alt = func_08844FE4(*self);
        if (alt != 0) {
            r = func_088FE708(func_08844FA4(alt->name2, format), a, b, 0);
            *self = alt;
        }
    }
    return r;
}

}
