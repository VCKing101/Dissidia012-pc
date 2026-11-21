typedef struct Font0887A3C8 {
    unsigned char loaded;              /* +0x00 */
    unsigned char unk_01[3];
    void *handle;                      /* +0x04 */
    unsigned char cached;              /* +0x08 */
} Font0887A3C8;

extern void func_0887A354(Font0887A3C8 *self);
extern void *func_0887CFE4(void);
extern void *func_0887D330(void *cache, const char *name, int size);
extern void *func_0887AD78(const char *name, int a, int size, int b, int c, int d);

/* Open a font: from the cache when it is there, else directly. 0, -1 (bad arguments) or -2. */
int func_0887A3C8(Font0887A3C8 *self, const char *name, int size)
{
    func_0887A354(self);
    if (name == 0 || size < 0) {
        return -1;
    }
    self->handle = func_0887D330(func_0887CFE4(), name, size);
    if (self->handle != 0) {
        self->cached = 1;
    } else {
        self->handle = func_0887AD78(name, 0, size, 0, 0, 0);
        if (self->handle == 0) {
            return -2;
        }
        self->cached = 0;
    }
    self->loaded = 1;
    return 0;
}
