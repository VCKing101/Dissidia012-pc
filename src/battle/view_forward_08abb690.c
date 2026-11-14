typedef struct ViewOwner {
    char pad_000[0xDA];
    unsigned char hidden;
    char pad_0DB[0x81];
    char field_15C[0x4];
} ViewOwner;

extern int func_08ABB474(ViewOwner *self);
extern int func_08ABD810(void *view, int a, int b, int c);

int func_08ABB690(ViewOwner *self, int a, int b, int c)
{
    if (func_08ABB474(self) == 0) {
        return -1;
    }
    if (self->hidden == 1) {
        return 0;
    }
    return func_08ABD810(self->field_15C, a, b, c);
}
